/*
 * hw_config.h - some definitions of hardware configuration, be careful:
 *      interrupts.c also have hardware-dependent parts!
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once
#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

#include "stm32f10x.h"

// RCC
void RCC_Config(void);
// packet size for data to be sent to USB
#define USART_RX_DATA_SIZE   512

void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

void Set_USBClock(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);

void Handle_USBAsynchXfer (void);
void USB_Send_Data(uint8_t data);

void Ports_Config(void);

#endif // __HW_CONFIG_H__
