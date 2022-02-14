# README

This example program echoes data sent in on USART2 on the
ST Nucleo-L452RE (STM32L452RE) eval board.  If the user
button B1 is pressed, the program changes the case of
alphabetical characters (lower -> upper, and vice-versa)

Uses interrupts moving data from RDR (RX data register)
and to TDR (TX data register).

The sending is done in a nonblocking way.

## Board connections

| Port  | Function      | Description                       |
| ----- | ------------- | --------------------------------- |
| `PA2` | `(USART2_TX)` | TTL serial output `(115200,8,N,1)` |
| `PA3` | `(USART2_RX)` | TTL serial input `(115200,8,N,1)`  |
