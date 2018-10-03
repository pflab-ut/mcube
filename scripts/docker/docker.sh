#!/bin/bash

uid=$(id -u)
gid=$(id -g)
code=$(realpath .)

docker run --rm -it \
       -v ${code}:/home/mcube \
       -e HOME=/home/mcube \
       -u ${uid}:${gid} \
       chishiro/mcube-build /bin/bash -c "cd /home/mcube && make -j$(nproc)"
