/*******************************************************************************
  PORT PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_port.h

  Summary:
    PORT PLIB Header File

  Description:
    This file provides an interface to control and interact with PORT-I/O
    Pin controller module.

 *******************************************************************************/

/*******************************************************************************
 * Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/

#ifndef PLIB_PORT_H
#define PLIB_PORT_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
    // DOM-IGNORE-END

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data types and constants
    // *****************************************************************************
    // *****************************************************************************

    /*** Macros for GPIO_PC00 pin ***/
#define GPIO_PC00_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 0U))
#define GPIO_PC00_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 0U))
#define GPIO_PC00_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 0U))
#define GPIO_PC00_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 0U))
#define GPIO_PC00_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 0U))
#define GPIO_PC00_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 0U)) & 0x01U)
#define GPIO_PC00_PIN                  PORT_PIN_PC00

    /*** Macros for GPIO_PC01 pin ***/
#define GPIO_PC01_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 1U))
#define GPIO_PC01_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 1U))
#define GPIO_PC01_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 1U))
#define GPIO_PC01_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 1U))
#define GPIO_PC01_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 1U))
#define GPIO_PC01_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 1U)) & 0x01U)
#define GPIO_PC01_PIN                  PORT_PIN_PC01

    /*** Macros for GPIO_PC02 pin ***/
#define GPIO_PC02_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 2U))
#define GPIO_PC02_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 2U))
#define GPIO_PC02_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 2U))
#define GPIO_PC02_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 2U))
#define GPIO_PC02_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 2U))
#define GPIO_PC02_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 2U)) & 0x01U)
#define GPIO_PC02_PIN                  PORT_PIN_PC02

    /*** Macros for GPIO_PC03 pin ***/
#define GPIO_PC03_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 3U))
#define GPIO_PC03_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 3U))
#define GPIO_PC03_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 3U))
#define GPIO_PC03_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 3U))
#define GPIO_PC03_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 3U))
#define GPIO_PC03_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 3U)) & 0x01U)
#define GPIO_PC03_PIN                  PORT_PIN_PC03

    /*** Macros for GPIO_PA02 pin ***/
#define GPIO_PA02_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 2U))
#define GPIO_PA02_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 2U))
#define GPIO_PA02_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 2U))
#define GPIO_PA02_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 2U))
#define GPIO_PA02_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 2U))
#define GPIO_PA02_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 2U)) & 0x01U)
#define GPIO_PA02_PIN                  PORT_PIN_PA02

    /*** Macros for GPIO_PA03 pin ***/
#define GPIO_PA03_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 3U))
#define GPIO_PA03_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 3U))
#define GPIO_PA03_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 3U))
#define GPIO_PA03_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 3U))
#define GPIO_PA03_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 3U))
#define GPIO_PA03_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 3U)) & 0x01U)
#define GPIO_PA03_PIN                  PORT_PIN_PA03

    /*** Macros for GPIO_PB04 pin ***/
#define GPIO_PB04_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 4U))
#define GPIO_PB04_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 4U))
#define GPIO_PB04_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 4U))
#define GPIO_PB04_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 4U))
#define GPIO_PB04_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 4U))
#define GPIO_PB04_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 4U)) & 0x01U)
#define GPIO_PB04_PIN                  PORT_PIN_PB04

    /*** Macros for GPIO_PB05 pin ***/
#define GPIO_PB05_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 5U))
#define GPIO_PB05_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 5U))
#define GPIO_PB05_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 5U))
#define GPIO_PB05_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 5U))
#define GPIO_PB05_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 5U))
#define GPIO_PB05_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 5U)) & 0x01U)
#define GPIO_PB05_PIN                  PORT_PIN_PB05

    /*** Macros for GPIO_PD00 pin ***/
#define GPIO_PD00_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 0U))
#define GPIO_PD00_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 0U))
#define GPIO_PD00_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 0U))
#define GPIO_PD00_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 0U))
#define GPIO_PD00_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 0U))
#define GPIO_PD00_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 0U)) & 0x01U)
#define GPIO_PD00_PIN                  PORT_PIN_PD00

    /*** Macros for GPIO_PD01 pin ***/
#define GPIO_PD01_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 1U))
#define GPIO_PD01_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 1U))
#define GPIO_PD01_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 1U))
#define GPIO_PD01_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 1U))
#define GPIO_PD01_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 1U))
#define GPIO_PD01_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 1U)) & 0x01U)
#define GPIO_PD01_PIN                  PORT_PIN_PD01

    /*** Macros for GPIO_PB06 pin ***/
#define GPIO_PB06_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 6U))
#define GPIO_PB06_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 6U))
#define GPIO_PB06_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 6U))
#define GPIO_PB06_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 6U))
#define GPIO_PB06_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 6U))
#define GPIO_PB06_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 6U)) & 0x01U)
#define GPIO_PB06_PIN                  PORT_PIN_PB06

    /*** Macros for GPIO_PB07 pin ***/
#define GPIO_PB07_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 7U))
#define GPIO_PB07_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 7U))
#define GPIO_PB07_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 7U))
#define GPIO_PB07_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 7U))
#define GPIO_PB07_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 7U))
#define GPIO_PB07_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 7U)) & 0x01U)
#define GPIO_PB07_PIN                  PORT_PIN_PB07

    /*** Macros for GPIO_PB08 pin ***/
#define GPIO_PB08_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 8U))
#define GPIO_PB08_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 8U))
#define GPIO_PB08_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 8U))
#define GPIO_PB08_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 8U))
#define GPIO_PB08_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 8U))
#define GPIO_PB08_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 8U)) & 0x01U)
#define GPIO_PB08_PIN                  PORT_PIN_PB08

    /*** Macros for GPIO_PB09 pin ***/
#define GPIO_PB09_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 9U))
#define GPIO_PB09_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 9U))
#define GPIO_PB09_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 9U))
#define GPIO_PB09_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 9U))
#define GPIO_PB09_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 9U))
#define GPIO_PB09_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 9U)) & 0x01U)
#define GPIO_PB09_PIN                  PORT_PIN_PB09

    /*** Macros for GPIO_PA04 pin ***/
#define GPIO_PA04_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 4U))
#define GPIO_PA04_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 4U))
#define GPIO_PA04_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 4U))
#define GPIO_PA04_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 4U))
#define GPIO_PA04_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 4U))
#define GPIO_PA04_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 4U)) & 0x01U)
#define GPIO_PA04_PIN                  PORT_PIN_PA04

    /*** Macros for GPIO_PA05 pin ***/
#define GPIO_PA05_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 5U))
#define GPIO_PA05_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 5U))
#define GPIO_PA05_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 5U))
#define GPIO_PA05_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 5U))
#define GPIO_PA05_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 5U))
#define GPIO_PA05_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 5U)) & 0x01U)
#define GPIO_PA05_PIN                  PORT_PIN_PA05

    /*** Macros for GPIO_PA07 pin ***/
#define GPIO_PA07_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 7U))
#define GPIO_PA07_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 7U))
#define GPIO_PA07_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 7U))
#define GPIO_PA07_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 7U))
#define GPIO_PA07_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 7U))
#define GPIO_PA07_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 7U)) & 0x01U)
#define GPIO_PA07_PIN                  PORT_PIN_PA07

    /*** Macros for GPIO_PC04 pin ***/
