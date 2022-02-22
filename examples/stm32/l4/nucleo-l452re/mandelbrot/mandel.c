/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Daniel Serpell <daniel.serpell@gmail.com>
 * Copyright (C) 2015 Piotr Esden-Tempski <piotr@esden.net>
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

/* Set STM32L452 instruction code to maximum (80MHz) */
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
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(LD2_PORT_RCC);

	/* Set GPIO driving LD2 to 'output push-pull'. */
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


/* Maximum number of iterations for the escape-time calculation */
#define maxIter 32

// from http://paulbourke.net/dataformats/asciiart/
// Character representation of grey scale images
// "Standard" character ramp for grey scale pictures, black -> white.
//      "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. "
// A more convincing but shorter sequence for representing 10 levels of grey is
//    " .:-=+*#%@"
//
// This array converts the iteration count to a character representation.
//static char color[maxIter+1] = " .:++xxXXX%%%%%%################";

static char color[maxIter+1] = " ..::--===+++****####%%%%%@@@@@@";

/* Main mandelbrot calculation */
static int iterate(float px, float py)
{
	int it = 0;
	float x = 0, y = 0;
	while (it < maxIter) {
		float nx = x*x;
		float ny = y*y;
		if ((nx + ny) > 4) {
			return it;
		}
		/* Zn+1 = Zn^2 + P */
		y = 2*x*y + py;
		x = nx - ny + px;
		it++;
	}
	return 0;
}

static void mandel(float cX, float cY, float scale)
{
	int x, y;
	for (x = -60; x < 60; x++) {
		for (y = -50; y < 50; y++) {
			int i = iterate(cX + x*scale, cY + y*scale);
			usart_send_blocking(USART2, color[i]);
		}
		//usart_send_blocking(USART2, '\r');
		usart_send_blocking(USART2, '\n');
	}
}

int main(void)
{
	float scale = 0.25f, centerX = -0.5f, centerY = 0.0f;

	clock_setup();
	led_setup();
	usart_setup();

	while (true) {
		/* Blink the LD2 LED on the board with each fractal drawn. */
		gpio_toggle(LD2_PORT, LD2_PIN);		/* LED on/off */
		mandel(centerX, centerY, scale);	/* draw mandelbrot */

		/* Change scale and center */
		centerX += 0.175f * scale;
		centerY += 0.522f * scale;
		scale	*= 0.875f;

		usart_send_blocking(USART2, '\r');
		usart_send_blocking(USART2, '\n');
	}
	return 0;
}
