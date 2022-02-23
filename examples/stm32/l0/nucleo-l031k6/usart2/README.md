# README

This example program sends repeating sequence of characters
"0123456789" or "ABCDEFGHIJ" to USART2 output on the 
ST Nucleo-L031K6 (STM32L031K6) eval board. The output sequence
of characters changes from numbers to letters and back again
based on the nature (number or character) of the most recent
character send to the Nucleo over the serial input.

The sending is done in a blocking way.
Serial reception does not block.

## Board connections

| Port   | Function      | Description                        |
| ------ | ------------- | ---------------------------------- |
| `PA2`  | `(USART2_TX)` | TTL serial output `(115200,8,N,1)` |
| `PA15` | `(USART2_RX)` | TTL serial input `(115200,8,N,1)`  |
