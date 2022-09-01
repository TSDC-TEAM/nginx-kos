#include <kos_net.h>

int main(void)
{
    /* Initialisation of network interface "en0". */
    if (!configure_net_iface(DEFAULT_INTERFACE, DEFAULT_ADDR, DEFAULT_MASK, DEFAULT_GATEWAY, DEFAULT_MTU))
    {
        perror("can not init network");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
