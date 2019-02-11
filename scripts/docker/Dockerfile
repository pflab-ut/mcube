# Mcube Kernel Build Tools

FROM ubuntu:18.04
LABEL Hiroyuki Chishiro

ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/home/mcube
RUN set -x \
    && apt-get update \
    && apt-get install -y \
               ccache gcc clang clang-tools llvm gcc-aarch64-linux-gnu cppcheck \
               git samba make doxygen sendmail git \
               qemu qemu-system \
               python3 python3-pip \
    && pip3 install pyflakes pylint \
    && mkdir -p $HOME

# for LaTeX
#               tetex-* texlive-latex* texlive-science ptex-jtex \
#               texlive-lang-japanese texlive-fonts-recommended \
#               dvi2ps-fontdesc-morisawa5 \

VOLUME $HOME

CMD ["/bin/bash"]
