---
sidebar_position: 1
---

# Introduction

Spirit file is simply a directory, this directory is compressed as an zip archive with
**.spirit** extension by a special tool for Spirit called 
[ritual](https://github.com/antony-jr/ritual). You should download this tool if you 
like to create new spirits, thus you use a **ritual** (as in the tool) to create new 
spirits to use as mascots. (pun intended)

Spirit file can use any valid specification that is given in this documentation, but
currently the one used is the 2022 edition specification. On each new edition of the
specification spirits might get new abilities to use.


## Creating a Spirit

After creating a valid spirit file directory using the available edition, one can use
ritual to generate a spirit file as shown below.

```bash
 $ # Get Ritual for Linux
 $ # Also available for Windows and MacOS
 $ wget "https://github.com/antony-jr/ritual/releases/download/development/ritual-linux-amd64"
 $ chmod +x ritual-linux-amd64
 $ ./ritual-linux-amd64 make spirit-file-directory
 $ # you will now have a file called spirit-file-directory.spirit 
 $ # Load this file in Spirit Manager to see the spirit 
```


