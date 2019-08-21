`art-frame` — An Artful Sands Image Emulation
=============================================

<!-- TODO: Describe basic purpose of the application, and give a
summary of supported features. -->

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

### Access Control GUI

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

![Fig. 1: Access Control Window](doc/images/control_window.png)

Fig. 1: Fully-Fledged Access Control Window

#### _Sensors_ Group

The _Sensors_ group shows sensor values from the sensor hat:

* the orientation of the art frame (_pitch_ and _roll_), and
* the acceleration of the art frame (_ax_ and _ay_).

#### _Temperature_ Group

The _Temperature_ group shows sensor values from the Raspberry Pi main
board:

* the temperature of the CPU (in °Celsius), and
* the temperature of the case (also in °Celsius).

#### _Art Frame_ Group

The _Art Frame_ group contains action buttons to

* show the _About_ window,
* show the _License_ window, and
* quit the application (and, optionally, thereby completely shut down
  the system).

#### _Simulation_ Group

The _Simulation_ group contains

* an action button to stop / resume the simulation, and
* a dial widget to control the violence of the simulation.

The violence of the simulation is a still experimental feature.  The
selected value should be a value near 0, but not equal to 0.  A value
of 0 will effectively pause the simulation.

#### _Image_ Group

The _Image_ group contains action buttons for

* switching to the previous of all configured images,
* resetting the current image to its initial state, and
* switching to the next of all configured images.

#### _Audio_ Group

The _Audio_ group contains

* an action button for muting / unmuting audio output, and
* a dial widget for controlling the audio volume.

The art frame application creates a (nearly) white noise sound.  The
loudness of the sound is proportional to the number of particles that
are currently moving.

### Other Windows

The access control window also provides access to an _About_ window.
This window can be activated either by the _About_ button or by any of
the configured keys for the _About_ action (be default, the _A_ key).

![Fig. 2: _About_ Window](doc/images/about_window.png)

Fig. 2: _About_ Window

There is also a window showing the license.  This window can be
activated either by the _License_ button or by any of the configured
keys for the _License_ action (be default, the _C_ key).

![Fig. 3: _License_ Window](doc/images/license_window.png)

Fig. 3: _License_ Window

### Kiosk Mode

In specific environments such as in an art exhibition, users may be
encouraged to play with the art frame, without a supervisor always
being present.  In such a setting, one may want to disable some
functionality such as shutting down the system or displaying audio
control features without any functional speaker attached to the
device.  In the `kiosk-mode` section, the configuration file
`config.xml` contains specific configuration variables to enable or
disable specific functionality available via the graphical GUI as well
as via any USB-connected keyboard.

![Fig. 4: Minimal Access Control Window](doc/images/control_window_kiosk.png)

Fig. 4: Minimal Access Control Window

Actually, the access control window can be completely deactivated, if
desired.

### How to Exit

The usual way to exit the art frame application is to either use the
_Quit_ button on the access control window or to press any of the
configured key for the _Quit_ action (by default, either the _Esc_ key
or the _Q_ key).  Upon quit, a confirmation dialog pops up.

![Fig. 5: Shutdown Confirmation Dialog](doc/images/shutdown_dialog.png)

Fig. 5: Shutdown Confirmation Dialog

Especially when installing the art-frame in a public room, you
probably do not want to let people access the internals of your
Raspberry Pi and / or shutting down the system via the GUI.

As explained above, the art-frame application's configuration file
offers many possibilities for access restrictions (called “kiosk-mode”
in the configuration file), from just disabling the “quit” button to
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

<!-- TODO: Describe gravity functionality. -->
<!-- TODO: Describe sweep functionality. -->

### Image Capturing

There is an experimental feature for capturing images.  Currently,
this feature is accessible only by a key press, but not yet via the
GUI.  By default, the key _T_ maps to the capture action.  When
pressing that key, the currently displayed image (more precisely: the
current image representation of the simulation status) is saved to a
file.  The target directory is specified with the `path` configuration
variable in the `capturing` section of the `config.xml` configuration
file.  By default, the path maps to the current working directory `.`,
which usually resolves to the path `/home/pi/art-frame/bin/build`,
where the art-frame binary resides.  The filename consists of a
prefix, a timestamp, and the `.png` suffix for the portable network
graphics media type of the generated image file.

<!-- TODO: Give usage examples and tips & tricks for practical use:
* When to rotate.
* When to sweep.
* Where to sweep.
-->
