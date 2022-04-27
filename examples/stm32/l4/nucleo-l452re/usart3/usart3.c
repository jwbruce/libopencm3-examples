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
 #ifndef STM32L4
    #define STM32L4
 #endif

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

static void led_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(LD2_PORT_RCC);

	/* Set GPIO driving LD2 (PA5) to 'output push-pull'. */
	gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);
}

static void button_setup(void)
{
	/* Enable button's GPIO clock. */
	rcc_periph_clock_enable(B1_PORT_RCC);

	/* Set button's port/pin to 'input floating'. */
	gpio_mode_setup(B1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, B1_PIN);
}

static void usart_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup GPIO pins for USART2 receive. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);

	/* Setup USART2 TX and RX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO3);

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

void usart_send_string(char* pach_str)
{
    for (uint32_t u32_i = 0; u32_i < strlen(pach_str); u32_i++) {
        if (pach_str[u32_i] == '\n') {
            /*
             * When using PuTTY, you must send \r\n to
             * write text on the next line
             */
            usart_send_blocking(USART2, '\r');
        }
        usart_send_blocking(USART2, pach_str[u32_i]);
    }
}

/* ******************************************************
 * Blink the LD2 LED on the Nucleo board.
 * Prints an entire message, "Hello, World!" if
 * B1 is released or "Goodbye, World!" if B1 is pressed
 * ******************************************************/
int main(void)
{
    led_setup();
    button_setup();
    usart_setup();
    char ach_msg1[] = "Hello, World!\n";
    char ach_msg2[] = "Goodbye, World!\n";

    while (true) {

        /* Toggle LD2 whenever we send an entire string  */
        gpio_toggle(LD2_PORT, LD2_PIN);

        /*
         * Button not pressed -> send Hello
         * Button pressed -> send Goodbye
         */
        if (gpio_get(B1_PORT, B1_PIN)) {
    		    usart_send_string(ach_msg1);
        }
        else {
            usart_send_string(ach_msg2);
        }

        /* Wait a bit */
		    for (uint32_t u32_i = 0; u32_i < 1000000; u32_i++) {
			      __asm__("nop");
		    }
   	}
  	return 0;
}
