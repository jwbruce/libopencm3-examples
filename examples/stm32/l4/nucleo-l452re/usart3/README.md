# README

This example program sends an entire string of characters
to USART2 output on the ST Nucleo-L452RE (STM32L452RE) eval board.
If B1 is released, "Hello, World!" is sent to the serial
output. If B1 is pressed, "Goodbye, World!" is sent to
the serial output. Each string is sent on its own line.

The sending is done in a blocking way.
Serial reception does not block.

## Board connections

| Port  | Function      | Description                        |
| ----- | ------------- | ---------------------------------- |
| `PA2` | `(USART2_TX)` | TTL serial output `(115200,8,N,1)`|
| `PA3` | `(USART2_RX)` | TTL serial input `(115200,8,N,1)` |
