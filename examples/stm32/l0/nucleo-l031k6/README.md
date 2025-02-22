# README

This folder contains assorted example projects for libopencm3 targeting
the Nucleo-L031K6 (STM32L031K6) evaluation board from ST Microelectronics.

The STM32L031K6 is a member of ST Microelectronics' access line
ultra-low-power 32-bit MCU Arm-based Cortex-M0+, with 32KB Flash,
8KB SRAM, 1KB EEPROM, ADC

The libopencm3 project aims to create an open-source firmware library for
various ARM Cortex-M microcontrollers.

For more information visit http://libopencm3.org


## Usage

The folder ../../../../libopencm3 (relative to this folder) must
contain the "made" (compliled) libopencm3 library.  Specifically,
the library *libopencm3_stm32l0.a* for the STM32L031
must be present down in this tree.  The most
convenient and modular method of making this configuration is
to **link** to the libopencm3 folder in your separate cloned
repo of the most recent libopencm3.

On linux, this is acheived by

  * opening a terminal window in this folder
  * executing *cd ../../../..* to move to the top-level folder
    of this repo
  * exectuing *rmdir libopencm3* to remove the existing empty folder
  * executing *ln -s* <PATH_TO_YOUR_LIBOPENCM3> *libopencm3* to create
    a symbolic link to the library in its repo.

For each example, you must run "make" in the directory containing
the example code.  Note that some examples require hardware to be
connected to you Nucleo-L031K6 board.  Other examples interact with
the Nucleo-L031K6 via the serial terminal.

For more verbose output, to see compiler command lines, use "make V=1"
For insanity levels of verboseness, use "make V=99"

The makefiles are generally useable for your own projects with
only minimal changes for the libopencm3 install path (See Reuse)

## Make Flash Target

Please note, the "make flash" target is complicated and
not always self-consistent.
Please see: https://github.com/libopencm3/libopencm3-examples/issues/34

For flashing the 'blink' example (after you built libopencm3 described
above) onto the ST Nucleo STM32L031K6 eval board, you can execute:

    cd examples/stm32/l0/nucleo-l031k6/blink
    make flash V=1

## Completed Examples

  * *blink* -- flashes the user LED **LD3**

  * *usart2* -- sends (115.2k/8N1) two different sets of
    repeating sequence of characters on UART.
    The output character sequence changes based on data
    received on UART. Sends are done in a blocking way.
    Receives do not block.

## TODOs

  * add better (and correct) comment/doxygen to the above examples


## Future Examples

  * button -- flashes the user LED **LD3** at two
    different rates depending on the state of an externally-
    installed user button

  * pwm1 -- periodically changes the brightness of **LD3**
    via pulse-width-modulation (PWM).

  * pwm2 -- changes the brightness of **LD3** via PWM
    based on characters received (115.2k/8N1) via UART.
    PWM value is returned over UART.
	Sends are done in a blocking way.
    Receives do not block.

  * timer -- flashes the user LED **LD3** with a Morse code
    pattern for SOS (dot-dot-dot dash-dash-dash dot-dot-dot)
    using the TIMER peripheral

  * usart1 -- sends (115.2k/8N1) repeating sequence of characters
    on UART.  Send is done in a blocking way.

  * usart_irq -- echos data received by UART 115200 baud (8N1);
    flashes the user LED **LD3** upon each data byte RX-ed;
    changes case of alphabetical data if externally-
    installed user button is pressed

  * foo -- based on the *f4/stm32f4-discovery/tick-blink*

  * ic -- tutorial using input capture to measure
    pulsewidth and/or frequency of B1 presses ?????

  * adc -- a new tutorial using the ADC with 2.048V internal reference

  * crc -- a new tutorial demonstrating the CRC generation peripheral

  * rtc -- a new tutorial to demonstrate the real-time clock peripheral

  * wdg -- a new tutorial to demonstrate watchdog timer

## Other Future Work

After building a complete set of examples for the Nucleo-L031K6
(based on the Nucleo-32 PCB from ST), adapt the examples with
appropriate header files to run on *any* Nucleo-32 based evaluation
board.
