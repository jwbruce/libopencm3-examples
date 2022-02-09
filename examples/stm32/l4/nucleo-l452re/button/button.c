/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2022 J.W. Bruce <jwbruce@ieee.org>
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

// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

/* Set STM32 to 80 MHz */
static void clock_setup(void)
{
	// use the high-speed internal 16MHz source
   	rcc_osc_on(RCC_HSI16);

	/* 16MHz / 4 = > 4 * 40 = 160MHz VCO => 80MHz main pll  */
	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40,
			0, 0, RCC_PLLCFGR_PLLR_DIV2);
	rcc_osc_on(RCC_PLL);
    
}

static void led_setup(void)
{
	/* Enable LED's GPIO clock. */
	rcc_periph_clock_enable(LD2_PORT_RCC);

	/* Set LEDs GPIO port/pin to 'output push-pull'. */
	gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);
}


static void button_setup(void)
{
	/* Enable button's GPIO clock. */
	rcc_periph_clock_enable(B1_PORT_RCC);

	/* Set button's port/pin to 'input floating'. */
	gpio_mode_setup(B1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, B1_PIN);
}

int main(void)
{
	uint32_t        u32_i;

	clock_setup();
	button_setup();
	led_setup();

	/* Blink the LED (PD12) on the board. */
	while (true) {
		gpio_toggle(LD2_PORT, LD2_PIN);

		/* Upon button press, blink more slowly. */
		if (gpio_get(B1_PORT, B1_PIN)) {
            // Wait a bit.....
			for (u32_i = 0; u32_i < 200000; u32_i++) {	
				__asm__("nop");
			}
		}
        // Wait a bit more...
		for (u32_i = 0; u32_i < 100000; u32_i++) {		
			__asm__("nop");
		}
	}

	return 0;
}
