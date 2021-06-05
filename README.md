<p align="center">
  <img src="artwork/spirit_logo.png" height="200px" width=auto alt="Spirit Logo">  <br>
</p>


# Spirit [![GitHub issues](https://img.shields.io/github/issues/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/issues) [![GitHub forks](https://img.shields.io/github/forks/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/network) [![GitHub stars](https://img.shields.io/github/stars/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/stargazers) [![GitHub license](https://img.shields.io/github/license/antony-jr/spirit.svg?style=flat-square)](https://github.com/antony-jr/spirit/blob/master/LICENSE) [![Deploy](https://github.com/antony-jr/spirit/actions/workflows/deploy.yml/badge.svg)](https://github.com/antony-jr/spirit/actions/workflows/deploy.yml)


Modern Window Sitter for X11 based Desktop Environments (Coming to Wayland, Windows and Mac soon-ish).
**But using with a terminal emulator is recommended.**

**This application is supported on all major desktop environments based on X11.**

**IMPORTANT NOTE**: The pre-built binaries (AppImages) only supports gif and png files to be rendered. So please don't
use webp or other formats with pre-built binaries, this will result in spirit not rendering the image.


**NOTE FOR CINNAMON USERS:** Please add this argument to your init, ``` -y 80 ```.


**NOTE FOR ELEMENTARY OS USERS:** Please execute this command(```$ echo "export QT_STYLE_OVERRIDE=Fusion" >> ~/.bashrc```) once before doing anything. You only have to do this once.



See the demo at https://www.youtube.com/watch?v=KC_MwRyo0js


![Preview of Spirited Konsole](.github/preview.gif)


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

```
 $ sudo apt-get install libxdo-dev # deps, also you need qtbase for sure.
 $ git clone https://github.com/antony-jr/spirit 
 $ cd spirit
 $ cmake .
 $ make -j$(nproc)
 $ ./spirit konsole # Test it out.
```

# License

Licensed under GPL v3.
