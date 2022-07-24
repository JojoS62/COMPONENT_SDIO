/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "sdio_device.h"
#include "platform/mbed_error.h"

/* Extern variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd;
// DMA_HandleTypeDef hdma_sdmmc_rx;
// DMA_HandleTypeDef hdma_sdmmc_tx;

// simple flags for DMA pending signaling
volatile uint8_t SD_DMA_ReadPendingState = SD_TRANSFER_OK;
volatile uint8_t SD_DMA_WritePendingState = SD_TRANSFER_OK;

/* External variables --------------------------------------------------------*/
 MDMA_HandleTypeDef hmdma_mdma_channel40_sdmmc1_command_end_0;
 MDMA_HandleTypeDef hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0;
 MDMA_HandleTypeDef hmdma_mdma_channel42_sdmmc1_end_data_0;



/* DMA Handlers are global, there is only one SDIO interface */

/**
* @brief This function handles SDMMC global interrupt.
*/
void _SDMMC_IRQHandler(void)
{
    HAL_SD_IRQHandler(&hsd);
}

/**
* @brief This function handles DMAx stream_n global interrupt. DMA Rx
*/
// void _DMA_Stream_Rx_IRQHandler(void)
// {
//     HAL_DMA_IRQHandler(hsd.hdmarx);
// }

/**
* @brief This function handles DMAx stream_n global interrupt. DMA Tx
*/
// void _DMA_Stream_Tx_IRQHandler(void)
// {
//     HAL_DMA_IRQHandler(hsd.hdmatx);
// }

/**
 *
 * @param hsd:  Handle for SD handle Structure definition
 */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    IRQn_Type IRQn;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (hsd->Instance == SDMMC1)
    {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
        PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            error("SDMMC MDMA Init error at %d in %s", __LINE__, __FILE__);
        }

        /* Peripheral clock enable */
        __HAL_RCC_SDMMC1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**SDMMC1 GPIO Configuration
        PC8     ------> SDMMC1_D0
        PC9     ------> SDMMC1_D1
        PC10     ------> SDMMC1_D2
        PC11     ------> SDMMC1_D3
        PC12     ------> SDMMC1_CK
        PD2     ------> SDMMC1_CMD
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                            |GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


        /* MDMA controller clock enable */
        __HAL_RCC_MDMA_CLK_ENABLE();
        /* Local variables */

        /* Configure MDMA channel MDMA_Channel0 */
        /* Configure MDMA request hmdma_mdma_channel40_sdmmc1_command_end_0 on MDMA_Channel0 */
        hmdma_mdma_channel40_sdmmc1_command_end_0.Instance = MDMA_Channel0;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.Priority = MDMA_PRIORITY_LOW;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.SourceInc = MDMA_SRC_INC_BYTE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.BufferTransferLength = 1;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.SourceBlockAddressOffset = 0;
        hmdma_mdma_channel40_sdmmc1_command_end_0.Init.DestBlockAddressOffset = 0;
        if (HAL_MDMA_Init(&hmdma_mdma_channel40_sdmmc1_command_end_0) != HAL_OK)
        {
            error("SDMMC MDMA Init error at %d in %s", __LINE__, __FILE__);
        }

        /* Configure MDMA channel MDMA_Channel1 */
        /* Configure MDMA request hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0 on MDMA_Channel1 */
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Instance = MDMA_Channel1;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.Priority = MDMA_PRIORITY_LOW;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.SourceInc = MDMA_SRC_INC_BYTE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.BufferTransferLength = 1;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.SourceBlockAddressOffset = 0;
        hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0.Init.DestBlockAddressOffset = 0;
        if (HAL_MDMA_Init(&hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0) != HAL_OK)
        {
            error("SDMMC MDMA Init error at %d in %s", __LINE__, __FILE__);
        }

        /* Configure MDMA channel MDMA_Channel2 */
        /* Configure MDMA request hmdma_mdma_channel42_sdmmc1_end_data_0 on MDMA_Channel2 */
        hmdma_mdma_channel42_sdmmc1_end_data_0.Instance = MDMA_Channel2;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.Priority = MDMA_PRIORITY_LOW;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.SourceInc = MDMA_SRC_INC_BYTE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.BufferTransferLength = 1;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.SourceBlockAddressOffset = 0;
        hmdma_mdma_channel42_sdmmc1_end_data_0.Init.DestBlockAddressOffset = 0;
        if (HAL_MDMA_Init(&hmdma_mdma_channel42_sdmmc1_end_data_0) != HAL_OK)
        {
            error("SDMMC MDMA Init error at %d in %s", __LINE__, __FILE__);
        }

        /* NVIC configuration for SDMMC interrupts */
        IRQn = MDMA_IRQn;
        /* SDMMC1 interrupt Init */
        HAL_NVIC_SetPriority(IRQn, 0x0E, 0);
        // NVIC_SetVector(IRQn, (uint32_t)&_SDMMC_IRQHandler);
        HAL_NVIC_EnableIRQ(IRQn);

        /* Enable NVIC for DMA transfer complete interrupts */
        // IRQn = DMA2_Stream3_IRQn;
        // NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Rx_IRQHandler);
        // HAL_NVIC_SetPriority(IRQn, 0x0, 0);
        // HAL_NVIC_EnableIRQ(IRQn);

        // IRQn = DMA2_Stream6_IRQn;
        // NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Tx_IRQHandler);
        // HAL_NVIC_SetPriority(IRQn, 0x0, 0);
        // HAL_NVIC_EnableIRQ(IRQn);
    }
