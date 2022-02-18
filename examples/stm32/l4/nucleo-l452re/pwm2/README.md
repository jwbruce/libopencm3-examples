# README

This example program controls the brightness of LD2
ST Nucleo-L452RE (STM32L452RE) eval board via
pulse-width-modulation (PWM) on TIM2_CH1.
The brightness of LD2 is controlled via
user input provided on the UART (115.2k/8n1).
Pressing the **+** key makes LD2 brighter;
pressing the **-** key makes LD2 dimmer.
All other key presses are ignored.

At each key press, the new value of the PWM
value is returned over the serial port.
Sending is done in a blocking way.

## Board connections

| Port  | Function      | Description                        |
| ----- | ------------- | ---------------------------------- |
| `PA2` | `(USART2_TX)` | TTL serial output `(115200,8,N,1)` |
| `PA3` | `(USART2_RX)` | TTL serial input `(115200,8,N,1)`  |
