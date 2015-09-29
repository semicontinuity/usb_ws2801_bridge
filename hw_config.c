/*
 * hw_config.c - USB & USART configuration
 *
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

#define USART1_IRQHandler Dummy
#define USART2_IRQHandler Dummy
#define USART3_IRQHandler Dummy
void Dummy(){};


/**
 * Configure GPIO ports (all pins have 2MHz speed)
 * @param mode  - port mode
 * @param GPIOx - port address
 * @param pin   - pin[s] to configure
 * @param spd   - GPIO speed
 */
void GPIO_pin_config_common(GPIOMode_TypeDef mode, GPIO_TypeDef* GPIOx, uint16_t pin, GPIOSpeed_TypeDef spd){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = spd;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

// default - slow
#define GPIO_pin_config(a,b,c) GPIO_pin_config_common(a,b,c, GPIO_Speed_2MHz)
#define GPIO_pin_config_fast(a,b,c) GPIO_pin_config_common(a,b,c, GPIO_Speed_50MHz)

void Ports_Config(){
	// enable AFIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// enable clocks for used ports
	RCC_APB2PeriphClockCmd(	  RCC_APB2Periph_GPIOA
							| RCC_APB2Periph_GPIOB
							| RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD
								,ENABLE);
}


// USB clock (48 MHz)
void Set_USBClock(void){
	// Select USBCLK source
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	// Enable the USB clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

// USB RX interrupts
void USB_Interrupts_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


// Software disconnect
void USB_Cable_Config (FunctionalState NewState){
	if (NewState != DISABLE)
		GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	else
		GPIO_SetBits(GPIOC, GPIO_Pin_11);
}

// Low power mode (if not required - just set flag)
void Enter_LowPowerMode(void){
	bDeviceState = SUSPENDED;
}

// enter full power mode (if not required - just set flag)
void Leave_LowPowerMode(void){
	DEVICE_INFO *pInfo = &Device_Info;
	if (pInfo->Current_Configuration != 0){
		bDeviceState = CONFIGURED;
	}else{
		bDeviceState = ATTACHED;
	}
}

// Clocks
void RCC_Config(void){
    ErrorStatus HSEStartUpStatus;
    // Configuration of SYSCLK, HCLK, PCLK2 É PCLK1
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if (HSEStartUpStatus == SUCCESS) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        // PLLCLK = 8MHz * 9 = 72 MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08){}

        /* Enable GPIOA clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        /* Enable SPI_MASTER Periph clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    } else {
        // HSE start error - clock configuration will be wrong
        // maybe say about it?
        while (1){}
    }
}
