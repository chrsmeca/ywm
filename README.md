# YWM
![Build Status](https://github.com/tcarrill/ywm/actions/workflows/makefile.yml/badge.svg)

YWM is a stacking window manager for X11.  It's UI takes inspiration from Apple's Rhapsody operating system, which grew from NeXTSTEP and would eventually become macOS.

![ywm](img/screenshot.png?raw=true)

## Building YWM
The following libraries are required to build YWM\
X11, Xft, Xext, Freetype

They can be installed in a Debian based distribution as follows
```
sudo apt-get update
sudo apt-get install libx11-dev libxft-dev libxext-dev libfreetype6-dev
```
After dependencies are installed, build YWM as follows
```
autoreconf --install
./configure
make
make install
```

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
## Configuration
Configuration is handled by a simple properties file ~/.ywm/ywmrc.  The default file is shown below.  

```
background_color=#666797
menu_title_color=#9b9bbb
window_resistance=30
window_snap_buffer=20
edge_resistance=50
edge_snap_buffer=20
```

To disable window to window snapping or window to screen edge snapping, set the appropriate property to 0, for example:

```
window_snap_buffer=0
```
