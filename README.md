<p align="center">
  <img src="artwork/spirit_logo.png" height="200px" width=auto alt="Spirit Logo">  <br>
</p>


# Spirit [![GitHub issues](https://img.shields.io/github/issues/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/issues) [![GitHub forks](https://img.shields.io/github/forks/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/network) [![GitHub stars](https://img.shields.io/github/stars/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/stargazers) [![GitHub license](https://img.shields.io/github/license/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/blob/master/LICENSE) [![Deploy](https://github.com/antony-jr/spirit/actions/workflows/deploy.yml/badge.svg)](https://github.com/antony-jr/spirit/actions/workflows/deploy.yml)


See the demo at https://www.youtube.com/watch?v=KC_MwRyo0js

# Quick Start

```
 $ wget "https://github.com/antony-jr/spirit/releases/download/v0.1.2/spirit" 
 $ chmod +x spirit
 $ ./spirit init 
 $ # Now only the focused terminal will have the overlay.
 $ ./spirit deinit # To kill all instance of spirit # This is important
```

# Installation / Update

```
 $ curl -Ls "https://git.io/JsgXK" | bash -s
```


# Get AppImages 

**Spirit** can be used as an AppImage, please don't integrate this into your system. You can put it in your local bin 
path and edit your terminal desktop file to invoke it with the spirit command.

### Development Builds

The build of the master branch into an AppImage are the development builds. On each push these binaries are updated.
Use this to get the latest and the greatest of spirit.


```
 $ wget "https://github.com/antony-jr/spirit/releases/download/development/spirit"
 $ chmod +x spirit
 $ ./spirit # Use it now, or move it to /usr/bin/ to install
```

### Versioned Releases

Somewhat considered as stable. These builds are usually older than the development builds. Bug fixes will happen a bit slower in these builds.


```
 $ wget "https://github.com/antony-jr/spirit/releases/download/v0.1.2/spirit"
 $ chmod +x spirit
 $ ./spirit # Use it now, or move it to /usr/bin/ to install
```


# Building from source

**Dependencies:** CivetWeb, QArchive 

```
 $ git clone https://github.com/antony-jr/spirit 
 $ cd spirit
 $ cmake .
 $ make -j$(nproc)
```

# License

Licensed under GPL v3.
