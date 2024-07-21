# Building Russian Doom for DOS with Docker toolchain

## Step 1: Setting up build environment

Install [Docker](https://docs.docker.com/engine/install/).
Make shore the Docker demon is running and docker commands accessible without root permissions.

## Step 2: Compiling project

Run `build_release_package.sh` script.

Or run the following commands:
```shell
docker build "./.devcontainer" -t toolchain-russian-doom-dos
docker run --rm -v ".:/tmp/russian-doom-dos" toolchain-russian-doom-dos \
       /bin/bash -c "cd /tmp/russian-doom-dos && cmake --workflow --preset 'local-watcom-release'"
```

The resulting package and its checksum can be found in the `build` directory.

# Configuring Clion IDE

Download [custom compiler config for Open Watcom](https://github.com/JetBrains/clion-custom-defined-compiler-examples/blob/master/CMake-OpenWatcom2/openwatcom2.yaml)
and select it in Clion's **Settings > Build, Execution, Deployment > Toolchains > Custom Compiler**

## Dev-container

Install `docker-buildx-plugin`.

Build and run dev-container

## Docker toolchain

### Step 1: Building image

Run `build_toolchain_image.sh` script.

Or run the following command:
```shell
docker build "./.devcontainer" -t toolchain-russian-doom-dos
```

### Step 2: Setting up toolchain

In Clion's **Settings > Build, Execution, Deployment > Toolchains** 
add `Docker toolchain` and select `toolchain-russian-doom-dos` image.
