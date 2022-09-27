#!/usr/bin/env bash

cp -rp /usr/local/lib/x86_64-linux-gnu/plugins/kf5 appdir/usr/plugins/
cd appdir/usr/plugins/kf5/kwindowsystem
patchelf --set-rpath "\$ORIGIN/../../../lib" KF5WindowSystemWaylandPlugin.so
patchelf --set-rpath "\$ORIGIN/../../../lib" KF5WindowSystemX11Plugin.so


