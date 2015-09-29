#include "includes.h"
#include <stdint.h>


GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

inline void application__init(void) {
    RCC_Config();

    /* Enable GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
    /* Configure PC.13 as Output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Initialize USB */
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    /* Configure SPI pins: SCK and MOSI with default alternate function (not re-mapped) push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure MISO as Input with internal pull-up */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /* SPI_MASTER configuration ------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable SPI1 */
    SPI_CalculateCRC(SPI1, DISABLE);
    SPI_Cmd(SPI1, ENABLE);
}


int main(void){
    application__init();
    while(1);
}


