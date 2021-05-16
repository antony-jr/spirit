# Spirit

A C++ program which can attach a gif or any graphic image attach to a X11 application.
**But using with a terminal emulator is recommended.**

This program aims to *work with any X11 based desktop environment not just KDE*, for now there are some bugs with 
gnome but there are plans for fix this as soon as possible.


**IMPORTANT NOTE**: The pre-built binaries (AppImages) only supports gif and png files to be rendered. So please don't
use webp or other formats with pre-built binaries, this will result in spirit not rendering the image.


See the demo at https://www.youtube.com/watch?v=KC_MwRyo0js


![Preview of Spirited Konsole](.github/preview.gif)


# Quick Start

```
 $ wget "https://github.com/antony-jr/spirit/releases/download/development/spirit"
 $ chmod +x spirit
 $ ./spirit konsole
 $ ./spirit -g path/to/your/custom.gif konsole # for your custom transparent gif
```

# Get AppImages 

**Spirit** can be used as an AppImage, please don't integrate this into your system. You can put it in your local bin 
path and edit your terminal desktop file to invoke it with the spirit command.

```
 $ wget "https://github.com/antony-jr/spirit/releases/download/development/spirit"
 $ chmod +x spirit
 $ ./spirit # Use it now, or move it to /usr/bin/ to install
```

### Development Builds

The build of the master branch into an AppImage are the development builds. On each push these binaries are updated.
Use this to get the latest and the greatest of spirit.

### Versioned Releases

Somewhat considered as stable. These builds are usually older than the development builds. Bug fixes will happen a bit slower in these builds.


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
