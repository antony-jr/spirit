#!/usr/bin/env bash
VERSION="v0.2.1"
CWD=$(pwd)
echo -e "\e[1;34mSpirit $VERSION\e[0m, Desktop Mascot of Twenty First Century."
echo -e "Copyright (C) D. Antony J R, GNU General Public License v3."
echo ""

mkdir -p ~/Spirit
rm -rf ~/Spirit/spirit
echo -e "Downloading..."
echo -n -e "\e[1;32m"
wget -q --show-progress --progress=bar:force -O ~/Spirit/spirit https://github.com/antony-jr/spirit/releases/download/$VERSION/spirit-$VERSION-x86_64.AppImage 2>&1
echo -e "\e[0m"

chmod a+x ~/Spirit/spirit
sed -i '/export PATH=$PATH:.*.\/Spirit # Spirit AppImage/d' ~/.bashrc
echo "export PATH=\$PATH:$HOME/Spirit # Spirit AppImage" >> ~/.bashrc

cd ~/Spirit
mkdir -p .icons 
rm -rf squashfs-root

./spirit --appimage-extract spirit.desktop > /dev/null
./spirit --appimage-extract spirit.png > /dev/null

mv squashfs-root/spirit.png .icons/spirit.png

sed -i 's+Name=spirit+Name=Spirit Manager '$VERSION'+g' squashfs-root/spirit.desktop
sed -i 's+Exec=spirit+Exec='$HOME'\/Spirit\/spirit+g' squashfs-root/spirit.desktop
sed -i 's+Icon=spirit+Icon='$HOME'\/Spirit\/.icons\/spirit.png+g' squashfs-root/spirit.desktop

cp -f squashfs-root/spirit.desktop ~/Desktop
cp -f squashfs-root/spirit.desktop ~/.local/share/applications/spirit.desktop
rm -rf squashfs-root

echo -e "\e[1;34mStart Spirit from your Application Menu or Terminal with 'spirit'.\e[0m"
echo -e "\e[42mInstalled Spirit Stable ($VERSION).\e[0m"

cd $CWD
bash
