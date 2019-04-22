# Mcube Kernel Build Tools

FROM ubuntu:18.04
LABEL Hiroyuki Chishiro

ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/home/mcube
RUN set -x \
    && apt-get update \
    && apt-get install -y \
               ccache gcc clang clang-tools llvm gcc-aarch64-linux-gnu \
               astyle cppcheck flawfinder \
               git samba make doxygen sendmail git \
               qemu qemu-system \
               python3 python3-pip \
    && pip3 install pyflakes pylint \
    && mkdir -p $HOME

VOLUME $HOME

CMD ["/bin/bash"]
