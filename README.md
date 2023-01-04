# VESC Firmware

VESC firmware for https://github.com/Rampagy/bldc-hardware

## Control

There are two control schemes that are supported: sinusoidal pulse width modulation (spwm), and space vector pulse width modulation (svpwm).

![Supported commutation methods](images/supported_commutation.png)

## Tools

* [gcc](https://sourceforge.net/projects/mingw/files/latest/download) (for unit testing)
* python (for autogenerating lookup tables)

## Tests

To test do the following:

1. Navigate to tests folder via console `cd tests`
1. Run `build_and_run_tests.bat`