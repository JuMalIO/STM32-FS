#include "fs.h"
#include "ff.h"
#include "diskio.h"
#include <stdio.h>

static FATFS FatFs;
static FIL File;
static DIR Dir;
static FILINFO Fno;

HAL_StatusTypeDef FS_Init(void)
{
	if (f_mount(&FatFs, "", 1) != FR_OK)
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

void FS_DeInit(void)
{
	f_mount(NULL, "", 0);
	
	disk_initialize(0);
}

uint32_t FS_Size(char *file)
{
	if (f_open(&File, file, FA_OPEN_EXISTING | FA_READ) != FR_OK)
	{
		return 0;
	}

	uint32_t bytesCount = f_size(&File);
	f_close(&File);
	return bytesCount;
}

uint32_t FS_ReadFile(char *file, uint8_t *buffer, uint32_t offset, uint32_t bytesToRead)
{
	if (f_open(&File, file, FA_OPEN_EXISTING | FA_READ) != FR_OK)
	{
		return 0;
	}

	if (offset != 0)
	{
		f_lseek(&File, offset);
	}

	uint32_t bytesCount = 0;
	f_read(&File, buffer, bytesToRead, &bytesCount);
	f_close(&File);
	return bytesCount;
}

HAL_StatusTypeDef FS_ReadEntireFile(char *file, uint8_t *buffer, uint32_t bufferSize, void (*callback)(uint8_t *buffer, uint32_t fromBytesCount, uint32_t bytesCount))
{
	if (f_open(&File, file, FA_READ) != FR_OK)
	{
		return HAL_ERROR;
	}
	
	uint32_t fromBytesCount = 0;
	uint32_t bytesCount;
	do
	{
		f_read(&File, buffer, bufferSize, &bytesCount);
		callback(buffer, fromBytesCount, bytesCount);
		fromBytesCount += bytesCount;
	} while (bytesCount > 0);

	f_close(&File);
	return HAL_OK;
}

HAL_StatusTypeDef FS_WriteFile(char *file, uint8_t *buffer, uint32_t bytesToWrite, bool append)
{
	if (f_open(&File, file, FA_WRITE | (append ? FA_OPEN_ALWAYS : FA_CREATE_ALWAYS)) != FR_OK)
	{
		return HAL_ERROR;
	}

	uint32_t bytesCount;
	f_lseek(&File, f_size(&File));
	f_write(&File, buffer, bytesToWrite, &bytesCount);
	f_close(&File);
	return HAL_OK;
}

HAL_StatusTypeDef FS_ReadDirectory(char *path, HAL_StatusTypeDef (*callback)(char *path, char *file))
{
	if (f_opendir(&Dir, path) != FR_OK)
	{
		return HAL_ERROR;
	}
	
	HAL_StatusTypeDef status;
	while (1)
	{
		if (f_readdir(&Dir, &Fno) != FR_OK || Fno.fname[0] == 0)
		{
			callback(path, NULL);
			break;
		}

		if (!(Fno.fattrib & AM_DIR))
		{
			status = callback(path, Fno.fname);
			if (status != HAL_OK)
			{
				f_closedir(&Dir);
				return status;
			}
		}
	}

	f_closedir(&Dir);
	return HAL_OK;
}

void FS_CreateDirectory(char *path)
{
	f_mkdir(path);
}

void FS_Delete(char *path)
{
	f_unlink(path);
}

bool FS_IsExistingDirectory(char *path)
{
	if (f_opendir(&Dir, path) == FR_OK)
	{
		f_closedir(&Dir);
		return true;
	}
	
	return false;
}

bool FS_IsExistingFile(char *file)
{
	if (f_open(&File, file, FA_READ) == FR_OK)
	{
		f_close(&File);
		return true;
	}
	
	return false;
}