#define GPIO_PC04_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 4U))
#define GPIO_PC04_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 4U))
#define GPIO_PC04_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 4U))
#define GPIO_PC04_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 4U))
#define GPIO_PC04_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 4U))
#define GPIO_PC04_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 4U)) & 0x01U)
#define GPIO_PC04_PIN                  PORT_PIN_PC04

    /*** Macros for GPIO_PC05 pin ***/
#define GPIO_PC05_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 5U))
#define GPIO_PC05_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 5U))
#define GPIO_PC05_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 5U))
#define GPIO_PC05_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 5U))
#define GPIO_PC05_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 5U))
#define GPIO_PC05_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 5U)) & 0x01U)
#define GPIO_PC05_PIN                  PORT_PIN_PC05

    /*** Macros for GPIO_PC06 pin ***/
#define GPIO_PC06_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 6U))
#define GPIO_PC06_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 6U))
#define GPIO_PC06_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 6U))
#define GPIO_PC06_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 6U))
#define GPIO_PC06_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 6U))
#define GPIO_PC06_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 6U)) & 0x01U)
#define GPIO_PC06_PIN                  PORT_PIN_PC06

    /*** Macros for GPIO_PC07 pin ***/
#define GPIO_PC07_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 7U))
#define GPIO_PC07_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 7U))
#define GPIO_PC07_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 7U))
#define GPIO_PC07_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 7U))
#define GPIO_PC07_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 7U))
#define GPIO_PC07_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 7U)) & 0x01U)
#define GPIO_PC07_PIN                  PORT_PIN_PC07

    /*** Macros for GPIO_PA08 pin ***/
#define GPIO_PA08_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 8U))
#define GPIO_PA08_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 8U))
#define GPIO_PA08_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 8U))
#define GPIO_PA08_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 8U))
#define GPIO_PA08_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 8U))
#define GPIO_PA08_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 8U)) & 0x01U)
#define GPIO_PA08_PIN                  PORT_PIN_PA08

    /*** Macros for GPIO_PA09 pin ***/
#define GPIO_PA09_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 9U))
#define GPIO_PA09_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 9U))
#define GPIO_PA09_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 9U))
#define GPIO_PA09_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 9U))
#define GPIO_PA09_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 9U))
#define GPIO_PA09_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 9U)) & 0x01U)
#define GPIO_PA09_PIN                  PORT_PIN_PA09

    /*** Macros for GPIO_PA10 pin ***/
#define GPIO_PA10_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 10U))
#define GPIO_PA10_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define GPIO_PA10_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define GPIO_PA10_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 10U))
#define GPIO_PA10_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define GPIO_PA10_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 10U)) & 0x01U)
#define GPIO_PA10_PIN                  PORT_PIN_PA10

    /*** Macros for GPIO_PA11 pin ***/
#define GPIO_PA11_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 11U))
#define GPIO_PA11_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 11U))
#define GPIO_PA11_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 11U))
#define GPIO_PA11_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 11U))
#define GPIO_PA11_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 11U))
#define GPIO_PA11_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 11U)) & 0x01U)
#define GPIO_PA11_PIN                  PORT_PIN_PA11

    /*** Macros for GPIO_PB10 pin ***/
#define GPIO_PB10_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 10U))
#define GPIO_PB10_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define GPIO_PB10_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define GPIO_PB10_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 10U))
#define GPIO_PB10_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define GPIO_PB10_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 10U)) & 0x01U)
#define GPIO_PB10_PIN                  PORT_PIN_PB10

    /*** Macros for GPIO_PB11 pin ***/
#define GPIO_PB11_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 11U))
#define GPIO_PB11_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 11U))
#define GPIO_PB11_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 11U))
#define GPIO_PB11_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 11U))
#define GPIO_PB11_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 11U))
#define GPIO_PB11_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 11U)) & 0x01U)
#define GPIO_PB11_PIN                  PORT_PIN_PB11

    /*** Macros for GPIO_PB12 pin ***/
#define GPIO_PB12_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 12U))
#define GPIO_PB12_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 12U))
#define GPIO_PB12_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 12U))
#define GPIO_PB12_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 12U))
#define GPIO_PB12_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 12U))
#define GPIO_PB12_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 12U)) & 0x01U)
#define GPIO_PB12_PIN                  PORT_PIN_PB12

    /*** Macros for GPIO_PB13 pin ***/
#define GPIO_PB13_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 13U))
#define GPIO_PB13_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 13U))
#define GPIO_PB13_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 13U))
#define GPIO_PB13_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 13U))
#define GPIO_PB13_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 13U))
#define GPIO_PB13_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 13U)) & 0x01U)
#define GPIO_PB13_PIN                  PORT_PIN_PB13

    /*** Macros for GPIO_PB14 pin ***/
#define GPIO_PB14_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 14U))
#define GPIO_PB14_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 14U))
#define GPIO_PB14_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 14U))
#define GPIO_PB14_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 14U))
#define GPIO_PB14_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 14U))
#define GPIO_PB14_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 14U)) & 0x01U)
#define GPIO_PB14_PIN                  PORT_PIN_PB14

    /*** Macros for GPIO_PB15 pin ***/
#define GPIO_PB15_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 15U))
#define GPIO_PB15_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 15U))
#define GPIO_PB15_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 15U))
#define GPIO_PB15_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 15U))
#define GPIO_PB15_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 15U))
#define GPIO_PB15_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 15U)) & 0x01U)
#define GPIO_PB15_PIN                  PORT_PIN_PB15

    /*** Macros for GPIO_PD08 pin ***/
#define GPIO_PD08_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 8U))
#define GPIO_PD08_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 8U))
#define GPIO_PD08_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 8U))
#define GPIO_PD08_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 8U))
#define GPIO_PD08_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 8U))
#define GPIO_PD08_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 8U)) & 0x01U)
#define GPIO_PD08_PIN                  PORT_PIN_PD08

    /*** Macros for GPIO_PD09 pin ***/
#define GPIO_PD09_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 9U))
#define GPIO_PD09_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 9U))
#define GPIO_PD09_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 9U))
#define GPIO_PD09_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 9U))
#define GPIO_PD09_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 9U))
#define GPIO_PD09_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 9U)) & 0x01U)
#define GPIO_PD09_PIN                  PORT_PIN_PD09

    /*** Macros for GPIO_PD10 pin ***/
#define GPIO_PD10_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 10U))
#define GPIO_PD10_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define GPIO_PD10_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define GPIO_PD10_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 10U))
#define GPIO_PD10_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define GPIO_PD10_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 10U)) & 0x01U)
#define GPIO_PD10_PIN                  PORT_PIN_PD10

    /*** Macros for GPIO_PD11 pin ***/
#define GPIO_PD11_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 11U))
#define GPIO_PD11_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 11U))
#define GPIO_PD11_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 11U))
#define GPIO_PD11_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 11U))
#define GPIO_PD11_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 11U))
#define GPIO_PD11_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 11U)) & 0x01U)
#define GPIO_PD11_PIN                  PORT_PIN_PD11

    /*** Macros for GPIO_PC10 pin ***/
#define GPIO_PC10_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 10U))
#define GPIO_PC10_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 10U))
#define GPIO_PC10_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 10U))
#define GPIO_PC10_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 10U))
#define GPIO_PC10_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 10U))
#define GPIO_PC10_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 10U)) & 0x01U)
#define GPIO_PC10_PIN                  PORT_PIN_PC10

    /*** Macros for GPIO_PC13 pin ***/
