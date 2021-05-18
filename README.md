# Spirit

A C++ program which can attach a gif or any graphic image attach to a X11 application.
**But using with a terminal emulator is recommended.**


**SUPPORTS KDE,GNOME AND PANTHEON(ELEMENTARY OS). BUT PLEASE NOTE ONLY X11 Desktop Environments are supported, no Wayland support.**


**IMPORTANT NOTE**: The pre-built binaries (AppImages) only supports gif and png files to be rendered. So please don't
use webp or other formats with pre-built binaries, this will result in spirit not rendering the image.


See the demo at https://www.youtube.com/watch?v=KC_MwRyo0js


![Preview of Spirited Konsole](.github/preview.gif)


# Quick Start

```
 $ wget "https://github.com/antony-jr/spirit/releases/download/v0.1.1/spirit" 
 $ chmod +x spirit
 $ ./spirit init 
 $ # Now only the focused terminal will have the overlay.
 $ ./spirit deinit # To kill all instance of spirit # This is important
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
 $ wget "https://github.com/antony-jr/spirit/releases/download/v0.1.1/spirit"
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
