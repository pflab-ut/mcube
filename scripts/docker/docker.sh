#!/bin/bash

args=($@)
#echo "args = " ${args[@]}

# remove the first argument "docker".
if [ $# -gt 1 ] ; then
    args=(${args[@]:1})
#    echo "args = " ${args[@]}
fi

uid=$(id -u)
gid=$(id -g)
code=$(realpath .)

docker run --rm -it \
       -v ${code}:/home/mcube \
       -e HOME=/home/mcube \
       -u ${uid}:${gid} \
       chishiro/mcube-build /bin/bash -c "cd /home/mcube && make ${args} -j$(nproc)"
