# ywm
### YWM is a stacking window manager for X11.  It's UI takes inspiration from Apple's Rhapsody operating system, which grew from NeXTSTEP and would eventually become macOS.

![Build Stauts](https://github.com/tcarrill/ywm/actions/workflows/makefile.yml/badge.svg)

![ywm](img/screenshot.png?raw=true)

## Usage
Right mouse button anywhere but a window will show the root menu.

Double clicking the title bar will shade/unshade the window.

## Root Menu
The root menu is controlled by the ~/.ywm/ywm.menu.  This is a pipe (|) delimited file in the form of "label|command".  The default file is shown below

```
Xterm|xterm
Xclock|xclock
Xlogo|xlogo
Xeyes|xeyes
Xcalc|xcalc
Xman|xman
Exit ywm|killall ywm
```
