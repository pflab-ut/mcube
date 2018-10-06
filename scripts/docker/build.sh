#!/bin/bash

docker build --tag=chishiro/mcube-build .
docker push chishiro/mcube-build