#if 0
// old

        /* SDMMC DMA Init */
        /* SDMMC_RX Init */
        hdma_sdmmc_rx.Instance = DMA2_Stream3;
        hdma_sdmmc_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_sdmmc_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_sdmmc_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_sdmmc_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_sdmmc_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_sdmmc_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_sdmmc_rx.Init.Mode = DMA_PFCTRL;
        hdma_sdmmc_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_sdmmc_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdma_sdmmc_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdma_sdmmc_rx.Init.MemBurst = DMA_MBURST_INC4;
        hdma_sdmmc_rx.Init.PeriphBurst = DMA_PBURST_INC4;
        if (HAL_DMA_Init(&hdma_sdmmc_rx) != HAL_OK) {
            error("SDMMC DMA Init error at %d in %s", __LINE__, __FILE__);
        }

        __HAL_LINKDMA(hsd, hdmarx, hdma_sdmmc_rx);

        /* Deinitialize the stream for new transfer */
        HAL_DMA_DeInit(&hdma_sdmmc_rx);

        /* Configure the DMA stream */
        HAL_DMA_Init(&hdma_sdmmc_rx);

        /* SDMMC_TX Init */
        hdma_sdmmc_tx.Instance = DMA2_Stream6;
        hdma_sdmmc_tx.Init.Channel = DMA_CHANNEL_4;
        hdma_sdmmc_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_sdmmc_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_sdmmc_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_sdmmc_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_sdmmc_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_sdmmc_tx.Init.Mode = DMA_PFCTRL;
        hdma_sdmmc_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_sdmmc_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdma_sdmmc_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdma_sdmmc_tx.Init.MemBurst = DMA_MBURST_INC4;
        hdma_sdmmc_tx.Init.PeriphBurst = DMA_PBURST_INC4;
        if (HAL_DMA_Init(&hdma_sdmmc_tx) != HAL_OK) {
            error("SDMMC DMA Init error at %d in %s", __LINE__, __FILE__);
        }

        __HAL_LINKDMA(hsd, hdmatx, hdma_sdmmc_tx);

        /* Deinitialize the stream for new transfer */
        HAL_DMA_DeInit(&hdma_sdmmc_tx);

        /* Configure the DMA stream */
        HAL_DMA_Init(&hdma_sdmmc_tx);

        /* Enable NVIC for DMA transfer complete interrupts */
        IRQn = DMA2_Stream3_IRQn;
        NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Rx_IRQHandler);
        HAL_NVIC_SetPriority(IRQn, 0x0, 0);
        HAL_NVIC_EnableIRQ(IRQn);

        IRQn = DMA2_Stream6_IRQn;
        NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Tx_IRQHandler);
        HAL_NVIC_SetPriority(IRQn, 0x0, 0);
        HAL_NVIC_EnableIRQ(IRQn);
    } 
