# Nginx web server for Kaspersky OS

Version of the Nginx web server adapted for use on Kaspersky OS.

For a default build and use, you need to install the Kaspersky OS SDK on your system.
The latest version of the SDK can be downloaded from this [link](https://os.kaspersky.com/development/).

All files required to build a web server with Kaspersky OS and an example of connecting nginx to your solution are located in the folder:

    ./kos

## Building

For build you need to run the script:

    ./kos/nginx/cross-build.sh

## Installation

For easy using nginx in your own solutions on Kaspersky OS, it is recommended to incorporate it in the SDK.
To do this, after the build you will need to run the script with root permissions:

    ./kos/nginx/install.sh

## Deinstallation

To remove all files related to nginx from Kaspersky OS SDK, just run the script with root permissions:

    ./kos/nginx/uinstall.sh

## Using in your own solutions on Kaspersky OS

To connect nginx to your solution, you need to add the nginx package connection in the cmake build script:

    find_package (nginx REQUIRED)

Next, you need to add the Nginx entity to the list of project entities and give it the required permissions to work with disk, network, etc.

After that, in the folder from which the image of the connected disk will be built, you need to add the nginx configuration files, as well as, if necessary, static files that the web server will response to clients' requests.

For further information on how to add nginx to your solution, see the example in the folder:

    ./kos/exmaple
