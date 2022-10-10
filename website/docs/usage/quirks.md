---
sidebar_position: 3
---

# Window Quirks

Window Quirks are fix up for exceptional applications in the operating system, for example, under 
Ubuntu, all applications which uses GTK will have a drop shadow which is added in the frame extents
and thus Spirit might not have a good placement, Qt application does not have this issue.

So you can set a global quirk for your OS which will count for the window decoration and will be 
applied automatically to all spirits loaded. If some program you use are Qt or does not have decoration
you can add another Quirk especially for this process.

**So far only Ubuntu and GNOME based DEs have issues, Qt Applications and KDE Plasma seems to be fine,
while Windows and MacOS ironically are perfect in regards to getting active window dimensions
accurately.**

To add or remove Quirks, Go to **File -> Quirks**.

![Quirks Window](/img/quirks.png)


Visible Name is **OPTIONAL** but Program is **REQUIRED**. The program should be the name of the process.
For example, if it's GNOME Terminal then this should be ```gnome-terminal-server``` or something. By
default you will be editing the global quirks which will be added to all application regardless of the
list.

You can use ```xprop``` tool to get the process name by verifying the window class string which is used
by Spirit.

