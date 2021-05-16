#!/usr/bin/env bash

apt-get update -qq
apt-get upgrade -y

apt-get -y install build-essential libgl1-mesa-dev xvfb \
               wget fuse libxdo-dev git

cd ~
git clone https://github.com/antony-jr/spirit
cd spirit

if [ ! -d "/usr/local/Qt-5.12.8" ]; then
    apt-get -y install gcc g++ make wget
    apt-get -y install build-essential
    apt-get -y install "^libxcb.*" libx11-dev libx11-xcb-dev libxcursor-dev libxrender-dev libxrandr-dev libxext-dev libxi-dev libxss-dev libxt-dev libxv-dev libxxf86vm-dev libxinerama-dev libxkbcommon-dev libfontconfig1-dev libharfbuzz-dev libasound2-dev libpulse-dev libdbus-1-dev udev mtdev-tools webp libudev-dev libglm-dev libwayland-dev libegl1-mesa-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libgles2-mesa libgles2-mesa-dev libmirclient-dev libproxy-dev libgtk2.0-dev libgtk-3-dev libcups2-dev libssl-dev openssl
    wget -c "https://download.qt.io/archive/qt/5.12/5.12.8/submodules/qtbase-everywhere-src-5.12.8.tar.xz"
    tar -xf qtbase-everywhere-src-5.12.8.tar.xz
    cd qtbase-everywhere-src-5.12.8
    ./configure -static -release -optimize-size -silent -opensource -confirm-license -opengl -openssl-linked -nomake examples -qt-xcb -sm -qt-libpng -no-libjpeg -no-icu -qt-zlib -qt-pcre -gtk -system-freetype -qt-harfbuzz
    make -j$(nproc)
    make install

    # Clean up
    cd ..
    rm -rf qtbase-everywhere-src-5.12.8
    rm -rf qtbase-everywhere-src-5.12.8.tar.xz
fi

echo "Installed Qt Static Version Successfully!"

export VERSION="dev"
/usr/local/Qt-5.12.8/bin/qmake .
make -j$(nproc)
mkdir -p appdir/usr/lib 
mkdir -p appdir/usr/bin ; 
strip spirit ; cp spirit appdir/usr/bin/
mkdir -p appdir/usr/share/applications ; 
cp build_resources/spirit.desktop appdir/usr/share/applications/
mkdir -p appdir/usr/share/icons/hicolor/256x256/apps/ ; 
cp build_resources/spirit.png appdir/usr/share/icons/hicolor/256x256/apps/spirit.png
wget "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"

chmod +x linuxdeploy-x86_64.AppImage
./linuxdeploy-x86_64.AppImage --appimage-extract-and-run --appdir appdir --output appimage
sed '0,/AI\x02/{s|AI\x02|\x00\x00\x00|}' -i spirit-$VERSION-x86_64.AppImage
mv spirit-$VERSION-x86_64.AppImage spirit

# Remove development appimages.
rm -rf linuxdeploy*

mkdir -p /out/

cp spirit /out/
