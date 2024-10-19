# SPDX-License-Identifier: Apache-2.0
# Based on https://github.com/volkertb/debian-open-watcom
# With additions from chocolate-doom

FROM debian:12.6-slim

ARG OW2_RELEASE_VERSION=2024-07-05-Build
ARG OW2_INSTALLER_SHA256=5a35fa0c23e2a3659fbcf9efdfad17e49f01d6230e7e14141678ccdef8299a62
ARG OW2_INSTALLER_NAME=open-watcom-2_0-c-linux-x64
ARG OW2_DESTINATION_DIR=/opt/watcom

ARG TERM=vt100

ARG USERNAME=dosdev
ARG USER_UID=1000
ARG USER_GID=1001

# [Dasperal] Docker layers optimization: Chain all shell commands in one RUN directive to create only one layer
#
# 1) Install dependencies
#    `ps` is required when using this image for a dev container
# 2) Download and verify Open Watcom v2 installer for Linux
# 3) Run the installer with `script` as a workaround for `Floating point exception (core dumped)`
#    See also https://github.com/open-watcom/open-watcom-v2/wiki/Notes#core-dump-in-linux-installer
# 4) Verify that the installation was indeed successful
#    Verify that the /h (sub)directory with header files was also installed (apparently required `FullInstall=1`)
# 5) Clean up
# 6) Setup user
RUN apt -y update && apt -y upgrade && \
    apt -y install wget procps cmake git && \
    \
    wget -P /tmp https://github.com/open-watcom/open-watcom-v2/releases/download/${OW2_RELEASE_VERSION}/${OW2_INSTALLER_NAME} && \
    echo "${OW2_INSTALLER_SHA256}  /tmp/${OW2_INSTALLER_NAME}" | sha256sum --check && \
    chmod +x /tmp/${OW2_INSTALLER_NAME} && \
    \
    script -c "/tmp/${OW2_INSTALLER_NAME} -i -dDstDir=${OW2_DESTINATION_DIR} -dFullInstall=1" && \
    \
    ls -lh ${OW2_DESTINATION_DIR} && \
    ls -lh ${OW2_DESTINATION_DIR}/h && \
    \
    rm /tmp/${OW2_INSTALLER_NAME} && \
    apt -y autoremove && \
    \
    groupadd --gid ${USER_GID} ${USERNAME} && \
    useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} && \
    chsh ${USERNAME} -s /bin/bash && \
    mkdir -p /home/${USERNAME}/.local/share && \
    chown -R ${USERNAME}:${USERNAME} /home/${USERNAME}/.local

USER $USERNAME

# Setting these ENVs is safer than having an entrypoint script sourcing ${OW2_DESTINATION_DIR}/owsetenv.sh,
# since entrypoints can be bypassed.
# Open Watcom Build Environment
ENV PATH=$OW2_DESTINATION_DIR/binl64:/opt/watcom/binl:$PATH
# [Dasperal] Don't add original INCLUDE to match default behavior of the open-watcom/setup-watcom GitHub Action
ENV INCLUDE=$OW2_DESTINATION_DIR/lh
ENV WATCOM=$OW2_DESTINATION_DIR
ENV EDPATH=$OW2_DESTINATION_DIR/eddat
ENV WIPFC=$OW2_DESTINATION_DIR/wipfc
