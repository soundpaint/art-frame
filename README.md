`art-frame` — An Artful Sands Image Emulation
=============================================

Source Code available here:

[https://github.com/soundpaint/art-frame](https://github.com/soundpaint/art-frame)

Installation & Configuration
----------------------------

For details, see the file [INSTALL.md](INSTALL.md).

In summary:

* Install some additional packages for Qt, XML, USB, audio and
  processing of art work.
* Download the sources.
* Compile the sources.
* Tweak some spots of the Pi's boot process.
* Configure the application to tailor your needs.
* Optionally provide additional images.
* Run it & have fun.

Usage Notes
-----------

### Access Control GUI via Double Click

While the art-frame application is running, you can alyways access the
control GUI by double-touching the screen (just like a double click
with a mouse), unless you have explicitly disabled the control GUI in
the application's configuration.  Be careful that both touches apply
to the very same location on the screen.  If the second touch is
slightly off regarding the position on the screen, the double touch
will show no effect.  Similarly, the double touch will show no effect
if it either occurs to fast, or if the amount of time between the
first and the second touch is too large.  You probably best achieve a
double touch if you keep your finger our pointing device very close to
the surface and perform a very light touch.  This way, you have better
fine-control over your touch and increase the chance to hit exactly
the same position twice.

### How to Exit

Especially when installing the art-frame in a public room, you
probably do not want to let people access the internals of your
Raspberry Pi and / or shutting down the system via the GUI.

The art-frame application's configuration file offers many
possibilities for access restrictions (called “kiosk-mode” in the
configuration file), from just disabling the “quit” button to
disabling the complete GUI control widget.  However, if you disable
all that stuff, how can you then ordinarily shut down the system by
yourself?

There some options:

* Connect a USB keyboard to the Pi.  Depending on the configuration,
  you may quit the art-frame application with a single key stroke.
  Alternatively, you may press, say, `CTRL + ALT + F3` to get the
  login prompt for a text console which enables you to kill the
  `art-frame.sh` shell script.
* If you have disabled keyboard functionality (because you fear people
  may connect a keyboard via USB), you may alternatively enable the
  SSH server on your Pi and then login from a remote host and kill the
  `art-frame.sh` shell script via the remote shell.
* You may configure a cron job that automatically shuts down the Pi at
  some time of the day.
