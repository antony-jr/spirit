if [ ! -d "/usr/local/Qt-5.12.8" ]; then
    sudo apt-get -y install gcc g++ make wget
    sudo apt-get -y install build-essential
    sudo apt-get -y install "^libxcb.*" libx11-dev libx11-xcb-dev libxcursor-dev libxrender-dev libxrandr-dev libxext-dev libxi-dev libxss-dev libxt-dev libxv-dev libxxf86vm-dev libxinerama-dev libxkbcommon-dev libfontconfig1-dev libharfbuzz-dev libasound2-dev libpulse-dev libdbus-1-dev udev mtdev-tools webp libudev-dev libglm-dev libwayland-dev libegl1-mesa-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libgles2-mesa libgles2-mesa-dev libmirclient-dev libproxy-dev libgtk2.0-dev libgtk-3-dev libcups2-dev libssl-dev openssl
    wget -c "https://download.qt.io/archive/qt/5.12/5.12.8/submodules/qtbase-everywhere-src-5.12.8.tar.xz"
    tar -xf qtbase-everywhere-src-5.12.8.tar.xz
    cd qtbase-everywhere-src-5.12.8
    ./configure -static -release -optimize-size -silent -opensource -confirm-license -opengl -openssl-linked -nomake examples -qt-xcb -sm -qt-libpng -no-libjpeg -no-icu -qt-zlib -qt-pcre -gtk -system-freetype -qt-harfbuzz
    make -j$(nproc)
    sudo make install

    # Clean up
    cd ..
    rm -rf qtbase-everywhere-src-5.12.8
    rm -rf qtbase-everywhere-src-5.12.8.tar.xz
fi

echo "Installed Qt Static Version Successfully!"
