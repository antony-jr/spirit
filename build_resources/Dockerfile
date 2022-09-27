FROM ubuntu:18.04
WORKDIR /environment

RUN apt-get update -qq && apt-get upgrade -y
RUN apt-get -y install apt-utils gpg wget software-properties-common
RUN add-apt-repository ppa:beineri/opt-qt-5.15.2-bionic -y
RUN apt-get update -qq
RUN apt-get -y install build-essential libgl1-mesa-dev xvfb qt515base \
               qt515multimedia qt515x11extras qt515tools qt515imageformats \
               qt515wayland automake libtool \
               libcairo-dev libfuse-dev zsync desktop-file-utils \
               ca-certificates autoconf libtool patchelf patch wget \
               vim-common desktop-file-utils pkg-config \
               librsvg2-dev librsvg2-bin liblzma-dev git vim
               # libxrender-dev libxcb-icccm4-dev \
               # libxcb-keysyms1-dev libxcb-res0-dev git

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update -qq
RUN rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install kitware-archive-keyring
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic-rc main' | tee -a /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update -qq
RUN apt-get -y install cmake

RUN git clone https://github.com/libarchive/libarchive
WORKDIR /environment/libarchive
RUN cmake .
RUN make -j$(nproc)
RUN make install -j$(nproc)
WORKDIR /environment
RUN rm -rf libarchive
RUN cp -fp /usr/local/lib/pkgconfig/libarchive.pc /usr/lib/pkgconfig/libarchive.pc
RUN cp -fp /usr/local/lib/libarchive.* /usr/lib/

RUN wget "https://github.com/civetweb/civetweb/archive/refs/tags/v1.15.tar.gz"
RUN tar -xvf v1.15.tar.gz
WORKDIR /environment/civetweb-1.15
RUN mkdir libbuild
WORKDIR /environment/civetweb-1.15/libbuild
RUN cmake -DCIVETWEB_ENABLE_CXX=ON -DCIVETWEB_DISABLE_CGI=ON -DCIVETWEB_SERVE_NO_FILES=ON -DCIVETWEB_ENABLE_SERVER_EXECUTABLE=OFF -DCIVETWEB_INSTALL_EXECUTABLE=OFF -DCIVETWEB_BUILD_TESTING=OFF ..
RUN make -j$(nproc)
RUN make install
WORKDIR /environment
RUN rm -rf v1.15.tar.gz civetweb-1.15

RUN source /opt/qt*/bin/qt*-env.sh || true
RUN git clone https://github.com/antony-jr/QArchive
WORKDIR /environment/QArchive
RUN git checkout 2d05e652ad9a2bff8c87962d5525e2b3c4d7351b 
RUN cmake .
RUN make -j$(nproc)
RUN make install
WORKDIR /environment

CMD ["bash"]
