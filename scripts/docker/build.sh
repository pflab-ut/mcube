#!/bin/sh
# @file scripts/docker/build.sh

docker build --tag=chishiro/mcube-build .
docker push chishiro/mcube-build
