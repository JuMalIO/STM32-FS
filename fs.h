#ifndef __fs_H
#define __fs_H


#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

HAL_StatusTypeDef FS_Init(void);
void FS_DeInit(void);
uint32_t FS_Size(char *file);
uint32_t FS_ReadFile(char *file, uint8_t *buffer, uint32_t offset, uint32_t bytesToRead);
HAL_StatusTypeDef FS_ReadEntireFile(char *file, uint8_t *buffer, uint32_t bufferSize, void (*callback)(uint8_t *buffer, uint32_t fromBytesCount, uint32_t bytesCount));
HAL_StatusTypeDef FS_WriteFile(char *file, uint8_t *buffer, uint32_t bytesToWrite, bool append);
HAL_StatusTypeDef FS_ReadDirectory(char *path, HAL_StatusTypeDef (*callback)(char *path, char *file));
void FS_CreateDirectory(char *path);
void FS_Delete(char *path);
bool FS_IsExistingDirectory(char *path);
bool FS_IsExistingFile(char *file);


#endif /* __fs_H */
