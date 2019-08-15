Additionally required packages on Raspian Wheezy
================================================

* Install qt5 (required for compiling):

```
  $ sudo apt-get install qt5-default
```

* Install `icu-devtools`, `libicu-dev`, `libxerces-c-dev`,
  `libxerces-c3.1` (required for compiling):

```
  $ sudo apt-get install libxerces-c-dev
```

* Install `libasound2-dev` (required for compiling):

```
  $ sudo apt-get install libasound2-dev
```

* Install `libusb-dev` (required for compiling):

```
  $ sudo apt-get install libusb-dev
```

* Install `GIMP` (required for art-work and icons creation):

```
  $ sudo apt-get install gimp
```

* Install `Dia` (required for icons creation):

```
  $ sudo apt-get install dia
```

* Install `InkScape` (required for icons creation):

```
  $ sudo apt-get install inkscape
```

Compile art-frame application
=============================

* Change to home directory:

```
  $ cd
```

* Clone Git repository of art-frame application:

```
  $ git clone https://github.com/soundpaint/art-frame.git
```

* Compile art-frame application:

```
  $ cd art-frame
  $ make
```

X Autostart
===========

* Make RasPi boot into text console with auto login (e.g. via the
  `raspi-config` command line application).

* Create the file `~/.bash_login`:

```
  $ cp -ip ~/art-frame/scripts/bash_login ~/.bash_login
```

In case the file `~/.bash_login` already exists, make a backup of the
existing file!  Also, you may then want to merge the contents of file
`~/art-frame/scripts/bash_login` into your existing `~/.bash_login`
rather than overwriting the existing file.

For a production environment, in the `.bash_login` script, you may
want to comment in the `sudo shutdown -h now` line for automatically
shutting down the Raspberry Pi upon application exit.  If you do this,
it is highly recommended that you enable the SSH server on the Pi,
such that you do not lock out yourself.  If the SSH server is enabled,
you may login from an external host and stop the art-frame application
by killing the process that runs the `art-frame.sh` script.  Then, you
can go on working on your Pi as usual.

* Disable Xsession screen saver by editing
  `/etc/xdg/lxsession/LXDE/autostart`:

```
  $ sudo nano /etc/xdg/lxsession/LXDE/autostart
```

Comment out with comment character `#` (or remove the line
completely):

```
  # @xscreensaver -no-splash
```

Add the following lines:

```
  # art-frame: no screen saver
  @xset s noblank
  @xset s off
  @xset -dpms
```

Quit the editor with `CTRL-X`.

* Optionally: Rotate LCD screen by 180 degrees, useful for some Pi cases

```
  $ sudo nano /boot/config.txt
```

Append to the end the following two lines:

```
  # art frame: rotate LCD screen by 180 degrees
  lcd_rotate=2
```

Quit the editor with `CTRL-X`.

* Optionally: Disable touch (usually, you do _not_ want to do this,
  unless you want to show a self-running demo only without any user
  interaction)

```
  $ sudo nano /boot/config.txt
```

Append to the end the following two lines:

```
  # art frame: disable touch functionaly
  disable_touchscreen=1
```

Installing a Fan [optional]
===========================

Since the simulation uses all of the 4 CPU cores, the CPU may get quit
hot.  To limit power dissipation, the simulation slows down when the
CPU core temperature goes high.  Details of this regulating process
can be set up in the configuration file `config.xml`.

As an additional cooling option, a fan may be installed.  Pin GPIO4 is
used to turn the fan on (value 1) or off (value 0).  Note that the
GPIO pins do not deliver sufficiently enough current for driving a fan
(in fact, you may destroy your GPIO pin this way); instead, you should
use them to drive e.g. a transistor that turns on / off a fan.  You do
not need to install a fan, but you should be aware that by default,
the application (or rather, the scripts `fan-init.sh`, `fan-on.sh` and
`fan-off.sh`) configures and uses GPIO4 as output, even if no fan is
installed.

However, it turns out, that the noise of a fan that drops in now and
then may feel annoying.  For this reason, I personally prefer _not_ to
install a fan, but rather accept some slow down of the simulation.

Audio Configuration [optional]
==============================

The art-frame application produces a noisy sound, just like slowly
moving sand.  The loudness of the noise is proportional to the number
of pixels that are currently moving around.

* Ensure audio comes out of the 3.5mm jack rather than via HDMI:

```
  $ sudo raspi-config
```

  [select _7 Advanced Options_ → _A4 Audio_ → _1 Force 3.5mm ('headphone') jack_]

  [select _Ok_]

  [select _Finish_]

* Select maximum volume:

```
  $ alsamixer
```

  [select maximum volume]

  [press `Esc` key to exit]

* If you do not need audio output, you should disable the audio
feature in the configuration file `config.xml` to save CPU computation
time and thus make the simulation appear possibly more smooth.

Images Configuration [optional]
===============================

If you want add or replace the images displayed by the art-frame
application, then modify the configuration file
`~/art-frame/build/bin/config.xml` as follows.  Beneath the XML node
`images` near the bottom of the file, specify the images to use.  Take
the existing definitions as examples.  Needless to say that you have
to provide those images that you specify in the `config.xml` file
under exactly the file path that you specify in your image definition.

Application Configuration [optional]
====================================

There are lots of configuration variables in the file
`~/art-frame/build/bin/config.xml`.  For a documentation of all of
these variables, just have a look into the corresponding comments in
the `config.xml` file.