#define GPIO_PC13_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 13U))
#define GPIO_PC13_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 13U))
#define GPIO_PC13_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 13U))
#define GPIO_PC13_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 13U))
#define GPIO_PC13_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 13U))
#define GPIO_PC13_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 13U)) & 0x01U)
#define GPIO_PC13_PIN                  PORT_PIN_PC13

    /*** Macros for GPIO_PC14 pin ***/
#define GPIO_PC14_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 14U))
#define GPIO_PC14_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 14U))
#define GPIO_PC14_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 14U))
#define GPIO_PC14_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 14U))
#define GPIO_PC14_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 14U))
#define GPIO_PC14_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 14U)) & 0x01U)
#define GPIO_PC14_PIN                  PORT_PIN_PC14

    /*** Macros for GPIO_PC15 pin ***/
#define GPIO_PC15_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 15U))
#define GPIO_PC15_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 15U))
#define GPIO_PC15_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 15U))
#define GPIO_PC15_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 15U))
#define GPIO_PC15_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 15U))
#define GPIO_PC15_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 15U)) & 0x01U)
#define GPIO_PC15_PIN                  PORT_PIN_PC15

    /*** Macros for GPIO_PA16 pin ***/
#define GPIO_PA16_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 16U))
#define GPIO_PA16_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 16U))
#define GPIO_PA16_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 16U))
#define GPIO_PA16_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 16U))
#define GPIO_PA16_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 16U))
#define GPIO_PA16_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 16U)) & 0x01U)
#define GPIO_PA16_PIN                  PORT_PIN_PA16

    /*** Macros for GPIO_PC16 pin ***/
#define GPIO_PC16_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 16U))
#define GPIO_PC16_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 16U))
#define GPIO_PC16_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 16U))
#define GPIO_PC16_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 16U))
#define GPIO_PC16_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 16U))
#define GPIO_PC16_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 16U)) & 0x01U)
#define GPIO_PC16_PIN                  PORT_PIN_PC16

    /*** Macros for GPIO_PC17 pin ***/
#define GPIO_PC17_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 17U))
#define GPIO_PC17_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 17U))
#define GPIO_PC17_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 17U))
#define GPIO_PC17_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 17U))
#define GPIO_PC17_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 17U))
#define GPIO_PC17_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 17U)) & 0x01U)
#define GPIO_PC17_PIN                  PORT_PIN_PC17

    /*** Macros for GPIO_PC18 pin ***/
#define GPIO_PC18_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 18U))
#define GPIO_PC18_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 18U))
#define GPIO_PC18_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 18U))
#define GPIO_PC18_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 18U))
#define GPIO_PC18_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 18U))
#define GPIO_PC18_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 18U)) & 0x01U)
#define GPIO_PC18_PIN                  PORT_PIN_PC18

    /*** Macros for GPIO_PC19 pin ***/
#define GPIO_PC19_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 19U))
#define GPIO_PC19_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 19U))
#define GPIO_PC19_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 19U))
#define GPIO_PC19_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 19U))
#define GPIO_PC19_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 19U))
#define GPIO_PC19_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 19U)) & 0x01U)
#define GPIO_PC19_PIN                  PORT_PIN_PC19

    /*** Macros for GPIO_PC22 pin ***/
#define GPIO_PC22_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 22U))
#define GPIO_PC22_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 22U))
#define GPIO_PC22_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 22U))
#define GPIO_PC22_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 22U))
#define GPIO_PC22_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 22U))
#define GPIO_PC22_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 22U)) & 0x01U)
#define GPIO_PC22_PIN                  PORT_PIN_PC22

    /*** Macros for GPIO_PC23 pin ***/
#define GPIO_PC23_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 23U))
#define GPIO_PC23_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 23U))
#define GPIO_PC23_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 23U))
#define GPIO_PC23_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 23U))
#define GPIO_PC23_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 23U))
#define GPIO_PC23_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 23U)) & 0x01U)
#define GPIO_PC23_PIN                  PORT_PIN_PC23

    /*** Macros for GPIO_PD20 pin ***/
#define GPIO_PD20_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 20U))
#define GPIO_PD20_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 20U))
#define GPIO_PD20_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 20U))
#define GPIO_PD20_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 20U))
#define GPIO_PD20_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 20U))
#define GPIO_PD20_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 20U)) & 0x01U)
#define GPIO_PD20_PIN                  PORT_PIN_PD20

    /*** Macros for GPIO_PD21 pin ***/
#define GPIO_PD21_Set()               (PORT_REGS->GROUP[3].PORT_OUTSET = ((uint32_t)1U << 21U))
#define GPIO_PD21_Clear()             (PORT_REGS->GROUP[3].PORT_OUTCLR = ((uint32_t)1U << 21U))
#define GPIO_PD21_Toggle()            (PORT_REGS->GROUP[3].PORT_OUTTGL = ((uint32_t)1U << 21U))
#define GPIO_PD21_OutputEnable()      (PORT_REGS->GROUP[3].PORT_DIRSET = ((uint32_t)1U << 21U))
#define GPIO_PD21_InputEnable()       (PORT_REGS->GROUP[3].PORT_DIRCLR = ((uint32_t)1U << 21U))
#define GPIO_PD21_Get()               (((PORT_REGS->GROUP[3].PORT_IN >> 21U)) & 0x01U)
#define GPIO_PD21_PIN                  PORT_PIN_PD21

    /*** Macros for GPIO_PB16 pin ***/
#define GPIO_PB16_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 16U))
#define GPIO_PB16_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 16U))
#define GPIO_PB16_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 16U))
#define GPIO_PB16_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 16U))
#define GPIO_PB16_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 16U))
#define GPIO_PB16_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 16U)) & 0x01U)
#define GPIO_PB16_PIN                  PORT_PIN_PB16

    /*** Macros for GPIO_PB17 pin ***/
#define GPIO_PB17_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 17U))
#define GPIO_PB17_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 17U))
#define GPIO_PB17_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 17U))
#define GPIO_PB17_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 17U))
#define GPIO_PB17_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 17U))
#define GPIO_PB17_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 17U)) & 0x01U)
#define GPIO_PB17_PIN                  PORT_PIN_PB17

    /*** Macros for GPIO_PB18 pin ***/
#define GPIO_PB18_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 18U))
#define GPIO_PB18_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 18U))
#define GPIO_PB18_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 18U))
#define GPIO_PB18_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 18U))
#define GPIO_PB18_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 18U))
#define GPIO_PB18_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 18U)) & 0x01U)
#define GPIO_PB18_PIN                  PORT_PIN_PB18

    /*** Macros for GPIO_PB19 pin ***/
#define GPIO_PB19_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 19U))
#define GPIO_PB19_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 19U))
#define GPIO_PB19_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 19U))
#define GPIO_PB19_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 19U))
#define GPIO_PB19_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 19U))
#define GPIO_PB19_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 19U)) & 0x01U)
#define GPIO_PB19_PIN                  PORT_PIN_PB19

    /*** Macros for GPIO_PB20 pin ***/
#define GPIO_PB20_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 20U))
#define GPIO_PB20_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 20U))
#define GPIO_PB20_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 20U))
#define GPIO_PB20_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 20U))
#define GPIO_PB20_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 20U))
#define GPIO_PB20_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 20U)) & 0x01U)
#define GPIO_PB20_PIN                  PORT_PIN_PB20

    /*** Macros for GPIO_PB21 pin ***/
#define GPIO_PB21_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 21U))
#define GPIO_PB21_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 21U))
#define GPIO_PB21_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 21U))
#define GPIO_PB21_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 21U))
#define GPIO_PB21_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 21U))
#define GPIO_PB21_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 21U)) & 0x01U)
#define GPIO_PB21_PIN                  PORT_PIN_PB21

    /*** Macros for GPIO_PA20 pin ***/
