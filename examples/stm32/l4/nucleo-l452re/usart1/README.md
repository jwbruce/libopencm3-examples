# README

This example program sends repeating sequence of characters
"0123456789" on USART2 on the ST Nucleo-L452RE (STM32L452RE)
eval board.

The sending is done in a blocking way.

## Board connections

| Port  | Function      | Description                        |
| ----- | ------------- | ---------------------------------  |
| `PA2` | `(USART2_TX)` | TTL serial output `(115200,8,N,1)` |
