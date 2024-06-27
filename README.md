# uncertainty-clock-divider

A musical clock divider and subharmonic generator for the eurorack module
[Uncertainty](https://github.com/oamodular/uncertainty)
by [Olivia Artz Modular](https://github.com/oamodular).

## About this firmware

The input of this module is treated as a clock source.  With each positive clock
pulse (crossing above 1.5V), the eight outputs will generate musical clock
divisions in powers of two (/2, /4, /8, /16, /32, /64, /128, and /256).  The
input is required to drop below 0.5V in order to trigger the next clock pulse
(i.e. Schmitt action).  This allows LFOs and other non-square waves to cleanly
trigger the clock divisions.

A negative clock pulse (crossing below -1.5V), will reset the clock
divisions to zero.  Since the module only has a single input, this means
clock and reset inputs will need to be mixed before being applied to the module.

## Installing

If you want to install this firmware as-is, you can download the .uf2 file and
follow the instructions [here](https://github.com/oamodular/uncertainty?tab=readme-ov-file#how-to-reinstall-the-default-firmware).

If you would like to modify the code and run it, you can do so using the Arduino
IDE, following the instructions
[here](https://wiki.seeedstudio.com/XIAO-RP2040-with-Arduino/).

## Use Cases

### Clock Divisions with Reset

To use as a standard clock divider with reset, attenuate the master clock signal
to range from 0.0V to ~2.5V.  Invert the reset clock signal so that it ranges
from 0.0V to -5.0V.  Mix these two signals and apply them to the input of
Uncertainty.  This will ensure that the master clock is able to trigger the
clock divisions, but when the reset clock goes high, even if the master clock is
present, the end result will be ~-2.5V, low enough to trigger the reset action.

### Subharmonic Generator

As with most clock dividers, this module can be used to generate eight square
waves, corresponding to the 8 octaves below the input's frequency.  However, due
to the reset behavior being triggered by negative voltages, some care is needed.

With a unipolar wave (e.g. 0-5V square wave, or looping envelope), the wave can
be applied to the input as-is, since the reset behavior will not be triggered.
For bipolar waves, however, you will need to offset and potentially attenuate
the wave first.  For example, a 2Vpp sine wave centered around 1V, will trigger
the clock divisions without triggering a reset, and so will a 5Vpp sine wave
centered above 1.0V.  Note: the closer to centering 5Vpp wave at 1.0, the more
risk there is of accidentally triggering a reset (1.0V - 2.5V = -1.5V), so it
is best to avoid these kinds of theoretical edge-cases.

The response at high frequencies is somewhat wave-shape-dependent.  With
unipolar square waves, the input can can slightly above the audible range
(20kHz).  With more sloped/curved waves, instability may occur earlier
(~10kHz).
