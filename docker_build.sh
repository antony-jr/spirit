#!/usr/bin/env bash

mkdir build
sudo docker run -it -v $PWD/build:/out -v $PWD/scripts:/scripts ubuntu:bionic /bin/bash /scripts/build_in_docker.sh

echo "Output stored in $PWD/build/spirit"