#define GPIO_PA20_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 20U))
#define GPIO_PA20_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 20U))
#define GPIO_PA20_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 20U))
#define GPIO_PA20_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 20U))
#define GPIO_PA20_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 20U))
#define GPIO_PA20_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 20U)) & 0x01U)
#define GPIO_PA20_PIN                  PORT_PIN_PA20

    /*** Macros for GPIO_PA21 pin ***/
#define GPIO_PA21_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 21U))
#define GPIO_PA21_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 21U))
#define GPIO_PA21_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 21U))
#define GPIO_PA21_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 21U))
#define GPIO_PA21_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 21U))
#define GPIO_PA21_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 21U)) & 0x01U)
#define GPIO_PA21_PIN                  PORT_PIN_PA21

    /*** Macros for GPIO_PA22 pin ***/
#define GPIO_PA22_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 22U))
#define GPIO_PA22_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 22U))
#define GPIO_PA22_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 22U))
#define GPIO_PA22_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 22U))
#define GPIO_PA22_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 22U))
#define GPIO_PA22_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 22U)) & 0x01U)
#define GPIO_PA22_PIN                  PORT_PIN_PA22

    /*** Macros for GPIO_PA23 pin ***/
#define GPIO_PA23_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 23U))
#define GPIO_PA23_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 23U))
#define GPIO_PA23_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 23U))
#define GPIO_PA23_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 23U))
#define GPIO_PA23_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 23U))
#define GPIO_PA23_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 23U)) & 0x01U)
#define GPIO_PA23_PIN                  PORT_PIN_PA23

    /*** Macros for GPIO_PA24 pin ***/
#define GPIO_PA24_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 24U))
#define GPIO_PA24_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 24U))
#define GPIO_PA24_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 24U))
#define GPIO_PA24_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 24U))
#define GPIO_PA24_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 24U))
#define GPIO_PA24_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 24U)) & 0x01U)
#define GPIO_PA24_PIN                  PORT_PIN_PA24

    /*** Macros for GPIO_PA25 pin ***/
#define GPIO_PA25_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 25U))
#define GPIO_PA25_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 25U))
#define GPIO_PA25_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 25U))
#define GPIO_PA25_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 25U))
#define GPIO_PA25_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 25U))
#define GPIO_PA25_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 25U)) & 0x01U)
#define GPIO_PA25_PIN                  PORT_PIN_PA25

    /*** Macros for GPIO_PB24 pin ***/
#define GPIO_PB24_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 24U))
#define GPIO_PB24_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 24U))
#define GPIO_PB24_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 24U))
#define GPIO_PB24_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 24U))
#define GPIO_PB24_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 24U))
#define GPIO_PB24_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 24U)) & 0x01U)
#define GPIO_PB24_PIN                  PORT_PIN_PB24

    /*** Macros for GPIO_PB25 pin ***/
#define GPIO_PB25_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 25U))
#define GPIO_PB25_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 25U))
#define GPIO_PB25_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 25U))
#define GPIO_PB25_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 25U))
#define GPIO_PB25_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 25U))
#define GPIO_PB25_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 25U)) & 0x01U)
#define GPIO_PB25_PIN                  PORT_PIN_PB25

    /*** Macros for GPIO_PB26 pin ***/
#define GPIO_PB26_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 26U))
#define GPIO_PB26_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 26U))
#define GPIO_PB26_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 26U))
#define GPIO_PB26_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 26U))
#define GPIO_PB26_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 26U))
#define GPIO_PB26_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 26U)) & 0x01U)
#define GPIO_PB26_PIN                  PORT_PIN_PB26

    /*** Macros for GPIO_PB27 pin ***/
#define GPIO_PB27_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 27U))
#define GPIO_PB27_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 27U))
#define GPIO_PB27_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 27U))
#define GPIO_PB27_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 27U))
#define GPIO_PB27_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 27U))
#define GPIO_PB27_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 27U)) & 0x01U)
#define GPIO_PB27_PIN                  PORT_PIN_PB27

    /*** Macros for GPIO_PB28 pin ***/
#define GPIO_PB28_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 28U))
#define GPIO_PB28_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 28U))
#define GPIO_PB28_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 28U))
#define GPIO_PB28_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 28U))
#define GPIO_PB28_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 28U))
#define GPIO_PB28_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 28U)) & 0x01U)
#define GPIO_PB28_PIN                  PORT_PIN_PB28

    /*** Macros for GPIO_PB29 pin ***/
#define GPIO_PB29_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 29U))
#define GPIO_PB29_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 29U))
#define GPIO_PB29_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 29U))
#define GPIO_PB29_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 29U))
#define GPIO_PB29_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 29U))
#define GPIO_PB29_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 29U)) & 0x01U)
#define GPIO_PB29_PIN                  PORT_PIN_PB29

    /*** Macros for GPIO_PC24 pin ***/
#define GPIO_PC24_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 24U))
#define GPIO_PC24_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 24U))
#define GPIO_PC24_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 24U))
#define GPIO_PC24_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 24U))
#define GPIO_PC24_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 24U))
#define GPIO_PC24_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 24U)) & 0x01U)
#define GPIO_PC24_PIN                  PORT_PIN_PC24

    /*** Macros for GPIO_PC25 pin ***/
#define GPIO_PC25_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 25U))
#define GPIO_PC25_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 25U))
#define GPIO_PC25_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 25U))
#define GPIO_PC25_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 25U))
#define GPIO_PC25_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 25U))
#define GPIO_PC25_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 25U)) & 0x01U)
#define GPIO_PC25_PIN                  PORT_PIN_PC25

    /*** Macros for GPIO_PC26 pin ***/
#define GPIO_PC26_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 26U))
#define GPIO_PC26_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 26U))
#define GPIO_PC26_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 26U))
#define GPIO_PC26_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 26U))
#define GPIO_PC26_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 26U))
#define GPIO_PC26_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 26U)) & 0x01U)
#define GPIO_PC26_PIN                  PORT_PIN_PC26

    /*** Macros for GPIO_PC27 pin ***/
#define GPIO_PC27_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 27U))
#define GPIO_PC27_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 27U))
#define GPIO_PC27_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 27U))
#define GPIO_PC27_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 27U))
#define GPIO_PC27_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 27U))
#define GPIO_PC27_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 27U)) & 0x01U)
#define GPIO_PC27_PIN                  PORT_PIN_PC27

    /*** Macros for GPIO_PC28 pin ***/
#define GPIO_PC28_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 28U))
#define GPIO_PC28_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 28U))
#define GPIO_PC28_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 28U))
#define GPIO_PC28_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 28U))
#define GPIO_PC28_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 28U))
#define GPIO_PC28_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 28U)) & 0x01U)
#define GPIO_PC28_PIN                  PORT_PIN_PC28

    /*** Macros for GPIO_PA27 pin ***/
#define GPIO_PA27_Set()               (PORT_REGS->GROUP[0].PORT_OUTSET = ((uint32_t)1U << 27U))
#define GPIO_PA27_Clear()             (PORT_REGS->GROUP[0].PORT_OUTCLR = ((uint32_t)1U << 27U))
#define GPIO_PA27_Toggle()            (PORT_REGS->GROUP[0].PORT_OUTTGL = ((uint32_t)1U << 27U))
#define GPIO_PA27_OutputEnable()      (PORT_REGS->GROUP[0].PORT_DIRSET = ((uint32_t)1U << 27U))
#define GPIO_PA27_InputEnable()       (PORT_REGS->GROUP[0].PORT_DIRCLR = ((uint32_t)1U << 27U))
#define GPIO_PA27_Get()               (((PORT_REGS->GROUP[0].PORT_IN >> 27U)) & 0x01U)
#define GPIO_PA27_PIN                  PORT_PIN_PA27

    /*** Macros for GPIO_PB31 pin ***/
