---
sidebar_position: 2
---

# Edition 2021 Specification

This document specifies the 2021 edition specification of the "Spirit File". The following
is the directory structure to be followed when creating an 2021 edition Spirit File 
directory.


```bash
-- <Spirit File Directory Name>
 |
 |-- actions
 | |
 | |-- default.webp # (REQUIRED) 
 | |-- _default.webp # (OPTIONAL)
 | 
 |-- audio # (OPTIONAL)
 | |
 | |-- track01.mp3 # (OPTIONAL)
 | -
 |
 |-- meta.json # (REQUIRED)
 -
```

The "Spirit File Directory Name" can be any valid directory name, but it is recommended
to use some meaningful name, for example if this is a mascot for Inori from Guilty Crown.
Then it might be good to name the directory ```Inori```.

## Actions Directory (actions/)

This directory holds all the animated webp files for each action of the spirit, a default
action is **required** to make the spirit valid. You can use traditional 2D animation
software to make different actions for your mascot. Webp files can also have specific
delays between frames and lot more customization. But this webp file should have a
transparent background.

**The actions/ directory should contain atleast the default.webp file which will be 
used to render the spirit when it is loaded by the user.**

A file with underscore before it's name corresponds to the bottom version of the 
animation of some action. For example _default.webp is used when spirit is placed at
the bottom of the window. This allows the spirit file maker to use different animation
file when the mascot is at the bottom, like creating a floor to sit on or chair or some
extra animation.

Only webp files are supported.

## Audio Directory (audio/)

This directory is **optional** and can be avoided, this directory is used to store all
your mp3 audio files in case you need to play some tune or music for specific actions
of your spirit.

Might be useful to create amazing animations with music. Like a music video animated
with the mascot playing a music.

## Meta JSON (meta.json)

This is the main file which describes everything about the spirit like copyright
information and how actions are defined, and positions and offsets. Example of a 
meta.json file is shown below.


```js
{
   "name": "Default", // Name of the Spirit
   "edition": "2021", // Spirit File Directory Specification
   "version": "v0.1.0", // Version of this Spirit File
   "author": "Antony J.R", // Author of this Spirit File
   "copyright": "Copyright (C) Giphy", // Copyright of Artwork used.
   "actions": {
           "default": { // Corresponds to actions in actions/ directory
              "yoff": 40, // Y offset to offset the mascot when sitting on top of window
              "scale": 40 // Scale of the mascot in percentage (here 40%)
           }
   }
}
```


### name

Specifies the fancy name of this Spirit, something like **"Naruto Uzumaki"** in case
the spirit displays a mascot of Naruto. This value can be anything will be displayed
in the Spirit application when the user opens it. **This field is required.**

### edition

Specifies the edition which you followed to create the "Spirit File Directory", 
[ritual](https://github.com/antony-jr/ritual) will use this information to check and
validate your directory. Currently the possible value is ```2021``` only. **This field
is required.**

### version

Specifies the version of this spirit file, this can be any string, but we recommend to
use semver. **This field is required.**

### author

Specifies the author of this spirit file, not any copyright owners of the artwork, only
the author of this spirit file. **This filed is required**.

### copyright

Copyright string to display to the users which should include attribution to the author
of the artwork used by this Spirit file or any other license information. If in public
domain you may relicense. **This filed is required**.

### actions

This object specifies a list of json objects which corresponds to each file in the 
actions directory. The json object will define specific options for the action like 
y-axis offset, x-axis offset, scale, speed or next action to go after this action
or loop. **The default action is by default looped.**

Each action should have the **file name of the webp file placed in actions/ directory
as it's key to correspond.** Since default.wep is required, the actions json object
should have a default json object to specify the options for the default animation.

Each object under action can have the following options,
```js
"actions" : {
  "default" : { // REQUIRED
      "xoff": 0, // INTEGER    | used when spirit is on top of window
      "yoff": 0, // INTEGER    | used when spirit is on top of window
      "_xoff": 0, // INTEGER   | used when spirit is on bottom of window
      "_yoff": 0, // INTEGER   | used when spirit is on bottom of window
      "loop": true, // BOOLEAN | loop the action (true by default for default action)
      "scale": 100, // INTEGER | defines the percentage of scale (here, 100%)
      "speed": 100, // INTEGER | defines the percentage of speed (here, 100%)
      "next": "", // STRING | when loop is false, show this action next
      "play": "track0", // STRING | mp3 file to play when showing this action (OPTIONAL)
      "delay": 1000, // INTEGER | delay in miliseconds when moving to next action  
  }
}
```

**All options shown above are totally optional but you should give a empty object
with the key corresponding to the filename of the webp file in actions directory.**


## Examples 

You can look at examples at [ritual](https://github.com/antony-jr/ritual) under sample/ 
directory in the root of the repo.
