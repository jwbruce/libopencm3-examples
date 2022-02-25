/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (c) 2015 Chuck McManis <cmcmanis@mcmanis.com>
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

// include hardware mappings for Nucleo-L031K6 board (STM32L031K6)
#include "../nucleo-l031k6.h"

static void led_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(LD3_PORT_RCC);

	/* Set GPIO driving LD3) to 'output push-pull'. */
	gpio_mode_setup(LD3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD3_PIN);
}

int main(void)
{
	uint32_t        u32_i;

	led_setup();

	/* Blink the LD3 LED on the Nucleo board. */
	while (true) {

		/* Using API function gpio_toggle(): */
		gpio_toggle(LD3_PORT, LD3_PIN);	/* LED on/off */
        
        /* Wait a bit. */
		for (u32_i = 0; u32_i < 50000; u32_i++) {	
			__asm__("nop");
		}
	}

	return 0;
}
