#!/bin/bash

# Build toolchain image
bash ./build_toolchain_image.sh

# Execute cmake in workflow mode
docker run --rm -v ".:/tmp/russian-doom-dos" toolchain-russian-doom-dos \
       /bin/bash -c "cd /tmp/russian-doom-dos && cmake --workflow --preset 'local-watcom-release'"
