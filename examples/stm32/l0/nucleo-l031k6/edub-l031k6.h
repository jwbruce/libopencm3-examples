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

/* *****************************************************************
 *  The following data was ripped from the Nucleo-32 User Manual
 *  UM1956 found at
 *  https://www.st.com/resource/en/user_manual/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf
 * 
 *  TABLE 14:  Arduino Nano connectors on NUCLEO-L031K6 
 * 
 *  CN3 pin     name    STM32_pin   function
 *  ----------  ------- ----------- -----------------
 *  CN3.1       D1      PA9         USART2_TX(1)
 *  CN3.2       D0      PA10        USART2_RX(1)
 *  CN3.3       RESET   NRST        RESET
 *  CN3.4       GND                 Ground
 *  CN3.5       D2      PA12        -
 *  CN3.6       D3      PB0         TIM2_CH3
 *  CN3.7       D4(2)   PB7         -
 *  CN3.8       D5(2)   PB6         TIM21_CH1
 *  CN3.9       D6      PB1         TIM2_CH4
 *  CN3.10      D7(3)   PC14        -
 *  CN3.11      D8(3)   PC15        -
 *  CN3.12      D9      PA8         TIM2_CH1
 *  CN3.13      D10     PA11        SPI_CS(4)
 *  CN3.14      D11     PB5         SPI1_MOSI || TIM22_CH2
 *  CN3.15      D12     PB4         SPI1_MISO
 * 
 *  CN4.1       VIN     -           Power input
 *  CN4.2       GND     -           Ground
 *  CN4.3       RESET   NRST        RESET
 *  CN4.4       +5V     -           5 V input/output
 *  CN4.5       A7      PA2         ADC_IN2(5)
 *  CN4.6       A6      PA7         ADC_IN7
 *  CN4.7       A5(2)   PA6         ADC_IN6 || I2C1_SCL
 *  CN4.8       A4(2)   PA5         ADC_IN5 || I2C1_SDA
 *  CN4.9       A3      PA4         ADC_IN4
 *  CN4.10      A2      PA3         ADC_IN3
 *  CN4.11      A1      PA1         ADC_IN1
 *  CN4.12      A0      PA0         ADC_IN0
 *  CN4.13      AREF    -           AVDD
 *  CN4.14      +3V3    -           3.3 V input/output
 *  CN4.15      D13     PB3         SPI1_SCK
 * ***************************************************************/
    
#ifndef __EDUBASE_L031K6_H
#define __EDUBASE_L031K6_H

/* the Nucleo-L031K6 PCB connects the EduBase LED0 */
/* to GPIO pin PA12. See the EduBase-V2 schematic */
#define     LED0_PORT        GPIOA
#define     LED0_PIN         GPIO12
#define     LED0_PORT_RCC    RCC_GPIOA

/* the Nucleo-L031K6 PCB connects the EduBase LED1 */
/* to GPIO pin PB0. See the EduBase-V2 schematic */
#define     LED1_PORT        GPIOB
#define     LED1_PIN         GPIO0
#define     LED1_PORT_RCC    RCC_GPIOB

/* the Nucleo-L031K6 PCB connects the EduBase LED2 */
/* to GPIO pin PC14. See the EduBase-V2 schematic */
#define     LED2_PORT        GPIOC
#define     LED2_PIN         GPIO14
#define     LED2_PORT_RCC    RCC_GPIOC

/* the Nucleo-L031K6 PCB connects the EduBase LED3 */
/* to GPIO pin PB7. See the EduBase-V2 schematic */
/* However, the Nucleo-32 comes with a solder jumper */
/* SB18 installed which, by default, connects PB7 */
/* to Edubase-V2 switch SW4.  Remove SB18 to isolate *
/* and control LED3 */
//#define     LED3_PORT        GPIOB
//#define     LED3_PIN         GPIO7
//#define     LED3_PORT_RCC    RCC_GPIOB

/* the Nucleo-L031K6 PCB connects the EduBase SW4 */
/* to GPIO pin PA5. See the EduBase-V2 schematic */
#define     SW4_PORT			GPIOA
#define     SW4_PIN				GPIO5
#define     SW4_PORT_RCC    	RCC_GPIOA

/* the Nucleo-L031K6 PCB connects the EduBase SW5 */
/* to GPIO pin PA6. See the EduBase-V2 schematic */
#define     SW5_PORT			GPIOA
#define     SW5_PIN				GPIO6
#define     SW5_PORT_RCC    	RCC_GPIOA

/* the Nucleo-L031K6 PCB connects the EduBase piezo- */
/* electric buzzer -- termed the speaker -- to GPIO */
/* pin PA8. See the EduBase-V2 schematic            */
#define     SPEAKER_PORT			GPIOA
#define     SPEAKER_PIN				GPIO8
#define     SPEAKER_PORT_RCC    	RCC_GPIOA

/* the Nucleo-L031K6 PCB connects the /CS_7 (active- */
/* low) chip select for the seven-segment display */
/* latch to GPIO pin PA7. See the EduBase-V2 schematic */
#define     CS_7_PORT				GPIOA
#define     CS_7_PIN				GPIO7
#define     CS_7_PORT_RCC    		RCC_GPIOA

/* the Nucleo-L031K6 PCB connects the /CS_SD (active- */
/* low) chip select for the SD memory card to GPIO */
/* pin PC15. See the EduBase-V2 schematic */
#define     CS_SD_PORT				GPIOC
#define     CS_SD_PIN				GPIO15
#define     CS_SD_PORT_RCC    		RCC_GPIOC

/* the Nucleo-L031K6 PCB connects the /CS_LCD (active- */
/* low) chip select for the LCD character display latch */
/* to GPIO pin PA11. See the EduBase-V2 schematic */
#define     CS_LCD_PORT				GPIOA
#define     CS_LCD_PIN				GPIO11
#define     CS_LCD_PORT_RCC    		RCC_GPIOA


// *******************************************************
// * There are more Nucleo-L031K6 to EduBase-V2 #defines
// * to be made as the examples are fleshed out.
// *******************************************************
/* the STM Nucleo-32 PCB connects the SERIAL CONSOLE RX */
/* to the alternate function on GPIO pin PA15 */
/* See the Nucleo-32 schematic and/or users manual*/
//#define     USART_CONSOLE                 USART2
//#define     USART_CONSOLE_RCC             RCC_USART2
//#define     USART_CONSOLE_RX_PORT         GPIOA
//#define     USART_CONSOLE_RX_PIN          GPIO15
//#define     USART_CONSOLE_RX_PORT_RCC     RCC_GPIOA
//#define     USART_CONSOLE_RX_AF           GPIO_AF4


#endif    // __EDUBASE_L031K6_H
