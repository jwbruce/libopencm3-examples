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

static void usart_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);
   
	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
   
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}


int main(void)
{
	uint32_t        u32_i;
    uint8_t         u8_j=0, u8_c=0;

	led_setup();
    usart_setup();

	/* Blink the LD2 LED on the Nucleo board. */
	while (true) {

		/* Using API function gpio_toggle(): */
		gpio_toggle(LD2_PORT, LD2_PIN);	/* LED on/off */
        
		usart_send_blocking(USART2, u8_c + '0'); /* USART2: Send byte. */
		u8_c = (u8_c == 9) ? 0 : u8_c + 1;	/* Increment c. */
		if ((++u8_j % 60) == 0) {		/* Newline after line full. */
            u8_j = 0;
			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');
		}
       
        /* Wait a bit. */
		for (u32_i = 0; u32_i < 200000; u32_i++) {	
			__asm__("nop");
		}
	}

	return 0;
}