#define GPIO_PB31_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 31U))
#define GPIO_PB31_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 31U))
#define GPIO_PB31_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 31U))
#define GPIO_PB31_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 31U))
#define GPIO_PB31_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 31U))
#define GPIO_PB31_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 31U)) & 0x01U)
#define GPIO_PB31_PIN                  PORT_PIN_PB31

    /*** Macros for GPIO_PC30 pin ***/
#define GPIO_PC30_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 30U))
#define GPIO_PC30_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 30U))
#define GPIO_PC30_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 30U))
#define GPIO_PC30_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 30U))
#define GPIO_PC30_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 30U))
#define GPIO_PC30_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 30U)) & 0x01U)
#define GPIO_PC30_PIN                  PORT_PIN_PC30

    /*** Macros for GPIO_PC31 pin ***/
#define GPIO_PC31_Set()               (PORT_REGS->GROUP[2].PORT_OUTSET = ((uint32_t)1U << 31U))
#define GPIO_PC31_Clear()             (PORT_REGS->GROUP[2].PORT_OUTCLR = ((uint32_t)1U << 31U))
#define GPIO_PC31_Toggle()            (PORT_REGS->GROUP[2].PORT_OUTTGL = ((uint32_t)1U << 31U))
#define GPIO_PC31_OutputEnable()      (PORT_REGS->GROUP[2].PORT_DIRSET = ((uint32_t)1U << 31U))
#define GPIO_PC31_InputEnable()       (PORT_REGS->GROUP[2].PORT_DIRCLR = ((uint32_t)1U << 31U))
#define GPIO_PC31_Get()               (((PORT_REGS->GROUP[2].PORT_IN >> 31U)) & 0x01U)
#define GPIO_PC31_PIN                  PORT_PIN_PC31

    /*** Macros for GPIO_PB00 pin ***/
#define GPIO_PB00_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 0U))
#define GPIO_PB00_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 0U))
#define GPIO_PB00_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 0U))
#define GPIO_PB00_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 0U))
#define GPIO_PB00_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 0U))
#define GPIO_PB00_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 0U)) & 0x01U)
#define GPIO_PB00_PIN                  PORT_PIN_PB00

    /*** Macros for GPIO_PB01 pin ***/
#define GPIO_PB01_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 1U))
#define GPIO_PB01_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 1U))
#define GPIO_PB01_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 1U))
#define GPIO_PB01_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 1U))
#define GPIO_PB01_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 1U))
#define GPIO_PB01_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 1U)) & 0x01U)
#define GPIO_PB01_PIN                  PORT_PIN_PB01

    /*** Macros for GPIO_PB02 pin ***/
#define GPIO_PB02_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 2U))
#define GPIO_PB02_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 2U))
#define GPIO_PB02_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 2U))
#define GPIO_PB02_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 2U))
#define GPIO_PB02_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 2U))
#define GPIO_PB02_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 2U)) & 0x01U)
#define GPIO_PB02_PIN                  PORT_PIN_PB02

    /*** Macros for GPIO_PB03 pin ***/
#define GPIO_PB03_Set()               (PORT_REGS->GROUP[1].PORT_OUTSET = ((uint32_t)1U << 3U))
#define GPIO_PB03_Clear()             (PORT_REGS->GROUP[1].PORT_OUTCLR = ((uint32_t)1U << 3U))
#define GPIO_PB03_Toggle()            (PORT_REGS->GROUP[1].PORT_OUTTGL = ((uint32_t)1U << 3U))
#define GPIO_PB03_OutputEnable()      (PORT_REGS->GROUP[1].PORT_DIRSET = ((uint32_t)1U << 3U))
#define GPIO_PB03_InputEnable()       (PORT_REGS->GROUP[1].PORT_DIRCLR = ((uint32_t)1U << 3U))
#define GPIO_PB03_Get()               (((PORT_REGS->GROUP[1].PORT_IN >> 3U)) & 0x01U)
#define GPIO_PB03_PIN                  PORT_PIN_PB03

    // *****************************************************************************
    /* PORT Group

      Summary:
        Identifies the port groups available on the device.

      Description:
        These macros identifies all the ports groups that are available on this
        device.

      Remarks:
        The caller should not use the constant expressions assigned to any of
        the preprocessor macros as these may vary between devices.

        Port groups shown here are the ones available on the selected device. Not
        all ports groups are implemented. Refer to the device specific datasheet
        for more details. The MHC will generate these macros with the port
        groups that are available on the device.
     */

    /* Group 0 */
#define PORT_GROUP_0 (PORT_BASE_ADDRESS + (0U * 0x80U))

    /* Group 1 */
#define PORT_GROUP_1 (PORT_BASE_ADDRESS + (1U * 0x80U))

    /* Group 2 */
#define PORT_GROUP_2 (PORT_BASE_ADDRESS + (2U * 0x80U))

    /* Group 3 */
#define PORT_GROUP_3 (PORT_BASE_ADDRESS + (3U * 0x80U))


    /* Helper macros to get port information from the pin */
