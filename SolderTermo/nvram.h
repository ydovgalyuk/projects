#ifndef NVRAM_HH
#define NVRAM_HH

struct NVRAM_Data
{
	int16_t Tsp;	// *C
	int16_t OffTimeout;	// sec
	int16_t Kp;
	int16_t Ki;
	int16_t Kd;
	int16_t IsOpenLoop;	// 0 - open, 1 - closed
	int16_t Tofs;	// 0 - open, 1 - closed
	
	int16_t Version;
	
	//last item!!!
	uint16_t nvramCrc;
};

extern struct NVRAM_Data nvram;

void initNVRAM();
void saveNVRAM();
void readNVRAM();
void resetNVRAM();

#endif
