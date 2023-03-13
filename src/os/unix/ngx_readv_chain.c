
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>

#ifdef __KOS__
static ssize_t
kos_readv(int fd, const struct iovec *iov, unsigned long iovCount, ngx_log_t *log) {
    // Validate at least one array is there to write to
    if (0 >= iovCount) {
        errno = EINVAL;
        return -1;
    }

    /*
     * Use the iovcnt int and the iov_len to figure out the size of the buffer we need
     * then use that length to create some memory to read the bytes from the descriptor,
     * then populate the arrays of the iovec struct with this data from memory
     */
    ssize_t totalLength = 0;
    for (int counter = 0; counter < iovCount; counter++) {
        if (0 > iov[counter].iov_len) {
            ngx_log_error(NGX_LOG_ALERT, log, NGX_EINVAL, "IO vector has negative length");
            errno = EINVAL;
            return -1;
        }
        totalLength = iov[counter].iov_len + totalLength;
    }

    // Make sure the total Length of iovec structure arrays is less than SIZE_MAX
    if (totalLength > SIZE_MAX) {
        errno = EINVAL;
        return -1;
    } else if (totalLength == 0) {
        return 0;
    }

    // use malloc to read the descriptor and store the data
    char *buffer = malloc(totalLength);
    memset(buffer, 0, totalLength);

    // Read from the descriptor and print it out
    ssize_t readBytes = read(fd, buffer, (size_t) totalLength);
    if (0 > readBytes) {
        ngx_log_error(NGX_LOG_ALERT, log, errno, "%s: reading failed", __func__);
        return -1;
    }

    // use memcpy to copy the contents to the original buffers
    size_t currentMemoryLocation = 0;

    for (int counter = 0; counter < iovCount; counter++) {
        /*
         * The memcpy function returns a pointer to a destination
         *
         * Failure to observe the requirement that the memory areas do not overlap has been the source of significant
         * bugs.
         *
         * Arguments:
         * void *memcpy(void *dest, const void *src, size_t n);
         *
         * Note: the second argument is using pointer arithmetic to find the location from the malloced array
         */
        memcpy(iov[counter].iov_base,
               buffer + currentMemoryLocation,
               iov[counter].iov_len);

        currentMemoryLocation += iov[counter].iov_len;
    }

    // Free the bytes acquired through malloc
    free(buffer);

    // Return success
    return readBytes;
}
#endif // __KOS__

