XeEngine
========

A cross-platform game engine that aims to run on low-budget hardware that uses
hardware rendering.

Its main focus is for 2D games or basic 3D graphics, simulating past console
features like palette, tilemap and parallax using line scrolling.

The engine is splitted into XeSDK and XeGame libraries, where the first one
does not contain any game logic but only operating system and hardware
abstraction. XeSDK alone can be used to develop cross-platform applications
too.

The code and the API are designed to be rich, simple, readable and flexible.
That simplicity allows the codebase to be downloaded, compiled and set ready
to use in less than 10 minutes.

While it is not supposed to be production ready, full commercial games can be
created and published with success.

The engine is still on version 0.x, which means that the API compatibility
could break between minor versions.

Requirements and compatibility
==============================

The following platforms are supported:

* Microsoft Windows 7 SP1 or greater
* Universal Windows Platform (UWP, for Microsoft Store)*
* Microsoft Xbox One*
* Sony Playstation Vita*

The following hardware and drivers are supported:

* Little-endian CPU (x86, x64, ARM, MIPS, etc.)

* Direct3D 9.3 video-card from Direct3D 11 compatibility layer or greater

* OpenGL 3.1 or greater

* XInput compatible gamepad

* Native Dualshock 4 support via USB cable

\*A developer license proof is required to have access on the platform's
source code.

Compile guide
=============

## From Windows

Adding the environment path *XESDK_DIR* is strictly required, where its value
is the cloning folder of the engine.

Windows 10 SDK 10.0.17134.0 and Visual Studio 2017 are mandatory, but they are
the versions used to compile the engine when targeting Windows or UWP.

Your first project
=====================

If you copy templates/visualstudio into
*Documents\Visual Studio 2017\Templates\Project Templates* then you will be
able to create pre-configured projects from scratch.

It is possible to view some samples too to learn how to use XeSDK:
[https://github.com/xeengine/samples](https://github.com/xeengine/samples)
