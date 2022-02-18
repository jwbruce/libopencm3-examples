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
#include <libopencm3/stm32/timer.h>


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

	/* the GPIO pin mode will be configured in the timer setup */
	//gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);
    
}

static void usart_setup(void)
{
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


static void timer_setup(void)
{
   	// Enable LED's GPIO clock.
	rcc_periph_clock_enable(RCC_TIM2);
    
    // Set up TIM2
    timer_disable_counter(TIM2);
    timer_set_mode(TIM2,            // Operate on TIM3
            TIM_CR1_CKD_CK_INT,     // No prescaling(Internal clock)
            TIM_CR1_CMS_EDGE,       // Edge-aligned
            TIM_CR1_DIR_UP);        // Up-counting

    // Set up TIM2 output modes
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM2);
    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_enable_break_main_output(TIM2);

    // Configure channel 1 (PA5)
    // Will be low for ?? us, high for the remaining  ??us
    timer_set_oc_value(TIM2, TIM_OC1, 188);

    // Set TIM2's period to ?? us at ?? MHz
    timer_set_period(TIM2, 256 - 1);

    // Setup LD2 pin (PA5) to its alternate function, TIM2_CH1
    gpio_mode_setup(LD2_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LD2_PIN);
    
    // Set AF1(TIM2_CH1) for PA5
    gpio_set_af(LD2_PORT, GPIO_AF1, LD2_PIN);    

    // Enable the timer
    timer_enable_counter(TIM2);

}

int main(void)
{
    uint8_t         u8_c;
    uint8_t         u8_step;

    clock_setup();
	led_setup();
    usart_setup();
    timer_setup();
    
    // initialize LED to be a bit dim
    u8_step = 6;        
    timer_set_oc_value(TIM2, TIM_OC1, u8_step<<5);
    usart_send_blocking(USART2, '0'+(u8_step & 0x0F) );
    usart_send_blocking(USART2, ' ');
   
	/* Control brightness of the LD2 LED on the Nucleo board. */
	while (true) {
    
        // see if UART2 has RX-ed anything
        if (usart_get_flag( USART2, USART_ISR_RXNE )) {
            u8_c = usart_recv( USART2 );
            
            if (u8_c == '+') {
                if (u8_step != 0) u8_step--;
                timer_set_oc_value(TIM2, TIM_OC1, u8_step<<5);
            } else if (u8_c == '-') {
                if (u8_step <= 7) u8_step++;
                timer_set_oc_value(TIM2, TIM_OC1, u8_step<<5);
            }
            usart_send_blocking(USART2, '0'+(u8_step & 0x0F) );
            usart_send_blocking(USART2, ' ');
        }
        
	} // end while(true)

	return 0;
}
