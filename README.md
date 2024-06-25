# nginx web server for KasperskyOS

>This version of nginx is adapted for KasperskyOS.

## What is an nginx web server for KasperskyOS?

The nginx web server for KasperskyOS is based on the nginx [1.25.1](https://github.com/nginx/nginx/releases/tag/release-1.25.1). Please refer to the <http://nginx.org/en/docs/> for more information that are not related to this project.

For the nginx web server for KasperskyOS the number of worker processes can only be 1. The [`worker_processes`](http://nginx.org/en/docs/ngx_core_module.html#worker_processes) directive in the nginx configuration file is ignored.

For additional details on KasperskyOS, including its limitations and known issues, please refer to the [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_community_edition).

## Table of contents

- [nginx web server for KasperskyOS](#nginx-web-server-for-kasperskyos)
  - [What is an nginx web server for KasperskyOS?](#what-is-an-nginx-web-server-for-kasperskyos)
  - [Table of contents](#table-of-contents)
  - [Getting started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Building the nginx web server for KasperskyOS](#building-the-nginx-web-server-for-kasperskyos)
    - [Installing and removing the nginx web server for KasperskyOS](#installing-and-removing-the-nginx-web-server-for-kasperskyos)
  - [Usage](#usage)
    - [Example](#example)
  - [Contributing](#contributing)
  - [License](#license)

## Getting started

### Prerequisites

1. [Install](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_sdk_install_and_remove) the KasperskyOS Community Edition SDK. You can download the latest version of KasperskyOS Community Edition for free from [os.kaspersky.com](https://os.kaspersky.com/development/). Minimum required version of the KasperskyOS Community Edition SDK is 1.2. For more information, see [System requirements](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_system_requirements).
1. Copy project sources files to your home directory. All files that are required to build the nginx web server for KasperskyOS and examples of KasperskyOS-based solutions are located in the following directory:
   ```
   ./kos
   ```
1. Set the environment variable `SDK_PREFIX` to `/opt/KasperskyOS-Community-Edition-<version>`, where `version` is the version of the KasperskyOS Community Edition SDK that you installed. To do this, run the following command:
   ```
   $ export SDK_PREFIX=/opt/KasperskyOS-Community-Edition-<version>
   ```

### Building the nginx web server for KasperskyOS

Run the following script to build the nginx web server for KasperskyOS:
```
./kos/nginx/cross-build.sh
```
The nginx web server for KasperskyOS is built using the CMake build system, which is provided in the KasperskyOS Community Edition SDK.

### Installing and removing the nginx web server for KasperskyOS

To install the nginx web server for KasperskyOS to the KasperskyOS Community Edition SDK, run the following script with root privileges:
```
./kos/nginx/install.sh
```

To remove the nginx web server for KasperskyOS from the KasperskyOS Community Edition SDK, run the following script with root privileges:
```
./kos/nginx/uninstall.sh
```

[⬆ Back to Top](#Table-of-contents)

## Usage

When you develop a KasperskyOS-based solution, use the [recommended structure of project directories](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_using_sdk_cmake) to simplify usage of CMake scripts.

To include the nginx web server in your KasperskyOS-based solution, follow these steps:

1. Add the `find_package()` command to the `./CMakeLists.txt` root file to find and load the `nginx` package.
   ```
   find_package (nginx REQUIRED)
   ```
   For more information about the `./CMakeLists.txt` root file, see [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_lists_root).
1. Add the `Nginx` program to a list of program executable files defined in the `./einit/CMakeLists.txt` file as follows:
   ```
   set (ENTITIES
        Nginx
        ...)
   ```
   For more information about the `./einit/CMakeLists.txt` file for building the `Einit` initializing program, see the [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_lists_einit).
1. Specify a list of IPC channels that connect the `Nginx` program to `VfsNet` and `VfsRamFs` programs in the `./einit/src/init.yaml.in` template file. For more information about the `init.yaml.in` template file, see the [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_yaml_templates).
1. Create a solution security policy description in the `./einit/src/security.psl.in` template file. For more information about the `security.psl.in` template file, see the [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_psl_templates).
1. Add nginx configuration files to the directory `./resources`.

### Example

You can review the example of developing a solution using nginx web server in KasperskyOS in the [`./kos/example`](kos/example) directory.

[⬆ Back to Top](#Table-of-contents)

## Contributing

Only KasperskyOS-specific changes can be approved. See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed instructions on code contribution.

## License

This project is licensed under the terms of the nginx license. See [LICENSE](LICENSE) for more information.

[⬆ Back to Top](#Table-of-contents)
