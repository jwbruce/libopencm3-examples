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
#include <ctype.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>


// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

// locally defined macros go here


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


static void usart_setup(void)
{
	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

	/* Enable the USART2 interrupt. */
	nvic_enable_irq(NVIC_USART2_IRQ);

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

	/* Enable USART2 Receive interrupt. */
	usart_enable_rx_interrupt(USART2);

	/* Finally enable the USART. */
	usart_enable(USART2);
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

void usart2_isr(void)
{
	static uint8_t data = 'A';

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
	   (usart_get_flag(USART2,USART_ISR_RXNE))) {
        
		/* Indicate that we got data. */
		gpio_toggle(LD2_PORT, LD2_PIN);


		/* Retrieve the data from the peripheral. */
        data = usart_recv(USART2);

		/* Enable transmit interrupt so it sends back the data. */
		usart_enable_tx_interrupt(USART2);
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
	    (usart_get_flag(USART2,USART_ISR_TXE))) {

   		// if B1 button is pressed, then
        //     change case of any alphabetic characters
		if (!gpio_get(B1_PORT, B1_PIN)) {
                   if (isupper(data))
                     data = tolower(data);
                   else if (islower(data))
                     data = toupper(data);
        }

        /* Put data into the transmit register to send data */
		usart_send(USART2, data);

		/* Disable the TXE interrupt as we don't need it anymore. */
		usart_disable_tx_interrupt(USART2);
	}
}

int main(void)
{
	clock_setup();
	led_setup();
	button_setup();
	usart_setup();

	while (true) {
		__asm__("NOP");
	}

	return false;
}
