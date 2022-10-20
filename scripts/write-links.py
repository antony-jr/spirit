#!/usr/bin/env python3
import os
import re
import requests

header = {
        "Accept": "application/vnd.github+json",
}

if "GITHUB_TOKEN" in os.environ.keys():
    header["Authorization"] = "Bearer {}".format(os.environ["GITHUB_TOKEN"])

fp = open("links.sh", "w")

# Get Latest Release Asset Links
response = requests.get(
        "https://api.github.com/repos/antony-jr/spirit/releases/latest",
        headers=header)

latest_json = response.json()
latest_assets = latest_json["assets"]

for asset in latest_assets:
    url = asset["browser_download_url"]

    if url.endswith(".AppImage"):
        # Export AppImage Release Link
        fp.write("export LINUX_STABLE_LINK=\"{}\"\n".format(url))
    elif url.endswith(".dmg"):
        # Export Mac Release Link
        fp.write("export MAC_STABLE_LINK=\"{}\"\n".format(url))
    elif url.endswith(".exe"):
        # Export Windows Release Link
        fp.write("export WIN_STABLE_LINK=\"{}\"\n".format(url))

# Get Latest Development Asset Links
response = requests.get(
        "https://api.github.com/repos/antony-jr/spirit/releases/tags/development",
        headers=header)

latest_json = response.json()
latest_assets = latest_json["assets"]

for asset in latest_assets:
    url = asset["browser_download_url"]

    if url.endswith(".AppImage"):
        fp.write("export LINUX_DEV_LINK=\"{}\"\n".format(url))
    elif url.endswith(".dmg"):
        fp.write("export MAC_DEV_LINK=\"{}\"\n".format(url))
    elif url.endswith(".exe"):
        fp.write("export WIN_DEV_LINK=\"{}\"\n".format(url))



fp.close()
