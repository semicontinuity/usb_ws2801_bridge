// -----------------------------------------------------------------------------
// USB Endpoint handler
// -----------------------------------------------------------------------------

#include <string.h>
#include "hw_config.h"
#include "usb_regs.h"
#include "usb_lib.h"
#include "usb_istr.h"

#define DATA_LENGTH (3*150)

uint8_t usb_endpoint_handler__buffer[64];
uint8_t data[DATA_LENGTH];
uint8_t *current_data = data;


/**
 * EP1 OUT callback.
 * Accumulates data from packets and sends it to SPI1.
 * SPI data can be larger than 64 bytes, and since the maximum payload size on this endpoint is 64 bytes,
 * data is split into multiple packets.
 * If data is larger than 64 bytes, all packet payloads but the last one are 64 bytes large.
 * Packet with less than 64 bytes of data is a marker for the end of SPI data.
 */
void EP1_OUT_Callback(void) {
    uint32_t count = USB_SIL_Read(EP1_OUT, usb_endpoint_handler__buffer);
    if (count == 64) {
        // non-final packet
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // blue LED on
        memcpy(current_data, usb_endpoint_handler__buffer, count);
        current_data += count;
    } else {
        // final packet
        if (current_data + count <= data + DATA_LENGTH) {
            GPIO_SetBits(GPIOC, GPIO_Pin_13);    // blue LED off    
            memcpy(current_data, usb_endpoint_handler__buffer, count);
            current_data += count;

            // send all data to SPI1
            uint8_t *p = data;
            while (p < current_data) {
                // Wait for SPIy Tx buffer empty
                while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
                SPI_I2S_SendData(SPI1, *p++);
            }
        }
        current_data = data;    
    }

    SetEPRxStatus(ENDP1, EP_RX_VALID);
}


void EP1_IN_Callback(void) {
    USB_SIL_Write(EP1_IN, usb_endpoint_handler__buffer, 8);
    SetEPTxValid(ENDP1);
}

