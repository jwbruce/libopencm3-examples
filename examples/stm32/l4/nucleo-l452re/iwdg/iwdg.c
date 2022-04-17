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
#include <libopencm3/stm32/iwdg.h>

// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

static void iwdg_setup(void)
{
   /* Sets up independent watchdog */
    iwdg_start();
   iwdg_set_period_ms(4096);
}

static void led_setup(void)
{
   * Enable GPIOA clock. */
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

int main(void)
{
    led_setup();
    button_setup();

    /* Blinks the LED to indicate reset (should end on LED on)*/
    gpio_set(LD2_PORT, LD2_PIN);
    for (uint32_t u32_j = 0; u32_j < 8; u32_j++) {
        for (uint32_t u32_i = 0; u32_i < 200000; u32_i++) {
            __asm__("nop");
        }
        gpio_toggle(LD2_PORT, LD2_PIN);
    }

    iwdg_setup();

    /* Keeps LED on as long as you keep pressing B1 within 4 ms */
    gpio_set(LD2_PORT, LD2_PIN);
    while (true) {

        /* Pressing B1 => 0, not pressing B1 => 1 */
        if(!gpio_get(B1_PORT, B1_PIN)) {
            /* Pressing B1 resets IWDG timer */
            iwdg_reset();
        }
    }

    return 0;
}