ssize_t
ngx_readv_chain(ngx_connection_t *c, ngx_chain_t *chain, off_t limit)
{
    u_char        *prev;
    ssize_t        n, size;
    ngx_err_t      err;
    ngx_array_t    vec;
    ngx_event_t   *rev;
    struct iovec  *iov, iovs[NGX_IOVS_PREALLOCATE];

    rev = c->read;

#if (NGX_HAVE_KQUEUE)

    if (ngx_event_flags & NGX_USE_KQUEUE_EVENT) {
        ngx_log_debug3(NGX_LOG_DEBUG_EVENT, c->log, 0,
                       "readv: eof:%d, avail:%d, err:%d",
                       rev->pending_eof, rev->available, rev->kq_errno);

        if (rev->available == 0) {
            if (rev->pending_eof) {
                rev->ready = 0;
                rev->eof = 1;

                ngx_log_error(NGX_LOG_INFO, c->log, rev->kq_errno,
                              "kevent() reported about an closed connection");

                if (rev->kq_errno) {
                    rev->error = 1;
                    ngx_set_socket_errno(rev->kq_errno);
                    return NGX_ERROR;
                }

                return 0;

            } else {
                return NGX_AGAIN;
            }
        }
    }

#endif

#if (NGX_HAVE_EPOLLRDHUP)

    if (ngx_event_flags & NGX_USE_EPOLL_EVENT) {
        ngx_log_debug2(NGX_LOG_DEBUG_EVENT, c->log, 0,
                       "readv: eof:%d, avail:%d",
                       rev->pending_eof, rev->available);

        if (rev->available == 0 && !rev->pending_eof) {
            return NGX_AGAIN;
        }
    }

#endif

    prev = NULL;
    iov = NULL;
    size = 0;

    vec.elts = iovs;
    vec.nelts = 0;
    vec.size = sizeof(struct iovec);
    vec.nalloc = NGX_IOVS_PREALLOCATE;
    vec.pool = c->pool;

    /* coalesce the neighbouring bufs */

    while (chain) {
        n = chain->buf->end - chain->buf->last;

        if (limit) {
            if (size >= limit) {
                break;
            }

            if (size + n > limit) {
                n = (ssize_t) (limit - size);
            }
        }

        if (prev == chain->buf->last) {
            iov->iov_len += n;

        } else {
            if (vec.nelts == vec.nalloc) {
                break;
            }

            iov = ngx_array_push(&vec);
            if (iov == NULL) {
                return NGX_ERROR;
            }

            iov->iov_base = (void *) chain->buf->last;
            iov->iov_len = n;
        }

        size += n;
        prev = chain->buf->end;
        chain = chain->next;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_EVENT, c->log, 0,
                   "readv: %ui, last:%uz", vec.nelts, iov->iov_len);

    do {
#ifdef __KOS__
        n = kos_readv(c->fd, (struct iovec *) vec.elts, vec.nelts, c->log);
#else
        n = readv(c->fd, (struct iovec *) vec.elts, vec.nelts);
#endif
        if (n == 0) {
            rev->ready = 0;
            rev->eof = 1;

#if (NGX_HAVE_KQUEUE)

            /*
             * on FreeBSD readv() may return 0 on closed socket
             * even if kqueue reported about available data
             */

            if (ngx_event_flags & NGX_USE_KQUEUE_EVENT) {
                rev->available = 0;
            }

#endif

            return 0;
        }

        if (n > 0) {

#if (NGX_HAVE_KQUEUE)

            if (ngx_event_flags & NGX_USE_KQUEUE_EVENT) {
                rev->available -= n;

                /*
                 * rev->available may be negative here because some additional
                 * bytes may be received between kevent() and readv()
                 */

                if (rev->available <= 0) {
                    if (!rev->pending_eof) {
                        rev->ready = 0;
                    }

                    rev->available = 0;
                }

                return n;
            }

#endif

#if (NGX_HAVE_FIONREAD)

            if (rev->available >= 0) {
                rev->available -= n;

                /*
                 * negative rev->available means some additional bytes
                 * were received between kernel notification and readv(),
                 * and therefore ev->ready can be safely reset even for
                 * edge-triggered event methods
                 */

                if (rev->available < 0) {
                    rev->available = 0;
                    rev->ready = 0;
                }

                ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0,
                               "readv: avail:%d", rev->available);

            } else if (n == size) {

                if (ngx_socket_nread(c->fd, &rev->available) == -1) {
                    n = ngx_connection_error(c, ngx_socket_errno,
                                             ngx_socket_nread_n " failed");
                    break;
                }

                ngx_log_debug1(NGX_LOG_DEBUG_EVENT, c->log, 0,
                               "readv: avail:%d", rev->available);
            }

#endif

#if (NGX_HAVE_EPOLLRDHUP)

            if ((ngx_event_flags & NGX_USE_EPOLL_EVENT)
                && ngx_use_epoll_rdhup)
            {
                if (n < size) {
                    if (!rev->pending_eof) {
                        rev->ready = 0;
                    }

                    rev->available = 0;
                }

                return n;
            }

#endif

            if (n < size && !(ngx_event_flags & NGX_USE_GREEDY_EVENT)) {
                rev->ready = 0;
            }

            return n;
        }

        err = ngx_socket_errno;

        if (err == NGX_EAGAIN || err == NGX_EINTR) {
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, c->log, err,
                           "readv() not ready");
            n = NGX_AGAIN;

        } else {
            n = ngx_connection_error(c, err, "readv() failed");
            break;
        }

    } while (err == NGX_EINTR);

    rev->ready = 0;

    if (n == NGX_ERROR) {
        c->read->error = 1;
    }

    return n;
}
