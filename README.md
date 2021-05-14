# Spirit

A C++ program which can attach a gif or any graphic image attach to a X11 application.
**But using with a terminal emulator is recommended.**

This program aims to *work with any X11 based desktop environment not just KDE*, for now there are some bugs with 
gnome but there are plans for fix this as soon as possible.


**IMPORTANT NOTE**: The pre-built binaries (AppImages) only supports gif and png files to be rendered. So please don't
use webp or other formats with pre-built binaries, this will result in spirit not rendering the image.


See the demo at https://www.youtube.com/watch?v=KC_MwRyo0js


![Preview of Spirited Konsole](.github/preview.gif)


# Get AppImages 

**Spirit** can be used as an AppImage, please don't integrate this into your system. You can put it in your local bin 
path and edit your terminal desktop file to invoke it with the spirit command.

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
 $ ./spirit ":/default.webp" konsole # Test it out.
```

# Usage 


```
Spirit v0.1.0, Attach gif/webp over any X11 window in a stylish way.
D. Antony J.R <antonyjr@pm.me>

Usage: ./spirit [GIF/WEBP FILE] [PROGRAM TO EXEC]

Example:
        ./spirit ":/default.webp" konsole
```


# License

Licensed under GPL v3.
