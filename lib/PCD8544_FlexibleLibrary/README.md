# PCD8544_FlexibleLibrary
Simple and flexible cross-platform library for PCD8544 LCD controllers.

Main features:
  - Using a framebuffer is optional;
  - Support for multiple languages, (ASCII and Russian out of box, others can be added);
  - Font scaling: default font 5x7 can be scaled 2 and 3 times;
  - Image scaling: images can be scaled up to 6 times;
  - Text frames: the area where text is printed can be limited by a "virtual frame";
  - "Smart" display content update when using framebuffer;
  - Fast vertical and horizontal lines drawing;
  - An ability to clear part of display (not only the whole display);

The image scaling feature is very useful when printing a simple logo across the entire display:
if the logo fits into 14x8 pixels, then it can be scaled 6 times to 84x48, which saves 36 times memory.

> Library code is C99 compliant taking into account the limitations of the SDCC compiler( - the only free C comliper for STM8 available on Linux).

### Usage

To use the library in your project, just copy the source files located in its
root directory to your project, configure the build system paths and edit
*_conf.h, *_port.h and optionally *_font.h.

> Only a single PCD8544 physical instance is supported.
