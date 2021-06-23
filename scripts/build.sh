#!/bin/bash

WORKING_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
BUILD_DIR=${WORKING_DIR}/../build

clean()
{
        build_files=( CMakeCache.txt cmake_install.cmake Makefile )
        build_dirs=( bin CMakeFiles )
        for var in "${build_files[@]}"
        do
                path="${BUILD_DIR}/${var}"
                if [[ -f ${path} ]]; then
                        rm ${path}
                fi
        done 

        for var in "${build_dirs[@]}"
        do
                path="${BUILD_DIR}/${var}"
                if [[ -d ${path} ]]; then
                        rm -r ${path}
                fi

        done 
}

build_debug()
{
        cd ${BUILD_DIR}
        cmake -DCMAKE_BUILD_TYPE=Debug ..
        make
}

build_release()
{
        cd ${BUILD_DIR}
        cmake -DCMAKE_BUILD_TYPE=Release ..
        make
}

clean
build_debug
