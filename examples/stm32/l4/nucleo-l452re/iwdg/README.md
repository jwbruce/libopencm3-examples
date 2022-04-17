# README

This example demonstrates the use of the independent watchdog (IWDG) timer.

The LED will blink 4 times before turning on indefinitely,
which is when the watchdog is enabled. The IWDG is set to a 4096 ms
timeout period, and once the timer expires, the watchdog will
reset the program, notifying the user with the initial 4 LED blinks.
To prevent IWDG timeout, the user must press B1 continually
within 4 s.

## Board connections

*none required*
