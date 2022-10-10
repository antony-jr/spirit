---
sidebar_position: 4
---

# CLI

The CLI can be used to control Spirits via the command line, this is for the sake of power users
who don't want to leave their terminal just to change your Spirit.

```
Spirit 👻 v1 commit-28f162a (build 103), The Twenty First Century Window Sitter.
Copyright (C) 2021, D. Antony J.R <antonyjr@pm.me>.

Usage: ./spirit-28f162a-x86_64.AppImage <SUBCOMMAND> [OPTIONS]
       ./spirit-28f162a-x86_64.AppImage load <LOCAL SPIRIT FILE> [OPTIONS]
       ./spirit-28f162a-x86_64.AppImage set-action <ACTION NAME> [OPTIONS]
       ./spirit-28f162a-x86_64.AppImage set-property <Property=Value> ... [OPTIONS]
       ./spirit-28f162a-x86_64.AppImage add-quirk name=<ProgramName> yoffset=<int> ... [OPTIONS]


SUBCOMMAND:

 init                   Initialize spirit daemon.
 load                   Load a spirit from local file.
 info                   Show info on current loaded spirit file.
 set-action             Set current action of the loaded spirit.
 list-actions           List all actions of the loaded spirit.
 show-action            Show current action.
 quirks                 Show Quirks list.
 add-quirk              Add a Quirk.
 remove-quirk           Remove a Quirk.
 property               Show current action properties.
 set-property           Set properties for current spirit.
 reset-property         Reset properties to default.
 gh-load                Load a spirit file from github repo.
 deinit                 Quit spirit daemon.
 get-port               Get the port that spirit daemon uses.

OPTIONS [init]:

 -d,--debug     Enable debug messages.
```