#endif
    
#if 0
    if(hsd->Instance==SDMMC2) {
        // set clock source for SDMMC2 to CLK48
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC2;
        PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
        [[maybe_unused]] volatile int err = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_SDMMC2_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**SDMMC2 GPIO Configuration
        PB4     ------> SDMMC2_D3
        PB3     ------> SDMMC2_D2
        PD7     ------> SDMMC2_CMD
        PD6     ------> SDMMC2_CK
        PG10     ------> SDMMC2_D1
        PG9     ------> SDMMC2_D0
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_SDMMC2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_SDMMC2;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /* NVIC configuration for SDMMC interrupts */
        IRQn = SDMMC2_IRQn;
        HAL_NVIC_SetPriority(IRQn, 0x0, 0);
        NVIC_SetVector(IRQn, (uint32_t)&_SDMMC_IRQHandler);
        HAL_NVIC_EnableIRQ(IRQn);

        /* SDMMC DMA Init */
        /* SDMMC_RX Init */
        hdma_sdmmc_rx.Instance = DMA2_Stream0;
        hdma_sdmmc_rx.Init.Channel = DMA_CHANNEL_11;
        hdma_sdmmc_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_sdmmc_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_sdmmc_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_sdmmc_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_sdmmc_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_sdmmc_rx.Init.Mode = DMA_PFCTRL;
        hdma_sdmmc_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_sdmmc_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdma_sdmmc_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdma_sdmmc_rx.Init.MemBurst = DMA_MBURST_INC4;
        hdma_sdmmc_rx.Init.PeriphBurst = DMA_PBURST_INC4;
        if (HAL_DMA_Init(&hdma_sdmmc_rx) != HAL_OK) {
            error("SDMMC DMA Init error at %d in %s", __LINE__, __FILE__);
        }

        __HAL_LINKDMA(hsd, hdmarx, hdma_sdmmc_rx);

        /* Deinitialize the stream for new transfer */
        HAL_DMA_DeInit(&hdma_sdmmc_rx);

        /* Configure the DMA stream */
        HAL_DMA_Init(&hdma_sdmmc_rx);

        /* SDMMC_TX Init */
        hdma_sdmmc_tx.Instance = DMA2_Stream5;
        hdma_sdmmc_tx.Init.Channel = DMA_CHANNEL_11;
        hdma_sdmmc_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_sdmmc_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_sdmmc_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_sdmmc_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_sdmmc_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_sdmmc_tx.Init.Mode = DMA_PFCTRL;
        hdma_sdmmc_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_sdmmc_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdma_sdmmc_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdma_sdmmc_tx.Init.MemBurst = DMA_MBURST_INC4;
        hdma_sdmmc_tx.Init.PeriphBurst = DMA_PBURST_INC4;
        if (HAL_DMA_Init(&hdma_sdmmc_tx) != HAL_OK) {
            error("SDMMC DMA Init error at %d in %s", __LINE__, __FILE__);
        }

        __HAL_LINKDMA(hsd, hdmatx, hdma_sdmmc_tx);

        /* Deinitialize the stream for new transfer */
        HAL_DMA_DeInit(&hdma_sdmmc_tx);

        /* Configure the DMA stream */
        HAL_DMA_Init(&hdma_sdmmc_tx);

        /* Enable NVIC for DMA transfer complete interrupts */
        IRQn = DMA2_Stream0_IRQn;
        NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Rx_IRQHandler);
        HAL_NVIC_SetPriority(IRQn, 0x0F, 0);
        HAL_NVIC_EnableIRQ(IRQn);

        IRQn = DMA2_Stream5_IRQn;
        NVIC_SetVector(IRQn, (uint32_t)&_DMA_Stream_Tx_IRQHandler);
        HAL_NVIC_SetPriority(IRQn, 0x0F, 0);
        HAL_NVIC_EnableIRQ(IRQn);
    }
#endif
}

