#!/usr/bin/env bash

set -x

sudo wget -O /usr/bin/spirit "https://github.com/antony-jr/spirit/releases/download/v0.1.2/spirit"
sudo chmod +x /usr/bin/spirit

set +x

if [ -f /usr/bin/spirit ];
	then
		echo "Installed Spirit v0.1.2!"
	else
		echo "Installation Failed!"
fi
