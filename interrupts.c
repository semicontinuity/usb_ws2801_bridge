/*
 * interrupts.c - interrupts handlers
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

#include "includes.h"

void NMI_Handler(){}
void HardFault_Handler(){while (1){};}
void MemManage_Handler(){while (1){};}
void BusFault_Handler(){while (1){};}
void UsageFault_Handler(){while (1){};}
void SVC_Handler(){}
void DebugMon_Handler(){}
void PendSV_Handler(){}
void SDIO_IRQHandler(){}
void TIM1_CC_IRQHandler() {}
void SysTick_Handler(){}
void TIM2_IRQHandler(){}

/*
 * peripherial interrupts
 */

void EXTI0_IRQHandler(){}
void EXTI2_IRQHandler(){}

/*
 * USB interrupts
 */

// USB Low Priority / CAN RX0
void USB_LP_CAN1_RX0_IRQHandler(){
    USB_Istr();
}

// Start Of Frame callback (11-bit frame number)
// we'll flush data in USB TX buffer every IN_FRAME_INTERVAL frames
void SOF_Callback(){
}