/**
 *
 * @param hsd:  Handle for SD handle Structure definition
 */
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    if (hsd->Instance == SDMMC1) {
        __HAL_RCC_SDMMC1_CLK_DISABLE();

        /**SDMMC1 GPIO Configuration
        PC8     ------> SDMMC1_D0
        PC9     ------> SDMMC1_D1
        PC10     ------> SDMMC1_D2
        PC11     ------> SDMMC1_D3
        PC12     ------> SDMMC1_CK
        PD2     ------> SDMMC1_CMD
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                            |GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    }


    /* SDMMC DMA DeInit */
    // HAL_DMA_DeInit(hsd->hdmarx);
    // HAL_DMA_DeInit(hsd->hdmatx);
}

/**
 * @brief  DeInitializes the SD MSP.
 * @param  hsd: SD handle
 * @param  Params : pointer on additional configuration parameters, can be NULL.
 */
void SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
    static DMA_HandleTypeDef dma_rx_handle;
    static DMA_HandleTypeDef dma_tx_handle;


    if (hsd->Instance == SDMMC1) {
        /* Disable NVIC for DMA transfer complete interrupts */
        HAL_NVIC_DisableIRQ(DMA2_Stream3_IRQn);
        HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);

        /* Deinitialize the stream for new transfer */
        dma_rx_handle.Instance = DMA2_Stream3;
        HAL_DMA_DeInit(&dma_rx_handle);

        /* Deinitialize the stream for new transfer */
        dma_tx_handle.Instance = DMA2_Stream6;
        HAL_DMA_DeInit(&dma_tx_handle);

        /* Disable NVIC for SDMMC interrupts */
        HAL_NVIC_DisableIRQ(SDMMC1_IRQn);

        /* Disable SDMMC clock */
        __HAL_RCC_SDMMC1_CLK_DISABLE();
    }

#if 0
    if (hsd->Instance == SDMMC2) {
        /* Disable NVIC for DMA transfer complete interrupts */
        HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
        HAL_NVIC_DisableIRQ(DMA2_Stream5_IRQn);

        /* Deinitialize the stream for new transfer */
        dma_rx_handle.Instance = DMA2_Stream0;
        HAL_DMA_DeInit(&dma_rx_handle);

        /* Deinitialize the stream for new transfer */
        dma_tx_handle.Instance = DMA2_Stream5;
        HAL_DMA_DeInit(&dma_tx_handle);
        /* Disable NVIC for SDMMC interrupts */
        HAL_NVIC_DisableIRQ(SDMMC2_IRQn);

        /* Disable SDMMC clock */
        __HAL_RCC_SDMMC2_CLK_DISABLE();
    }
#endif
}


// /**
//   * @brief  Enables the SD wide bus mode.
//   * @param  hsd pointer to SD handle
//   * @retval error state
//   */
// static uint32_t SD_WideBus_Enable(SD_HandleTypeDef *hsd)
// {
//     uint32_t errorstate = HAL_SD_ERROR_NONE;

//     if ((SDMMC_GetResponse(hsd->Instance, SDMMC_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
//     {
//         return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;
//     }

//     /* Send CMD55 APP_CMD with argument as card's RCA.*/
//     errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
//     if (errorstate != HAL_OK)
//     {
//         return errorstate;
//     }

//     /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
//     errorstate = SDMMC_CmdBusWidth(hsd->Instance, 2U);
//     if (errorstate != HAL_OK)
//     {
//         return errorstate;
//     }

//     hsd->Init.BusWide = SDMMC_BUS_WIDE_4B;
//     SDMMC_Init(hsd->Instance, hsd->Init);

//     return HAL_SD_ERROR_NONE;
// }

/**
 * @brief  Initializes the SD card device.
 * @retval SD status
 */
uint8_t SD_Init(void)
{
    uint8_t sd_state = MSD_OK;

#ifdef TARGET_DISCO_F769NI
    hsd.Instance = SDMMC2;
#else
    hsd.Instance = SDMMC1;
#endif
    hsd.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
    hsd.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDMMC_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 0;

    /* HAL SD initialization */
    sd_state = HAL_SD_Init(&hsd);
    /* Configure SD Bus width (4 bits mode selected) */
    if (sd_state == MSD_OK)
    {
        /* Enable wide operation */
        if (HAL_SD_ConfigWideBusOperation(&hsd, SDMMC_BUS_WIDE_4B) != HAL_OK)
        {
            sd_state = MSD_ERROR;
        }
    }

    return sd_state;
}

