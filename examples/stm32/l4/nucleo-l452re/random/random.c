/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Frantisek Burian <BuFran@seznam.cz>
 * Copyright (C) 2013 Piotr Esden-Tempski <piotr@esden.net>
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

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/l4/rng.h>
#include <libopencm3/cm3/systick.h>
#include "../nucleo-l452re.h"

/* Set STM32 to 80 MHz */
static void clock_setup(void)
{
    /* Use the high-speed internal 16 MHz source */
    rcc_osc_on(RCC_HSI16);

    /* 16MHz / 2 => 8 * 40 = 320MHz VCO / 4 => 80MHz main pll */
    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 2, 40,
		                 0, 0, RCC_PLLCFGR_PLLR_DIV4);
    rcc_osc_on(RCC_PLL);

    /* Turns on high-speed internal 48 MHz clock for RNG clock */
    rcc_osc_on(RCC_HSI48);
    rcc_periph_clock_enable(RCC_RNG);
}

static void led_setup(void)
{
    /* Enable LED's GPIO clock */
    rcc_periph_clock_enable(LD2_PORT_RCC);

    /* Set LEDs GPIO port/pin to 'output push-pull'. */
    gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);
}

static void rng_setup(void)
{
    /*
     * Enable interupt by setting the IE bit in the RNG_CR register.
     * Enable the random number generation by setting the RNGEN bit in
     * the RNG_CR register. This activates the analog part, the RNG_LFSR
     * and the error detector.
     */
    rng_enable();
    rng_interrupt_enable();
}

int main(void)
{
    uint32_t u32_i, u32_j;
    uint32_t u32_rnd = 0;
    clock_setup();
    led_setup();
    rng_setup();

    while (1) {

        /*
         * Generates a random 32-bit value using the built-in
         * true random number generator (RNG)
         */
        u32_rnd = rng_get_random_blocking();

        /*
         * Blink LD2 on and off according to the randomized
         * sequence of bits in u32_rnd starting with the
         * least significant bit, in which LD2 is on
         * when the current bit is 1 and LD2 is off
         * when the current bit is 0.
         */
        for (u32_i = 0; u32_i < 32; u32_i++) {
            if ((u32_rnd & (1 << u32_i)) != 0) {
                gpio_set(LD2_PORT, LD2_PIN);
            } else {
                gpio_clear(LD2_PORT, LD2_PIN);
			      }

            /* Each bit has a set delay length */
            for (u32_j = 0; u32_j < 200000; u32_j++) {
                __asm__("nop");
            }
        }
    }
}
