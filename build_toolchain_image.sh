#!/bin/bash

# Check Docker is installed
if ! which docker > /dev/null 2>&1; then
  echo "Docker is not installed" 1>&2
  exit 1
fi

# Check Docker demon is running
if ! pgrep "[d]ocker" > /dev/null 2>&1; then
  echo "Docker demon is not running" 1>&2
  exit 1
fi

# Check Docker is accessible without root permissions
if !  docker info > /dev/null 2>&1; then
  echo "Docker demon is not accessible without root permissions.
        Add yourself to the 'docker' group and reboot your system" 1>&2
  exit 1
fi

# Build toolchain image
docker build "./.devcontainer" -t toolchain-russian-doom-dos
