#include "common.h"
#include "crc.h"
#include "nvram.h"
#include "flash.h"

#define NVRAM_SECTOR				100

static struct NVRAM_Data defnvram = { 
	260, // Tsp
	600, // OFF Timeout
	45,	 // Kp
	80,  // Ki
	20,	 // Kd
	0,	 // loop 1 - open, 0 - closed (use termocouple)
	0,   // temp offset
	110, // Version,
	0	 // CRC
};

struct NVRAM_Data nvram;

static CRC_ConfigType sCRCConfig = {0};
const uint32_t u32SeedValue			 = 0xFFFF;                   /*!< set CRC-CCITT seed value */     

void initNVRAM()
{	
	 /* inital CRC register works under 16-bit mode */ 
    sCRCConfig.u32PolyData            = 0x1021;                   /*!< initialize CRC-CCITT poly value */     
    sCRCConfig.bWidth                 = CRC_WIDTH_16BIT;
    sCRCConfig.bTransposeReadType     = CRC_READ_TRANSPOSE_NONE;  /*!< do not transpose in read */      
    sCRCConfig.bTransposeWriteType    = CRC_WRITE_TRANSPOSE_NONE; /*!< do not transpose in write */ 
	
	FLASH_Init(BUS_CLK_HZ);
}

void saveNVRAM()
{
	/* begin to CRC-CCITT convertion */    
    CRC_Init(&sCRCConfig);      
    /*!< crc module inital as 16-bit mode */	
	nvram.nvramCrc = CRC_Cal16(u32SeedValue, &nvram, sizeof(nvram) - sizeof(nvram.nvramCrc));
	
	/* Erase 99th sector */
	//EEPROM_EraseSector(EEPROM_START_ADDRESS);
    FLASH_EraseSector( NVRAM_SECTOR * FLASH_SECTOR_SIZE );
	//EEPROM_Program(EEPROM_START_ADDRESS, &nvram, sizeof(nvram));
	FLASH_Program( NVRAM_SECTOR * FLASH_SECTOR_SIZE, &nvram, sizeof(nvram) );
}

void resetNVRAM()
{
	memcpy(&nvram, &defnvram, sizeof(struct NVRAM_Data));
	//saveNVRAM();
}

void readNVRAM()
{	
	/* begin to CRC-CCITT convertion */    
    CRC_Init(&sCRCConfig);      	
	
	struct NVRAM_Data* flashData = (struct NVRAM_Data*)(NVRAM_SECTOR * FLASH_SECTOR_SIZE);
	uint16_t crc = CRC_Cal16(u32SeedValue, NVRAM_SECTOR * FLASH_SECTOR_SIZE, sizeof(nvram) - sizeof(nvram.nvramCrc));
	
	if (crc == flashData->nvramCrc)
	{		
		// everything is ok, continue to use saved data
		memcpy(&nvram, flashData, sizeof(nvram));
	}
	else
	{
		// we have problems!!!
		// continue with default params
		resetNVRAM();
	}
}
