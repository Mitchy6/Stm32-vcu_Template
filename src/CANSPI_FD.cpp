/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "CANSPI_FD.h"

// #ifdef __cplusplus  // Provide C++ Compatibility
// extern "C" {
// #endif

#define MAX_TXQUEUE_ATTEMPTS 15
#define FLASH_DELAY 500
//------------------------------------
// First Channel Config:
CAN_CONFIG config_ch1;
CAN_OPERATION_MODE opMode_ch1;

// Transmit objects
CAN_TX_FIFO_CONFIG txConfig_ch1;
CAN_TX_FIFO_EVENT txFlags_ch1;
CAN_TX_MSGOBJ txObj_ch1;
uint8_t txd_ch1[MAX_DATA_BYTES];

// Receive objects
CAN_RX_FIFO_CONFIG rxConfig_ch1;
REG_CiFLTOBJ fObj_ch1;
REG_CiMASK mObj_ch1;
CAN_RX_FIFO_EVENT rxFlags_ch1;
CAN_RX_MSGOBJ rxObj_ch1;
uint8_t rxd_ch1[MAX_DATA_BYTES];
CAN_BUS_DIAGNOSTIC busDiagnostics_ch1;
uint8_t tec_ch1;
uint8_t rec_ch1;
CAN_ERROR_STATE errorFlags_ch1;

//------------------------------------
// Second Channel Config:
CAN_CONFIG config_ch2;
CAN_OPERATION_MODE opMode_ch2;

// Transmit objects
CAN_TX_FIFO_CONFIG txConfig_ch2;
CAN_TX_FIFO_EVENT txFlags_ch2;
CAN_TX_MSGOBJ txObj_ch2;
uint8_t txd_ch2[MAX_DATA_BYTES];

// Receive objects
CAN_RX_FIFO_CONFIG rxConfig_ch2;
REG_CiFLTOBJ fObj_ch2;
REG_CiMASK mObj_ch2;
CAN_RX_FIFO_EVENT rxFlags_ch2;
CAN_RX_MSGOBJ rxObj_ch2;
uint8_t rxd_ch2[MAX_DATA_BYTES];
CAN_BUS_DIAGNOSTIC busDiagnostics_ch2;
uint8_t tec_ch2;
uint8_t rec_ch2;
CAN_ERROR_STATE errorFlags_ch2;

void CANSPI_INIT_FD_CH1()
{
     // Reset device
     DRV_CANFDSPI_Reset(1);
     
     // Enable ECC and initialize RAM
     DRV_CANFDSPI_EccEnable(1);

     DRV_CANFDSPI_RamInit(1, 0xff);
 
     // Configure device
     DRV_CANFDSPI_ConfigureObjectReset(&config_ch1);
     config_ch1.IsoCrcEnable = 1;
     config_ch1.StoreInTEF = 0;
     config_ch1.RestrictReTxAttempts = 0;
 
     DRV_CANFDSPI_Configure(1, &config_ch1);
 
     // Setup TX FIFO
     DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig_ch1);
     txConfig_ch1.FifoSize = 32;
     txConfig_ch1.PayLoadSize = CAN_PLSIZE_8;
     txConfig_ch1.TxPriority = 1;
 
     DRV_CANFDSPI_TransmitChannelConfigure(1, APP_TX_FIFO, &txConfig_ch1);
 
     // Setup RX FIFO
     DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig_ch1);
     rxConfig_ch1.FifoSize = 32;
     rxConfig_ch1.PayLoadSize = CAN_PLSIZE_8;
 
     DRV_CANFDSPI_ReceiveChannelConfigure(1, APP_RX_FIFO, &rxConfig_ch1);
 
     // Setup RX Filter
     fObj_ch1.word = 0;
 
     DRV_CANFDSPI_FilterObjectConfigure(1, CAN_FILTER0, &fObj_ch1.bF);
 
     // Setup RX Mask
     mObj_ch1.word = 0;
     DRV_CANFDSPI_FilterMaskConfigure(1, CAN_FILTER0, &mObj_ch1.bF);
 
     // Link FIFO and Filter
     DRV_CANFDSPI_FilterToFifoLink(1, CAN_FILTER0, APP_RX_FIFO, true);
 
     // Setup Bit Time
     DRV_CANFDSPI_BitTimeConfigure(1, CAN_500K_5M, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);
 
     // Setup Transmit and Receive Interrupts
     DRV_CANFDSPI_GpioModeConfigure(1, GPIO_MODE_INT, GPIO_MODE_INT);
     DRV_CANFDSPI_ReceiveChannelEventEnable(1, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
     DRV_CANFDSPI_ModuleEventEnable(1, (CAN_MODULE_EVENT)(CAN_TX_EVENT | CAN_RX_EVENT));
 
     // Select Normal Mode
     DRV_CANFDSPI_OperationModeSelect(1, CAN_CLASSIC_MODE);
}

