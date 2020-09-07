#!/bin/bash

# ------------------------------------------------------------
# This file is a Dockerfile of memtool of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

# This bash script builds all Docker builder containers at once
# Usage:
#   create-containers.sh [PREFIX]
#
# The created container images will be labeled PREFIX:PLATFORM.
# * If no PREFIX is submitted, then "headcode-memtool" will be used.
# * The PLATFORM is the suffix of the Dockerfile name found.

cd $(dirname $(readlink -f ${0}))

PREFIX="${1}"
if [[ -z ${PREFIX} ]]; then
    PREFIX="headcode-memtool"
fi
PLATFORMS=$(ls -1 Dockerfile.build.* | cut -d '.' -f 3 | xargs echo)

for PLATFORM in ${PLATFORMS}; do
    BUILDER_IMAGE="${PREFIX}:${PLATFORM}"
    echo "Creating ${BUILDER_IMAGE}..."
    docker build --tag ${BUILDER_IMAGE} --file Dockerfile.build.${PLATFORM} .
done