/**
 * @brief  DeInitializes the SD card device.
 * @retval SD status
 */
uint8_t SD_DeInit(void)
{
    uint8_t sd_state = MSD_OK;

    /* HAL SD deinitialization */
    if (HAL_SD_DeInit(&hsd) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    /* Msp SD deinitialization */
    SD_MspDeInit(&hsd, NULL);

    return sd_state;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @param  Timeout: Timeout for read operation
 * @retval SD status
 */
uint8_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_ReadBlocks(&hsd, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @param  Timeout: Timeout for write operation
 * @retval SD status
 */
uint8_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_WriteBlocks(&hsd, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @retval SD status
 */
uint8_t SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
    uint8_t sd_state = MSD_OK;
    SD_DMA_ReadPendingState = SD_TRANSFER_BUSY;

    /* Read block(s) in DMA transfer mode */
    if (HAL_SD_ReadBlocks_DMA(&hsd, (uint8_t *)pData, ReadAddr, NumOfBlocks) != HAL_OK)
    {
        sd_state = MSD_ERROR;
        SD_DMA_ReadPendingState = SD_TRANSFER_OK;
    }

    return sd_state;
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @retval SD status
 */
uint8_t SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
    uint8_t sd_state = MSD_OK;
    SD_DMA_WritePendingState = SD_TRANSFER_BUSY;

    /* Write block(s) in DMA transfer mode */
    if (HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t *)pData, WriteAddr, NumOfBlocks) != HAL_OK)
    {
        sd_state = MSD_ERROR;
        SD_DMA_WritePendingState = SD_TRANSFER_OK;
    }

    return sd_state;
}

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  EndAddr: End byte address
 * @retval SD status
 */
uint8_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_Erase(&hsd, StartAddr, EndAddr) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Gets the current SD card data status.
 * @param  None
 * @retval Data transfer state.
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
uint8_t SD_GetCardState(void)
{
    return ((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
 * @brief  Get SD information about specific SD card.
 * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
 * @retval None
 */
void SD_GetCardInfo(SD_Cardinfo_t *CardInfo)
{
    /* Get SD card Information, copy structure for portability */
    HAL_SD_CardInfoTypeDef HAL_CardInfo;

    HAL_SD_GetCardInfo(&hsd, &HAL_CardInfo);

    if (CardInfo)
    {
        CardInfo->CardType = HAL_CardInfo.CardType;
        CardInfo->CardVersion = HAL_CardInfo.CardVersion;
        CardInfo->Class = HAL_CardInfo.Class;
        CardInfo->RelCardAdd = HAL_CardInfo.RelCardAdd;
        CardInfo->BlockNbr = HAL_CardInfo.BlockNbr;
        CardInfo->BlockSize = HAL_CardInfo.BlockSize;
        CardInfo->LogBlockNbr = HAL_CardInfo.LogBlockNbr;
        CardInfo->LogBlockSize = HAL_CardInfo.LogBlockSize;
    }
}

/**
 * @brief  Check if a DMA operation is pending
 * @retval DMA operation is pending
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
uint8_t SD_DMA_ReadPending(void)
{
    return SD_DMA_ReadPendingState;
}

/**
 * @brief  Check if a DMA operation is pending
 * @retval DMA operation is pending
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
uint8_t SD_DMA_WritePending(void)
{
    return SD_DMA_WritePendingState;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd Pointer SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    SD_DMA_ReadPendingState = SD_TRANSFER_OK;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd Pointer to SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    SD_DMA_WritePendingState = SD_TRANSFER_OK;
}

void MDMA_IRQHandler(void)
{
  /* USER CODE BEGIN MDMA_IRQn 0 */

  /* USER CODE END MDMA_IRQn 0 */
  HAL_MDMA_IRQHandler(&hmdma_mdma_channel40_sdmmc1_command_end_0);
  HAL_MDMA_IRQHandler(&hmdma_mdma_channel41_sdmmc1_dma_endbuffer_0);
  HAL_MDMA_IRQHandler(&hmdma_mdma_channel42_sdmmc1_end_data_0);
  /* USER CODE BEGIN MDMA_IRQn 1 */

  /* USER CODE END MDMA_IRQn 1 */
}