void CANSPI_INIT_FD_CH2()
{
     // Reset device
     DRV_CANFDSPI_Reset(2);
     
     // Enable ECC and initialize RAM
     DRV_CANFDSPI_EccEnable(2);

     DRV_CANFDSPI_RamInit(2, 0xff);
 
     // Configure device
     DRV_CANFDSPI_ConfigureObjectReset(&config_ch2);
     config_ch2.IsoCrcEnable = 1;
     config_ch2.StoreInTEF = 0;
     config_ch2.RestrictReTxAttempts = 0;
 
     DRV_CANFDSPI_Configure(2, &config_ch2);
 
     // Setup TX FIFO
     DRV_CANFDSPI_TransmitChannelConfigureObjectReset(&txConfig_ch2);
     txConfig_ch2.FifoSize = 32;
     txConfig_ch2.PayLoadSize = CAN_PLSIZE_8;
     txConfig_ch2.TxPriority = 1;
 
     DRV_CANFDSPI_TransmitChannelConfigure(2, APP_TX_FIFO, &txConfig_ch2);
 
     // Setup RX FIFO
     DRV_CANFDSPI_ReceiveChannelConfigureObjectReset(&rxConfig_ch2);
     rxConfig_ch2.FifoSize = 32;
     rxConfig_ch2.PayLoadSize = CAN_PLSIZE_8;
 
     DRV_CANFDSPI_ReceiveChannelConfigure(2, APP_RX_FIFO, &rxConfig_ch2);
 
     // Setup RX Filter
     fObj_ch2.word = 0;
 
     DRV_CANFDSPI_FilterObjectConfigure(2, CAN_FILTER0, &fObj_ch2.bF);
 
     // Setup RX Mask
     mObj_ch2.word = 0;
     DRV_CANFDSPI_FilterMaskConfigure(2, CAN_FILTER0, &mObj_ch2.bF);
 
     // Link FIFO and Filter
     DRV_CANFDSPI_FilterToFifoLink(2, CAN_FILTER0, APP_RX_FIFO, true);
 
     // Setup Bit Time
     DRV_CANFDSPI_BitTimeConfigure(2, CAN_500K_5M, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);
 
     // Setup Transmit and Receive Interrupts
     DRV_CANFDSPI_GpioModeConfigure(2, GPIO_MODE_INT, GPIO_MODE_INT);
     DRV_CANFDSPI_ReceiveChannelEventEnable(2, APP_RX_FIFO, CAN_RX_FIFO_NOT_EMPTY_EVENT);
     DRV_CANFDSPI_ModuleEventEnable(2, (CAN_MODULE_EVENT)(CAN_TX_EVENT | CAN_RX_EVENT));
 
     // Select Normal Mode
     DRV_CANFDSPI_OperationModeSelect(2, CAN_CLASSIC_MODE);
}

uint8_t CANSPI_FD_receive(CAN20_MSG *tempCanMsg,int8_t channel)
{
    uint8_t returnValue = 0;
    uint8_t rxd_can[8];
    CAN_RX_MSGOBJ rxObj_can;
    CAN_RX_FIFO_EVENT rxFlags_can;
    DRV_CANFDSPI_ReceiveChannelEventGet(channel, APP_RX_FIFO, &rxFlags_can);
    if (rxFlags_can & CAN_RX_FIFO_NOT_EMPTY_EVENT) 
	{
        DRV_CANFDSPI_ReceiveMessageGet(channel, APP_RX_FIFO, &rxObj_can, rxd_can, 8);
        if (rxObj_can.bF.ctrl.IDE == 1)
        {
            tempCanMsg->frame.id = (rxObj_can.bF.id.EID | (rxObj_can.bF.id.SID << 18));
        }
        else
        {
            tempCanMsg->frame.id = rxObj_can.bF.id.SID;
        }
        tempCanMsg->frame.dlc   = rxObj_can.bF.ctrl.DLC;
        tempCanMsg->frame.data0 = rxd_can[0];
        tempCanMsg->frame.data1 = rxd_can[1];
        tempCanMsg->frame.data2 = rxd_can[2];
        tempCanMsg->frame.data3 = rxd_can[3];
        tempCanMsg->frame.data4 = rxd_can[4];
        tempCanMsg->frame.data5 = rxd_can[5];
        tempCanMsg->frame.data6 = rxd_can[6];
        tempCanMsg->frame.data7 = rxd_can[7];
        returnValue = 1;
    }
    else
    {
        returnValue = 0;
	}
    
    return (returnValue);
}

