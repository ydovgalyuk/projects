

// Menu structure
/*


OPEr
	- tSP    <-- default startup
	- ct

Int
	- It	// junction temp
	- IdlC	// idle counter
	- rEL	// release - version

PAr
//	- tSP
	- coP
	- coI
	- coD
	- Idlt
	- oFFt

*/

#include "common.h"
#include "nvram.h"
#include "StateMachine.h"

struct MenuCategory;
struct DataItem
{
	char Name[5];
	char Unit[5];
	char RO;
	uint16 step;	
	uint16 divdr;
	int16  *data;
};

struct MenuCategory
{
	char   Name[5];
	char   ItemsNo;
	struct DataItem *data;
};

enum CMD_TYPE
{
	SAVE_CMD = 1,
	RESET_CMD,
	EXT_STP_CMD
};

struct MenuCategory m_oper = 	{ "-OPE",  4, NULL };
struct MenuCategory m_int = 	{ "-Int",  4, NULL };
struct MenuCategory m_params = 	{ "-PAr",  8, NULL };

struct MenuCategory* m_menus[] = {&m_oper, &m_int, &m_params};

struct DataItem di_oper[4] = {
	{"", "*", 1, 0, 10, &vram.CurrTemp	}, //*Ct
	{"", "*", 0, 5, 1, &vram.TempSP		}, //tSP
	{"", "%", 0, 10, 10, &vram.PWM_Sp		},  //PWM - manual control
	{"LooP", "oPn",   0, 1, 1, &nvram.IsOpenLoop}, // Manual parameters
};

struct DataItem di_int[4] = {
	{"Junt", "*", 1, 0, 10, &vram.JunctTemp	}, //tSP
	{"idlC", "c", 1, 0, 40, &vram.OffCntDwn	}, //tSP
	{"tAdc", "",  1, 0, 1, &vram.ADCVal		}, //PWM
	{"rEL ", "",  1, 0, 1, &nvram.Version	}, //tSP
};

struct DataItem di_par[8] = {
	{"coP ", "",   0, 1, 1, &nvram.Kp			}, // Kp
	{"coI ", "",   0, 1, 1, &nvram.Ki			}, // Ki
	{"coD ", "",   0, 1, 1, &nvram.Kd			}, // Kd
	{"tOFS", "*",  0, 1, 1, &nvram.Tofs			}, // Temp offset
	{"oFFt", "c",  0, 40, 40, &nvram.OffTimeout	}, // Off timeout
	{"Stor", "",   0, 1, SAVE_CMD, NULL			}, // Store parameters
	{"rSEt", "",   0, 1, RESET_CMD, NULL		}, // Reset parameters
	{"donE", "",   0, 1, EXT_STP_CMD, NULL		}, // Reset parameters
};

struct MenuCategory *currMenu = &m_oper;
struct DataItem *currDItem = &di_oper[0];
char diEditMode = 0;
char diViewMode = 0;

static void command(int cmd)
{
	switch(cmd)
	{
		case SAVE_CMD:
			saveNVRAM();
			break;
		case RESET_CMD:
			resetNVRAM();
			break;
		case EXT_STP_CMD:
			exitSetupMode();
			break;
	}
}

void initHMI()
{
	m_oper.data = di_oper;
	m_int.data = di_int;
	m_params.data = di_par;
}

void activateOperMenu()
{
	if (diEditMode || diViewMode)
		return;
	currMenu = &m_oper;
	currDItem = &di_oper[0];
}

void activateManualMenu()
{
	if (diEditMode || diViewMode)
		return;
	currMenu = &m_oper;
	currDItem = &di_oper[2];
}

void menu_btn_down()
{
	if (currDItem) 
	{
		// we are on data item level
		if (diEditMode)
		{
			*((int16_t*)currDItem->data) -= currDItem->step;
		}
		else if (diViewMode)
		{

		}
		else // 
		{
			if (currDItem == &currMenu->data[currMenu->ItemsNo - 1])  // if the last item in menu
			{
				; // do nothing
			}
			else
			{
				for(int i = 0; i < currMenu->ItemsNo - 1; ++i) //search for current data item index
				{
					if (&currMenu->data[i] == currDItem)  // if index found
					{
						currDItem = &currMenu->data[i + 1];  // go one item forward
						break;
					}
				}
			}
		}
	}
	else if (currMenu)// we are on menu category level
	{		
		for(int i = 0; i < 2; ++i) //search for current menu item index
		{
			if (m_menus[i] == currMenu)  // if index found
			{
				currMenu = m_menus[i + 1];  // go one item forward
				break;
			}
		}
	}	
}

void menu_btn_up()
{
	if (currDItem) 
	{
		// we are on data item level
		if (diEditMode)
		{
			*((int16_t*)currDItem->data) += currDItem->step;
		}
		else if (diViewMode)
		{

		}
		else // 
		{
			if (currDItem == &currMenu->data[0]) 
			{
				currDItem = NULL; // if we are on the first item in menu go up to menu level
			}
			else
			{
				for(int i = 1; i < currMenu->ItemsNo; ++i) //search for current data item index
				{
					if (&currMenu->data[i] == currDItem)  // if index found
					{
						currDItem = &currMenu->data[i - 1];  // go one item back
						break;
					}
				}
			}
		}		
	}
	else if (currMenu) // we are on menu category level
	{		
		for(int i = 1; i < 3; ++i) //search for current menu item index
		{
			if (m_menus[i] == currMenu)  // if index found
			{
				currMenu = m_menus[i - 1];  // go one item forward
				break;
			}
		}		
	}	
}

void menu_btn_ok()
{
	if (currDItem) // we are on data item level
	{		
		if (!currDItem->RO)
		{
			if (currDItem->data == NULL)
			{
				// this is a command
				command(currDItem->divdr);
			}
			else
			{
				// this is a data
				if (diEditMode)
					// exit edit mode
					diEditMode = 0;
				else
					// enter edit mode
					diEditMode = 1;
			}
		}
		else if ( strlen(currDItem->Name) != 0 ) // if there is undermenu
		{
			if (diViewMode)
				// exit edit mode
				diViewMode = 0;
			else
				// enter edit mode
				diViewMode = 1;			
		}
		// else do nothing
	}
	else if (currMenu && !currDItem) // we are on menu category level
	{		
		currDItem = &currMenu->data[0];
	}
}

void rightAlign(char *buff)
{
	int len = strlen(buff); // align to right side
	int rizn = 4 - len;
	for(int i = 3; i >= 0 && rizn > 0; --i)
	{
		if (i - rizn >= 0)
			buff[i] = buff[i - rizn];
		else
			buff[i] = ' ';
	}
}

void updateDisplay()	// called every 200 ms
{
	char buff[5] = {0};
	char shouldShift = 0;
	if (currDItem) // we are on data item level
	{
		if (diEditMode || diViewMode || strlen(currDItem->Name) == 0)
		{
			sprintf(buff, "%hd%s", *(int16_t*)currDItem->data / currDItem->divdr, currDItem->Unit);
			rightAlign(buff);			
			shouldShift = 0;
		}		
		else
		{			
			sprintf(buff, "%s", currDItem->Name);
			shouldShift = 1;
		}			
	}
	else if (currMenu && !currDItem) // we are on menu category level
	{
		sprintf(buff, "%s", currMenu->Name);
		shouldShift = 1;
	}
	
	setString(buff, strlen(buff), diEditMode, shouldShift);
}
