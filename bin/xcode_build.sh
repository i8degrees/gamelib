#!/bin/bash

# NOTE: This script is intended to be ran from the project's current build
# directory.

XCODEBUILD_BIN=$(which xcodebuild)
BUILD_TYPE_ARG=$1

# Default
BUILD_TYPE="Debug"

if [[ !( -z "$BUILD_TYPE_ARG") ]]; then
  BUILD_TYPE=$1
fi

echo "Build configuration: ${BUILD_TYPE}..."

${XCODEBUILD_BIN} -configuration ${BUILD_TYPE} -target ALL_BUILD
