/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (c) 2022 J.W. Bruce <jwbruce@ieee.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// include hardware mappings for Nucleo-L031K6 board (STM32L031K6)
#include "../nucleo-l031k6.h"

static void led_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(LD3_PORT_RCC);

	/* Set GPIO driving LD3 to 'output push-pull'. */
	gpio_mode_setup(LD3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD3_PIN);
}

static void usart_setup(void)
{
	/* Enable clocks for USARTx itself */
	rcc_periph_clock_enable(USART_CONSOLE_RCC);
   
    /* Enable clocks for USARTx pins */
	rcc_periph_clock_enable(USART_CONSOLE_RX_PORT_RCC);    
    rcc_periph_clock_enable(USART_CONSOLE_TX_PORT_RCC);    
   
	/* Setup GPIO pins for USART transmit. */
	gpio_mode_setup(USART_CONSOLE_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_CONSOLE_TX_PIN);

	/* Setup GPIO pins for USART receive. */
	gpio_mode_setup(USART_CONSOLE_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_CONSOLE_RX_PIN);
	gpio_set_output_options(USART_CONSOLE_RX_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, USART_CONSOLE_RX_PIN);

	/* Setup USART2 TX and RX pin as alternate function. */
	gpio_set_af(USART_CONSOLE_RX_PORT, USART_CONSOLE_RX_AF, USART_CONSOLE_RX_PIN);
	gpio_set_af(USART_CONSOLE_TX_PORT, USART_CONSOLE_TX_AF, USART_CONSOLE_TX_PIN);
    
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART_CONSOLE, 115200);
	usart_set_databits(USART_CONSOLE, 8);
	usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
	usart_set_mode(USART_CONSOLE, USART_MODE_TX_RX);
	usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
	usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART_CONSOLE);
}


/* ******************************************************
 * Blink the LD3 LED on the Nucleo board.
 * Print a repeating sequence of characters (numbers
 * or letters) to the serial port (115.2k/8N1).
 * The repeating sequence changes from letters to numbers
 * and back again based if users sends a letter or number
 * character to the serial port.
 * ******************************************************/  
int main(void)
{
	uint32_t        u32_i;
    uint8_t         u8_j=0, u8_c=0, u8_in;
    static uint8_t  u8_base;
    

	led_setup();
    usart_setup();

    u8_base = '0';      // start program by sending numbers

	while (true) {
      
        /* Toggle LD2 whenever we send a character */
        gpio_toggle(LD3_PORT, LD3_PIN);	

        // see if UART2 has RX-ed anything
        if ( usart_get_flag(USART_CONSOLE, USART_ISR_RXNE) ) {
            u8_in = usart_recv( USART_CONSOLE );
            if (isalpha(u8_in))
                u8_base = 'A';      // got a letter, so send letters
            else
                u8_base = '0';      // else, send numbers
        }
    
        // send character out USART2
		usart_send_blocking(USART_CONSOLE, u8_c + u8_base);
        // increment our character
		u8_c = (u8_c == 9) ? 0 : u8_c + 1;
        // After 60 characters, send a newline
		if ((++u8_j % 60) == 0) {
            u8_j = 0;
			usart_send_blocking(USART_CONSOLE, '\r');
			usart_send_blocking(USART_CONSOLE, '\n');
		}
       
        // Wait a bit
		for (u32_i = 0; u32_i < 100000; u32_i++) {	
			__asm__("nop");
		}
	}

	return 0;
}
