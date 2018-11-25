#!/bin/bash


if [ $# -ne 1 ]; then
	echo "Need one argument."
	echo "Usage:   $0 <tuplet>"
	echo "Example: $0 arm-unknown-linux-"
	echo
	exit -1
fi
GCC=''
GXX=''

if which ${1}gcc > /dev/null 2>&1; then
	GCC=`which ${1}gcc`
	echo "GCC compiler found at ${GCC}"
else
	echo "GCC compiler '${1}gcc not found."
	echo "Please check the tuplet and make sure that the toolchain is in the"
	echo "PATH envorinment variable."
	echo
	exit -1
fi

if which ${1}g++ > /dev/null 2>&1; then
	GXX=`which ${1}g++`
	echo "G++ compiler found at ${GXX}"
else
	echo "G++ compiler '${1}g++ not found."
	echo "Please check the tuplet and make sure that the toolchain is in the"
	echo "PATH envorinment variable."
	echo
	exit -1
fi


GENFILE="./toolchain.cmake"

echo "Creating ${GENFILE} ..."

cat > ${GENFILE} <<EOF;
# compiler.inc
# This file was automatically generated at `date`
# Use ${0} to regenerate it if needed.

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_COMPILER ${GCC})
SET(CMAKE_CXX_COMPILER ${GXX})
SET(CMAKE_TOOLCHAIN_PREFIX ${1})
EOF

if [ -d target_sysroot ]; then
    echo "Using target_sysroot as CMAKE_SYSROOT"
    echo "SET(CMAKE_SYSROOT $(pwd)/target_sysroot)" >> ${GENFILE}
elif [ ! -z "${TARGET_SYSROOT}" ]; then
    echo "Using variable TARGET_SYSROOT as CMAKE_SYSROOT: ${TARGET_SYSROOT}"
    echo "SET(CMAKE_SYSROOT ${TARGET_SYSROOT}" >> ${GENFILE}
else
    echo "target_sysroot was not found"
    echo "variable TARGET_SYSROOT is not set as well."
    echo "YOU MAY EXPERIENCE BUILD FAILURES IN CASE YOUR TOOLCHAIN LACKS THE"
    echo "NEEDED LIBRARIES."
    echo "PROJECT WILL BE CONFIGURED BUT YOUR MILEAGE MAY VARY"
fi
    
    
cat >> ${GENFILE} <<EOF;
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF

echo "All set now. The project should be ready for build."
