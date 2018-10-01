#ifndef YWM_H
#define YWM_H

Display *dpy;
Window root;

enum AtomsWM {
  AtomWMDeleteWindow,
  AtomWMProtocols,
  NumberOfAtoms
};

Atom atom_wm[NumberOfAtoms];

void frame(Display *dpy, Window root, Window win);
#endif
