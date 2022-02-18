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
#include <libopencm3/stm32/timer.h>

// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

// locally defined macros go here
#define     PWM_PERIOD          256
#define     PWM_STEP            8

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
    // Start PWM value at mid-scale
    // Will be low for ?? us, high for the remaining  ??us
    timer_set_oc_value(TIM2, TIM_OC1, PWM_PERIOD/2);

    // Set TIM2's period to ?? us at ?? MHz
    timer_set_period(TIM2, PWM_PERIOD - 1);

    // Enable the timer
    timer_enable_counter(TIM2);


}

static void led_setup(void)
{
	/* Enable LED's GPIO clock. */
	rcc_periph_clock_enable(LD2_PORT_RCC);

	/* Set LEDs GPIO port/pin to 'output push-pull'. */
	gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);

    // Setup LD2 pin (PA5) to its alternate function, TIM2_CH1
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5);
    // Set AF1(TIM2_CH1) for PA5
    gpio_set_af(GPIOA, GPIO_AF1, GPIO5);    
   
    
}

int main(void)
{
    uint32_t        u32_i;
    int32_t         i32_pwmValue = 0;
    bool            b_countUp = true;
    
	clock_setup();
	led_setup();
    timer_setup();

	/* ramp brightess of LD2 LED on the Nucleo board up and down */
	while (true) {
        // repeatedly ramp up and down
        if (b_countUp) {
            i32_pwmValue -= PWM_STEP;
            if (i32_pwmValue <= 0) {
                i32_pwmValue=0;
                b_countUp = false;
            }
        } else  {
            i32_pwmValue += PWM_STEP;
            if (i32_pwmValue >= PWM_PERIOD) {
                i32_pwmValue=PWM_PERIOD-1;                     
                b_countUp = true;
            }
        }
        timer_set_oc_value(TIM2, TIM_OC1, i32_pwmValue);
        
        // Wait a bit 
		for (u32_i = 0; u32_i < 25000; u32_i++) {	
			__asm__("nop");
		}
	}

	return false;
}
