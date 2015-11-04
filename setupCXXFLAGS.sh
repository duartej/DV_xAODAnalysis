#!/bin/bash
#
# Pre-configuration in order to compile DVAnalysis package
#
# use: source setupCXXFLAGS.sh
#

PINCLUDE=$(python-config --includes)
PLIBS=$(python-config --libs)
export CXXFLAGS=${CXXFLAGS}" "${PINCLUDE}" "${PLIBS}" -Wno-format"