#define GET_PORT_GROUP(pin)  ((PORT_GROUP)(PORT_BASE_ADDRESS + (0x80U * (((uint32_t)pin) >> 5U))))
#define GET_PIN_MASK(pin)   (((uint32_t)(0x1U)) << (((uint32_t)pin) & 0x1FU))

    /* Named type for port group */
    typedef uint32_t PORT_GROUP;

    typedef enum {
        PERIPHERAL_FUNCTION_A = 0x0,
        PERIPHERAL_FUNCTION_B = 0x1,
        PERIPHERAL_FUNCTION_C = 0x2,
        PERIPHERAL_FUNCTION_D = 0x3,
        PERIPHERAL_FUNCTION_E = 0x4,
        PERIPHERAL_FUNCTION_F = 0x5,
        PERIPHERAL_FUNCTION_G = 0x6,
        PERIPHERAL_FUNCTION_H = 0x7,
        PERIPHERAL_FUNCTION_I = 0x8,
        PERIPHERAL_FUNCTION_J = 0x9,
        PERIPHERAL_FUNCTION_K = 0xA,
        PERIPHERAL_FUNCTION_L = 0xB,
        PERIPHERAL_FUNCTION_M = 0xC,
        PERIPHERAL_FUNCTION_N = 0xD,
        PERIPHERAL_FUNCTION_DEFAULT = 0x1

    } PERIPHERAL_FUNCTION;

    // *****************************************************************************

    /* PORT Pins

      Summary:
        Identifies the available Ports pins.

      Description:
        This enumeration identifies all the ports pins that are available on this
        device.

      Remarks:
        The caller should not use the constant expressions assigned to any of
        the enumeration constants as these may vary between devices.

        Port pins shown here are the ones available on the selected device. Not
        all ports pins within a port group are implemented. Refer to the device
        specific datasheet for more details.
     */

    typedef enum {
        /* PA00 pin */
        PORT_PIN_PA00 = 0U,

        /* PA01 pin */
        PORT_PIN_PA01 = 1U,

        /* PA02 pin */
        PORT_PIN_PA02 = 2U,

        /* PA03 pin */
        PORT_PIN_PA03 = 3U,

        /* PA04 pin */
        PORT_PIN_PA04 = 4U,

        /* PA05 pin */
        PORT_PIN_PA05 = 5U,

        /* PA06 pin */
        PORT_PIN_PA06 = 6U,

        /* PA07 pin */
        PORT_PIN_PA07 = 7U,

        /* PA08 pin */
        PORT_PIN_PA08 = 8U,

        /* PA09 pin */
        PORT_PIN_PA09 = 9U,

        /* PA10 pin */
        PORT_PIN_PA10 = 10U,

        /* PA11 pin */
        PORT_PIN_PA11 = 11U,

        /* PA12 pin */
        PORT_PIN_PA12 = 12U,

        /* PA13 pin */
        PORT_PIN_PA13 = 13U,

        /* PA14 pin */
        PORT_PIN_PA14 = 14U,

        /* PA15 pin */
        PORT_PIN_PA15 = 15U,

        /* PA16 pin */
        PORT_PIN_PA16 = 16U,

        /* PA17 pin */
        PORT_PIN_PA17 = 17U,

        /* PA18 pin */
        PORT_PIN_PA18 = 18U,

        /* PA19 pin */
        PORT_PIN_PA19 = 19U,

        /* PA20 pin */
        PORT_PIN_PA20 = 20U,

        /* PA21 pin */
        PORT_PIN_PA21 = 21U,

        /* PA22 pin */
        PORT_PIN_PA22 = 22U,

        /* PA23 pin */
        PORT_PIN_PA23 = 23U,

        /* PA24 pin */
        PORT_PIN_PA24 = 24U,

        /* PA25 pin */
        PORT_PIN_PA25 = 25U,

        /* PA27 pin */
        PORT_PIN_PA27 = 27U,

        /* PA30 pin */
        PORT_PIN_PA30 = 30U,

        /* PA31 pin */
        PORT_PIN_PA31 = 31U,

        /* PB00 pin */
        PORT_PIN_PB00 = 32U,

        /* PB01 pin */
        PORT_PIN_PB01 = 33U,

        /* PB02 pin */
        PORT_PIN_PB02 = 34U,

        /* PB03 pin */
        PORT_PIN_PB03 = 35U,

        /* PB04 pin */
        PORT_PIN_PB04 = 36U,

        /* PB05 pin */
        PORT_PIN_PB05 = 37U,

        /* PB06 pin */
        PORT_PIN_PB06 = 38U,

        /* PB07 pin */
        PORT_PIN_PB07 = 39U,

        /* PB08 pin */
        PORT_PIN_PB08 = 40U,

        /* PB09 pin */
        PORT_PIN_PB09 = 41U,

        /* PB10 pin */
        PORT_PIN_PB10 = 42U,

        /* PB11 pin */
        PORT_PIN_PB11 = 43U,

        /* PB12 pin */
        PORT_PIN_PB12 = 44U,

        /* PB13 pin */
        PORT_PIN_PB13 = 45U,

        /* PB14 pin */
        PORT_PIN_PB14 = 46U,

        /* PB15 pin */
        PORT_PIN_PB15 = 47U,

        /* PB16 pin */
        PORT_PIN_PB16 = 48U,

        /* PB17 pin */
        PORT_PIN_PB17 = 49U,

        /* PB18 pin */
        PORT_PIN_PB18 = 50U,

        /* PB19 pin */
        PORT_PIN_PB19 = 51U,

        /* PB20 pin */
        PORT_PIN_PB20 = 52U,

        /* PB21 pin */
        PORT_PIN_PB21 = 53U,

        /* PB22 pin */
        PORT_PIN_PB22 = 54U,

        /* PB23 pin */
        PORT_PIN_PB23 = 55U,

        /* PB24 pin */
        PORT_PIN_PB24 = 56U,

        /* PB25 pin */
        PORT_PIN_PB25 = 57U,

        /* PB26 pin */
        PORT_PIN_PB26 = 58U,

        /* PB27 pin */
        PORT_PIN_PB27 = 59U,

        /* PB28 pin */
        PORT_PIN_PB28 = 60U,

        /* PB29 pin */
        PORT_PIN_PB29 = 61U,

        /* PB30 pin */
        PORT_PIN_PB30 = 62U,

        /* PB31 pin */
        PORT_PIN_PB31 = 63U,

        /* PC00 pin */
        PORT_PIN_PC00 = 64U,

        /* PC01 pin */
        PORT_PIN_PC01 = 65U,

        /* PC02 pin */
        PORT_PIN_PC02 = 66U,

        /* PC03 pin */
        PORT_PIN_PC03 = 67U,

        /* PC04 pin */
        PORT_PIN_PC04 = 68U,

        /* PC05 pin */
        PORT_PIN_PC05 = 69U,

        /* PC06 pin */
        PORT_PIN_PC06 = 70U,

        /* PC07 pin */
        PORT_PIN_PC07 = 71U,

        /* PC10 pin */
        PORT_PIN_PC10 = 74U,

        /* PC11 pin */
        PORT_PIN_PC11 = 75U,

        /* PC12 pin */
        PORT_PIN_PC12 = 76U,

        /* PC13 pin */
        PORT_PIN_PC13 = 77U,

        /* PC14 pin */
        PORT_PIN_PC14 = 78U,

        /* PC15 pin */
        PORT_PIN_PC15 = 79U,

        /* PC16 pin */
        PORT_PIN_PC16 = 80U,

        /* PC17 pin */
        PORT_PIN_PC17 = 81U,

        /* PC18 pin */
        PORT_PIN_PC18 = 82U,

        /* PC19 pin */
        PORT_PIN_PC19 = 83U,

        /* PC20 pin */
        PORT_PIN_PC20 = 84U,

        /* PC21 pin */
        PORT_PIN_PC21 = 85U,

        /* PC22 pin */
        PORT_PIN_PC22 = 86U,

        /* PC23 pin */
        PORT_PIN_PC23 = 87U,

        /* PC24 pin */
        PORT_PIN_PC24 = 88U,

        /* PC25 pin */
        PORT_PIN_PC25 = 89U,

        /* PC26 pin */
        PORT_PIN_PC26 = 90U,

        /* PC27 pin */
        PORT_PIN_PC27 = 91U,

        /* PC28 pin */
        PORT_PIN_PC28 = 92U,

        /* PC30 pin */
        PORT_PIN_PC30 = 94U,

        /* PC31 pin */
        PORT_PIN_PC31 = 95U,

        /* PD00 pin */
        PORT_PIN_PD00 = 96U,

        /* PD01 pin */
        PORT_PIN_PD01 = 97U,

        /* PD08 pin */
        PORT_PIN_PD08 = 104U,

        /* PD09 pin */
        PORT_PIN_PD09 = 105U,

        /* PD10 pin */
        PORT_PIN_PD10 = 106U,

        /* PD11 pin */
        PORT_PIN_PD11 = 107U,

        /* PD12 pin */
        PORT_PIN_PD12 = 108U,

        /* PD20 pin */
        PORT_PIN_PD20 = 116U,

        /* PD21 pin */
        PORT_PIN_PD21 = 117U,

        /* This element should not be used in any of the PORT APIs.
         * It will be used by other modules or application to denote that none of
         * the PORT Pin is used */
        PORT_PIN_NONE = 65535U,

    } PORT_PIN;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Generated API based on pin configurations done in Pin Manager
    // *****************************************************************************
    // *****************************************************************************
    // *****************************************************************************
    /* Function:
        void PORT_Initialize(void)

      Summary:
        Initializes the PORT Library.

      Description:
        This function initializes all ports and pins as configured in the
        MHC Pin Manager.

      Precondition:
        None.

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>

        PORT_Initialize();

        </code>

      Remarks:
        The function should be called once before calling any other PORTS PLIB
        functions.
     */

    void PORT_Initialize(void);

    // *****************************************************************************
    // *****************************************************************************
    // Section: PORT APIs which operates on multiple pins of a group
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************
    /* Function:
        uint32_t PORT_GroupRead(PORT_GROUP group)

      Summary:
        Read all the I/O pins in the specified port group.

      Description:
        The function reads the hardware pin state of all pins in the specified group
        and returns this as a 32 bit value. Each bit in the 32 bit value represent a
        pin. For example, bit 0 in group 0 will represent pin PA0. Bit 1 will
        represent PA1 and so on. The application should only consider the value of
        the port group pins which are implemented on the device.

      Precondition:
        The PORT_Initialize() function should have been called. Input buffer
        (INEN bit in the Pin Configuration register) should be enabled in MHC.

      Parameters:
        group - One of the IO groups from the enum PORT_GROUP.

      Returns:
        A 32-bit value representing the hardware state of of all the I/O pins in the
        selected port group.

      Example:
        <code>

        uint32_t value;
        value = PORT_Read(PORT_GROUP_C);

        </code>

      Remarks:
        None.
     */

    uint32_t PORT_GroupRead(PORT_GROUP group);

    // *****************************************************************************
    /* Function:
        uint32_t PORT_GroupLatchRead(PORT_GROUP group)

      Summary:
        Read the data driven on all the I/O pins of the selected port group.

      Description:
        The function will return a 32-bit value representing the logic levels being
        driven on the output pins within the group. The function will not sample the
        actual hardware state of the output pin. Each bit in the 32-bit return value
        will represent one of the 32 port pins within the group. The application
        should only consider the value of the pins which are available on the
        device.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One of the IO groups from the enum PORT_GROUP.

      Returns:
        A 32-bit value representing the output state of of all the I/O pins in the
        selected port group.

      Example:
        <code>

        uint32_t value;
        value = PORT_GroupLatchRead(PORT_GROUP_C);

        </code>

      Remarks:
        None.
     */

    uint32_t PORT_GroupLatchRead(PORT_GROUP group);

    // *****************************************************************************
    /* Function:
        void PORT_GroupWrite(PORT_GROUP group, uint32_t mask, uint32_t value);

      Summary:
        Write value on the masked pins of the selected port group.

      Description:
        This function writes the value contained in the value parameter to the
        port group. Port group pins which are configured for output will be updated.
        The mask parameter provides additional control on the bits in the group to
        be affected. Setting a bit to 1 in the mask will cause the corresponding
        bit in the port group to be updated. Clearing a bit in the mask will cause
        that corresponding bit in the group to stay unaffected. For example,
        setting a mask value 0xFFFFFFFF will cause all bits in the port group
        to be updated. Setting a value 0x3 will only cause port group bit 0 and
        bit 1 to be updated.

        For port pins which are not configured for output and have the pull feature
        enabled, this function will affect pull value (pull up or pull down). A bit
        value of 1 will enable the pull up. A bit value of 0 will enable the pull
        down.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One of the IO groups from the enum PORT_GROUP.

        mask  - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port group will be written.
                 1's - Will write to corresponding IO pins.
                 0's - Will remain unchanged.

        value - Value which has to be written/driven on the I/O
                 lines of the selected port for which mask bits are '1'.
                 Values for the corresponding mask bit '0' will be ignored.
                 Refer to the function description for effect on pins
                 which are not configured for output.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupWrite(PORT_GROUP_C, 0x0F, 0xF563D453);

        </code>

      Remarks:
        None.
     */

    void PORT_GroupWrite(PORT_GROUP group, uint32_t mask, uint32_t value);

    // *****************************************************************************
    /* Function:
        void PORT_GroupSet(PORT_GROUP group, uint32_t mask)

      Summary:
        Set the selected IO pins of a group.

      Description:
        This function sets (drives a logic high) on the selected output pins of a
        group. The mask parameter control the pins to be updated. A mask bit
        position with a value 1 will cause that corresponding port pin to be set. A
        mask bit position with a value 0 will cause the corresponding port pin to
        stay un-affected.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One of the IO ports from the enum PORT_GROUP.
        mask - A 32 bit value in which a bit represent a pin in the group. If the
        value of the bit is 1, the corresponding port pin will driven to logic 1. If
        the value of the bit is 0. the corresponding port pin will stay un-affected.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupSet(PORT_GROUP_C, 0x00A0);

        </code>

      Remarks:
        If the port pin within the the group is not configured for output and has
        the pull feature enabled, driving a logic 1 on this pin will cause the pull
        up to be enabled.
     */

    void PORT_GroupSet(PORT_GROUP group, uint32_t mask);

    // *****************************************************************************
    /* Function:
        void PORT_GroupClear(PORT_GROUP group, uint32_t mask)

      Summary:
        Clears the selected IO pins of a group.

      Description:
        This function clears (drives a logic 0) on the selected output pins of a
        group. The mask parameter control the pins to be updated. A mask bit
        position with a value 1 will cause that corresponding port pin to be clear.
        A mask bit position with a value 0 will cause the corresponding port pin to
        stay un-affected.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One of the IO ports from the enum PORT_GROUP.
        mask - A 32 bit value in which a bit represent a pin in the group. If the
        value of the bit is 1, the corresponding port pin will driven to logic 0. If
        the value of the bit is 0. the corresponding port pin will stay un-affected.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupClear(PORT_GROUP_C, 0x00A0);

        </code>

      Remarks:
        If the port pin within the the group is not configured for output and has
        the pull feature enabled, driving a logic 0 on this pin will cause the pull
        down to be enabled.
     */

    void PORT_GroupClear(PORT_GROUP group, uint32_t mask);

    // *****************************************************************************
    /* Function:
        void PORT_GroupToggle(PORT_GROUP group, uint32_t mask)

      Summary:
        Toggles the selected IO pins of a group.

      Description:
        This function toggles the selected output pins of a group. The mask
        parameter control the pins to be updated. A mask bit position with a value 1
        will cause that corresponding port pin to be toggled.  A mask bit position
        with a value 0 will cause the corresponding port pin to stay un-affected.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One of the IO ports from the enum PORT_GROUP.
        mask - A 32 bit value in which a bit represent a pin in the group. If the
        value of the bit is 1, the corresponding port pin will be toggled. If the
        value of the bit is 0. the corresponding port pin will stay un-affected.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupToggle(PORT_GROUP_C, 0x00A0);

        </code>

      Remarks:
        If the port pin within the the group is not configured for output and has
        the pull feature enabled, driving a logic 0 on this pin will cause the pull
        down to be enabled. Driving a logic 1 on this pin will cause the pull up to
        be enabled.
     */

    void PORT_GroupToggle(PORT_GROUP group, uint32_t mask);

    // *****************************************************************************
    /* Function:
        void PORT_GroupInputEnable(PORT_GROUP group, uint32_t mask)

      Summary:
        Configures the selected IO pins of a group as input.

      Description:
        This function configures the selected IO pins of a group as input. The pins
        to be configured as input are selected by setting the corresponding bits in
        the mask parameter to 1.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One or more of the of the IO ports from the enum PORT_GROUP.
        mask - A 32 bit value in which a bit represents a pin in the group. If the
        value of the bit is 1, the corresponding port pin will be configured as
        input. If the value of the bit is 0. the corresponding port pin will stay
        un-affected.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupInputEnable(PORT_GROUP_C, 0x00A0);

        </code>

      Remarks:
       None.
     */

    void PORT_GroupInputEnable(PORT_GROUP group, uint32_t mask);

    // *****************************************************************************
    /* Function:
        void PORT_GroupOutputEnable(PORT_GROUP group, uint32_t mask)

      Summary:
        Configures the selected IO pins of a group as output.

      Description:
        This function configures the selected IO pins of a group as output. The pins
        to be configured as output are selected by setting the corresponding bits in
        the mask parameter to 1.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        group - One or more of the of the IO ports from the enum PORT_GROUP.
        mask - A 32 bit value in which a bit represents a pin in the group. If the
        value of the bit is 1, the corresponding port pin will be configured as
        output. If the value of the bit is 0. the corresponding port pin will stay
        un-affected.

      Returns:
        None.

      Example:
        <code>

        PORT_GroupOutputEnable(PORT_GROUP_C, 0x00A0);

        </code>

      Remarks:
        None.
     */

    void PORT_GroupOutputEnable(PORT_GROUP group, uint32_t mask);

    // *****************************************************************************
    /* Function:
        void PORT_PinPeripheralFunctionConfig(PORT_PIN pin, PERIPHERAL_FUNCTION function)

      Summary:
        Configures the peripheral function on the selected port pin

      Description:
        This function configures the selected peripheral function on the given port pin.

      Remarks:
        None
     */
    void PORT_PinPeripheralFunctionConfig(PORT_PIN pin, PERIPHERAL_FUNCTION function);

    // *****************************************************************************
    /* Function:
        void PORT_PinGPIOConfig(PORT_PIN pin)

      Summary:
        Configures the selected pin as GPIO

      Description:
        This function configures the given pin as GPIO.

      Remarks:
        None
     */
    void PORT_PinGPIOConfig(PORT_PIN pin);

    // *****************************************************************************
    // *****************************************************************************
    // Section: PORT APIs which operates on one pin at a time
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************

    /* Function:
        void PORT_PinWrite(PORT_PIN pin, bool value)

      Summary:
        Writes the specified value to the selected pin.

      Description:
        This function writes/drives the "value" on the selected I/O line/pin.

      Precondition:
        The PORT_Initialize() function should have been called once.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.
        value - value to be written on the selected pin.
                true  = set pin to high (1).
                false = clear pin to low (0).

      Returns:
        None.

      Example:
        <code>

        bool value = true;
        PORT_PinWrite(PORT_PIN_PB3, value);

        </code>

      Remarks:
        Calling this function with an input pin with the pull-up/pull-down feature
        enabled will affect the pull-up/pull-down configuration. If the value is
        false, the pull-down will be enabled. If the value is true, the pull-up will
        be enabled.
     */

    static inline void PORT_PinWrite(PORT_PIN pin, bool value) {
        PORT_GroupWrite(GET_PORT_GROUP(pin),
                GET_PIN_MASK(pin),
                (value ? GET_PIN_MASK(pin) : 0U));
    }


    // *****************************************************************************

    /* Function:
        bool PORT_PinRead(PORT_PIN pin)

      Summary:
        Read the selected pin value.

      Description:
        This function reads the present state at the selected input pin.  The
        function can also be called to read the value of an output pin if input
        sampling on the output pin is enabled in MHC. If input synchronization on
        the pin is disabled in MHC, the function will cause a 2 PORT Clock cycles
        delay. Enabling the synchronization eliminates the delay but will increase
        power consumption.

      Precondition:
        The PORT_Initialize() function should have been called. Input buffer
        (INEN bit in the Pin Configuration register) should be enabled in MHC.

      Parameters:
        pin - the port pin whose state needs to be read.

      Returns:
        true - the state at the pin is a logic high.
        false - the state at the pin is a logic low.

      Example:
        <code>

        bool value;
        value = PORT_PinRead(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline bool PORT_PinRead(PORT_PIN pin) {
        return ((PORT_GroupRead(GET_PORT_GROUP(pin)) & GET_PIN_MASK(pin)) != 0U);
    }


    // *****************************************************************************

    /* Function:
        bool PORT_PinLatchRead(PORT_PIN pin)

      Summary:
        Read the value driven on the selected pin.

      Description:
        This function reads the data driven on the selected I/O line/pin. The
        function does not sample the state of the hardware pin. It only returns the
        value that is written to output register. Refer to the PORT_PinRead()
        function if the state of the output pin needs to be read.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        true - the present value in the output latch is a logic high.
        false - the present value in the output latch is a logic low.

      Example:
        <code>

        bool value;
        value = PORT_PinLatchRead(PORT_PIN_PB3);

        </code>

      Remarks:
        To read actual pin value, PIN_Read API should be used.
     */

    static inline bool PORT_PinLatchRead(PORT_PIN pin) {
        return ((PORT_GroupLatchRead(GET_PORT_GROUP(pin)) & GET_PIN_MASK(pin)) != 0U);
    }


    // *****************************************************************************

    /* Function:
        void PORT_PinToggle(PORT_PIN pin)

      Summary:
        Toggles the selected pin.

      Description:
        This function toggles/inverts the present value on the selected I/O line/pin.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        None.

      Example:
        <code>

        PORT_PinToggle(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline void PORT_PinToggle(PORT_PIN pin) {
        PORT_GroupToggle(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
    }


    // *****************************************************************************

    /* Function:
        void PORT_PinSet(PORT_PIN pin)

      Summary:
        Sets the selected pin.

      Description:
        This function drives a logic 1 on the selected I/O line/pin.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        None.

      Example:
        <code>

        PORT_PinSet(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline void PORT_PinSet(PORT_PIN pin) {
        PORT_GroupSet(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
    }


    // *****************************************************************************

    /* Function:
        void PORT_PinClear(PORT_PIN pin)

      Summary:
        Clears the selected pin.

      Description:
        This function drives a logic 0 on the selected I/O line/pin.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        None.

      Example:
        <code>

        PORT_PinClear(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline void PORT_PinClear(PORT_PIN pin) {
        PORT_GroupClear(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
    }


    // *****************************************************************************

    /* Function:
        void PORT_PinInputEnable(PORT_PIN pin)

      Summary:
        Configures the selected IO pin as input.

      Description:
        This function configures the selected IO pin as input. This function
        override the MHC input output pin settings.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        None.

      Example:
        <code>

        PORT_PinInputEnable(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline void PORT_PinInputEnable(PORT_PIN pin) {
        PORT_GroupInputEnable(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
    }


    // *****************************************************************************

    /* Function:
        void PORT_PinOutputEnable(PORT_PIN pin)

      Summary:
        Enables selected IO pin as output.

      Description:
        This function enables selected IO pin as output. Calling this function will
        override the MHC input output pin configuration.

      Precondition:
        The PORT_Initialize() function should have been called.

      Parameters:
        pin - One of the IO pins from the enum PORT_PIN.

      Returns:
        None.

      Example:
        <code>

        PORT_PinOutputEnable(PORT_PIN_PB3);

        </code>

      Remarks:
        None.
     */

    static inline void PORT_PinOutputEnable(PORT_PIN pin) {
        PORT_GroupOutputEnable(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
    }

    // DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif
// DOM-IGNORE-END
#endif // PLIB_PORT_H
