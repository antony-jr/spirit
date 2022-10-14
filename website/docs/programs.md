---
sidebar_position: 5
---

# Spirit Addons

Spirits can be controlled using the special local REST API, some addons or programs are made
to use the rest api to denote some useful information from different parts of the 
operating system

## Bash Feedback Addon

Under GNU/Linux when using Bash, you could install this Spirit addon and see your spirit
change to some action you set when the command return code is an error. Might be useful if you
are building some appplication and want to know the status of the build without staring the terminal
much.

### Installation

**Dev Build**

```
  curl -Ls "https://raw.githubusercontent.com/antony-jr/spirit-addons/main/bash_feedback/install-dev.sh" | bash -s
```

**Stable Build**

```
  curl -Ls "https://raw.githubusercontent.com/antony-jr/spirit-addons/main/bash_feedback/install.sh" | bash -s
```

### Uninstallation

You should use this if you want to remove the addon from the system, since the installation would
have updated your bash.rc file.

```
  curl -Ls "https://raw.githubusercontent.com/antony-jr/spirit-addons/main/bash_feedback/uninstall.sh" | bash -s
```

