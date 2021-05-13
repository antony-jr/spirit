### Important: For some reason, the appimage did not work. This can be because the application uses X libraries. So only possible way to use this is to compile from source for now

# Spirit

A C++ program which can attach a gif or any graphic image attach to a X11 application.
**But using with a terminal emulator is recommended.**

![Preview of Spirited Konsole](.github/preview.gif)

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