void CANSPI_FD_Transmit(uint32_t id, uint8_t ext, uint8_t len, const uint8_t *buf, int8_t channel)
{
    int i;
    uint8_t ch1_n;
    uint8_t attempts_ch1;
    uint8_t ch1_spiTransferError;
    int j;
    uint8_t ch2_n;
    uint8_t attempts_ch2;
    uint8_t ch2_spiTransferError;
    
    switch (channel)
{
    case 1:
        
        // Configure message data
        txObj_ch1.word[0] = 0;
        txObj_ch1.word[1] = 0;
        txObj_ch1.bF.ctrl.RTR = 0;
        txObj_ch1.bF.ctrl.DLC = len;
        txObj_ch1.bF.ctrl.IDE = !!ext;
        if (ext) {
        txObj_ch1.bF.id.SID = (id >> 18) & 0x7FF;
        txObj_ch1.bF.id.EID = id & 0x3FFFF;
        } else {
        txObj_ch1.bF.id.SID = id;}
        txObj_ch1.bF.ctrl.BRS = true;
        txObj_ch1.bF.ctrl.FDF = (len > 8);
        ch1_n = DRV_CANFDSPI_DlcToDataBytes((CAN_DLC)txObj_ch1.bF.ctrl.DLC);
        // Prepare data
        for (i = 0; i < ch1_n; i++) {txd_ch1[i] = buf[i];}
        attempts_ch1 = 1;
        // Check if FIFO is not full
        do {
            DRV_CANFDSPI_TransmitChannelEventGet(1, APP_TX_FIFO, &txFlags_ch1);
            if (attempts_ch1 == 0) {
                DRV_CANFDSPI_ErrorCountStateGet(1, &tec_ch1, &rec_ch1, &errorFlags_ch1);
                }
                attempts_ch1--;
            }
            while (!(txFlags_ch1 & CAN_TX_FIFO_NOT_FULL_EVENT));
        // Load message and transmit
        DRV_CANFDSPI_TransmitChannelLoad(1, APP_TX_FIFO, &txObj_ch1, txd_ch1, ch1_n, true);
    break;
        
    case 2:
        // Configure message data
        txObj_ch2.word[0] = 0;
        txObj_ch2.word[1] = 0;
        txObj_ch2.bF.ctrl.RTR = 0;
        txObj_ch2.bF.ctrl.DLC = len;
        txObj_ch2.bF.ctrl.IDE = !!ext;
        if (ext) {
        txObj_ch2.bF.id.SID = (id >> 18) & 0x7FF;
        txObj_ch2.bF.id.EID = id & 0x3FFFF;
        } else {txObj_ch2.bF.id.SID = id;}
        txObj_ch2.bF.ctrl.BRS = true;
        txObj_ch2.bF.ctrl.FDF = (len > 8);
        ch2_n = DRV_CANFDSPI_DlcToDataBytes((CAN_DLC)txObj_ch2.bF.ctrl.DLC);
        // Prepare data
        for (j = 0; j < ch2_n; j++) {txd_ch2[j] = buf[j];}
        attempts_ch2 = 1;
            // Check if FIFO is not full
            do {
                DRV_CANFDSPI_TransmitChannelEventGet(2, APP_TX_FIFO, &txFlags_ch2);
                if (attempts_ch2 == 0) {
                DRV_CANFDSPI_ErrorCountStateGet(2, &tec_ch2, &rec_ch2, &errorFlags_ch2);
                }
                attempts_ch2--;
            }
            while (!(txFlags_ch2 & CAN_TX_FIFO_NOT_FULL_EVENT));
            // Load message and transmit
        DRV_CANFDSPI_TransmitChannelLoad(2, APP_TX_FIFO, &txObj_ch2, txd_ch2, ch2_n, true);
    break;
}
}
// #ifdef __cplusplus
// }
// #endif