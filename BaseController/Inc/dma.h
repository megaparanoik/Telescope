#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern DMA_HandleTypeDef hdma_usart3_tx;

void MX_DMA_Init(void);

#ifdef __cplusplus
}
#endif
