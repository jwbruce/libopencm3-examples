# README

This folder contains assorted example projects for libopencm3 targeting
the Nucleo-L452RE (STM32L452RE) evaluation board from ST Microelectronics.

The libopencm3 project aims to create an open-source firmware library for
various ARM Cortex-M microcontrollers.

For more information visit http://libopencm3.org


## Usage

The folder ../../../../libopencm3 (relative to this folder) must
contain the "made" (compliled) libopencm3 library.  Specifically,
the library *libopencm3_stm32l4.a* for the STM32L452RE
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
connected to you Nucleo-L452RE board.  Other examples interact with
the Nucleo-L452RE via the serial terminal.

For more verbose output, to see compiler command lines, use "make V=1"
For insanity levels of verboseness, use "make V=99"

The makefiles are generally useable for your own projects with
only minimal changes for the libopencm3 install path (See Reuse)

## Make Flash Target

Please note, the "make flash" target is complicated and
not always self-consistent.
Please see: https://github.com/libopencm3/libopencm3-examples/issues/34

For flashing the 'blink' example (after you built libopencm3 described
above) onto the ST Nucleo STM32L452RE eval board, you can execute:

    cd examples/stm32/l4/nucleo-l452re/blink
    make flash V=1

## Completed Examples

  * blink -- flashes the user LED **LD2**

  * button -- flashes the user LED **LD2** at two
    different rates depending on the state of the user
    button **B1**

  * mandelbrot -- demonstrate the floating-point capability and speed
    of the STM32L452RE processor by computing the Mandelbrot set
    and "drawing" the famous Mandelbrot "beetle" by sending
    (115.2k/8N1) ASCII art to the UART.
    Sending is done in a blocking way.

  * pwm1 -- periodically changes the brightness of **LD2**
    via pulse-width-modulation (PWM) on TIM2_CH1.

  * pwm2 -- changes the brightness of **LD2** via PWM
    based on characters received (115.2k/8N1) via UART.
    PWM value is returned over UART.
	Sends are done in a blocking way.
    Receives do not block.
    
  * tick_blink -- flashes the user LED **LD2** using
    the systick timer to generate time accurate delays/interrupts.

  * timer -- flashes the user LED **LD2** with a Morse code
    pattern for SOS (dot-dot-dot dash-dash-dash dot-dot-dot)
    using the TIM2 peripheral

  * usart1 -- sends (115.2k/8N1) repeating sequence of characters
    on UART.  Send is done in a blocking way.

  * usart2 -- sends (115.2k/8N1) two different sets of
    repeating sequence of characters on UART.
    The output character sequence changes based on data
    received on UART. Sends are done in a blocking way.
    Receives do not block.

  * usart_irq -- echos data received by UART 115200 baud (8N1);
    flashes the user LED **LD2** upon each data byte RX-ed;
    changes case of alphabetical data if user button **B1** is pressed

## TODOs

  * add better (and correct) comment/doxygen to the above examples


## Future Examples

foo -- based on the *f4/stm32f4-discovery/adc-dac-printf*

foo -- based on the *f4/stm32f429i-discovery/random*

*ic* -- tutorial using input capture to measure
pulsewidth and/or frequency of B1 presses ?????

*ext_timer* -- a new tutorial using timer with external trigger input
to count the edges from a pin, *e.g.* count bounces on user
button **B1** presses during a short window (15-25ms)

*adc* -- a new tutorial using the ADC with 2.048V internal reference
to measure ???? (anything internal to measure? Attach an inexpensive
sensor like a CdS cell or thermistor or pot)

*dac* -- a new tutorial using the built-in DAC to generate one (or
more) voltage waveforms.  Possibly adjust characteristics of the
waveform via **B1** or the UART

*reset* -- a new tutorial program to determine the reset source upon boot and
print to UART (provide a **B1** or UART method to cause code to run
to create a myriad of reset conditions)

*rtc* -- a new tutorial to demonstrate the real-time clock peripheral
(use serial port to set and display RTC information???)

*wdg* -- a new tutorial to demonstrate watchdog timer (user must press
button **B1** often enough to prevent WDG timeout)

## Other Future Work

After building a complete set of examples for the Nucleo-L452RE
(based on the Nucleo-64 PCB from ST), adapt the examples with
appropriate header files to run on *any* Nucleo-64 based evaluation
board.

Adapt as many of the above examples (as appropriate) to the small
Nucleo board (**Nucleo-F031K6** and **Nucleo-L031K6**) used in the
Tennessee Tech program.  Should be able (with some connected
hardware) to adapt all of these programs, except *mandelbrot* and 
*dac* as F0 and L0 parts do not have a FPU or DACs.
