/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Damjan Marion <damjan.marion@gmail.com>
 * Copyright (C) 2011 Mark Panajotovic <marko@electrontube.org>
 * Copyright (C) 2013 Chuck McManis <cmcmanis@mcmanis.com>
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

/* This version derived from fancy blink */
#ifndef STM32L4
    #define STM32L4
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// include hardware mappings for Nucleo-L452RE board (STM32L452RE)
#include "../nucleo-l452re.h"

/*
 * Monotonically increasing number of milliseconds from reset
 * overflows every 49 days if you're wondering
 */
volatile uint32_t system_millis;

/* Called when systick fires */
void sys_tick_handler(void)
{
	system_millis++;
}

/* Sleep for delay milliseconds */
static void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

/* Set up a timer to create 1mS ticks. */
static void systick_setup(void)
{
	/* clock rate / 20 / 1000 to get 1mS interrupt rate */
	systick_set_reload(4000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	/* this done last */
	systick_interrupt_enable();
}

/* Set STM32 to 80 MHz. */
static void clock_setup(void)
{
    /* Use the high-speed internal 16 MHz source */
    rcc_osc_on(RCC_HSI16);

    /* 16MHz / 4 => 4 * 40 = 160MHz VCO => 80MHz main pll */
    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40,
                     0, 0, RCC_PLLCFGR_PLLR_DIV2);
    rcc_osc_on(RCC_PLL);
}

static void led_setup(void)
{
    /* Enable LED's GPIO clock */
    rcc_periph_clock_enable(LD2_PORT_RCC);

    /* Set LEDs GPIO port/pin to 'output push-pull'. */
    gpio_mode_setup(LD2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_PIN);
}


int main(void)
{
    clock_setup();
    led_setup();
    systick_setup();

    /* Blink the LD2 LED on the Nucleo board. */
    while (1) {
        gpio_toggle(LD2_PORT, LD2_PIN);
        msleep(100);
    }

    return 0;
}
