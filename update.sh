#!/usr/bin/env bash
VERSION="v0.2.1"
echo -e "\e[1;34mSpirit $VERSION\e[0m, Desktop Mascot of Twenty First Century."
echo -e "Copyright (C) D. Antony J R, GNU General Public License v3."
echo ""

mkdir -p ~/Spirit

echo "Checking for AppImage Updater."
if [ -f ~/Spirit/.appimageupdater ]; then
    echo "Found AppImage Updater."
else
    echo "Downloading AppImage Updater..."
    echo -n -e "\e[1;32m"
    wget -q --show-progress --progress=bar:force -O ~/Spirit/.appimageupdater "https://github.com/antony-jr/AppImageUpdater/releases/download/v2.0.0/appimageupdatercli-v2.0.0-x86_64.AppImage" 2>&1
    echo -e "\e[0m"
fi

chmod a+x ~/Spirit/.appimageupdater
cd ~/Spirit

echo ""
echo "Updating Spirit..."
$(pwd)/.appimageupdater -t -D spirit

if [ -f *.AppImage ]; then
   mv *.AppImage spirit
else
   echo -e "\e[42mYou already have latest Spirit Stable ($VERSION).\e[0m"
   exit
fi

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
echo -e "\e[42mUpdated to Spirit Stable ($VERSION).\e[0m"
