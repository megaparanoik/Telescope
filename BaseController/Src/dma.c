#include "dma.h"

DMA_HandleTypeDef hdma_usart3_tx;

void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_usart3_tx.Instance = DMA1_Channel2;
  hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart3_tx.Init.Mode = DMA_NORMAL;
  hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_usart3_tx);

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}
