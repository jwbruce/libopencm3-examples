/*
 * This file is part of the libopencm3 project.
 *
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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#ifndef __NUCLEO_L452RE_H
#define __NUCLEO_L452RE_H

/* the STM Nucleo-64 PCB connects the user LED */
/* LD2 to GPIO pin PA5 through the SB42 solder jumper */
/* See the Nucleo-64 schematic and/or users manual*/
#define     LD2_PORT        GPIOA
#define     LD2_PIN         GPIO5

#endif    // __NUCLEO_L452RE_H
