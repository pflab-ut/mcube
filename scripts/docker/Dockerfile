# Mcube Kernel Build Tools

FROM ubuntu:18.04
LABEL Hiroyuki Chishiro

ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/home/mcube
RUN set -x \
    && apt-get update \
    && apt-get install -y \
               ccache gcc clang llvm gcc-aarch64-linux-gnu nasm \
               python3 python3-pip \
               git samba make doxygen sendmail git genisoimage \
               dvi2ps-fontdesc-morisawa5 \
               qemu qemu-system \
    && pip3 install pyflakes pylint \
    && mkdir -p $HOME

#               tetex-* texlive-latex* texlive-science ptex-jtex \
#               texlive-lang-japanese texlive-fonts-recommended \

VOLUME $HOME

CMD ["/bin/bash"]
