---
sidebar_position: 1
---

Spirit is a modern and hackable desktop mascot, it operates very similar
to **MaCoPix** and **Shimeji** but was never invented to replace either of them. Spirit
is made just out of curiosity and as an alternative to both of them since both are 
legacy software.

Spirit is built from the ground up using Qt5 Framework and C++, uses rust for it's
auxilary tools and helper applications. Spirit is also cross-platform and can run in all the major 
platforms with no issues.

## Getting Started

Spirit is very easy to install, grab a fresh copy of Spirit from the 
[downloads](/#downloads) section. We recommend you to install **Stable** version 
of the software on most cases but if you like to try out the latest and greatest of
Spirit, you should use the **Development** version. Although development builds 
might sometimes not work.

### GNU/Linux Instructions

In Linux, Spirit is distributed as an AppImage. You will need [libfuse2](https://packages.ubuntu.com/search?keywords=libfuse2) which is mostly installed by default in 
modern linux distributions, to execute any AppImage.

```bash
 $ chmod +x spirit-*-x86_64.AppImage
 $ ./spirit-*-x86_64.AppImage
```

However you can run the AppImage without libfuse2 using the argument ```--appimage-extract-and-run```, although this is not recommended.

### MacOS Instructions

Open the .dmg file by **"Control + Click"** and then **Open**. This is because the
dmg file and application inside is not code signed, but this is not a problem but
if you are worried make sure to cross check the checksums from the build log and
the one you downloaded from Github.

### Windows Instructions

Open the setup file and just follow the instructions, since the file is not code
signed Windows will warn you, you should force it to keep the file anyway and install
it anyway.

