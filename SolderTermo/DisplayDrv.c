 //  __	 0
 // |  |	5 1
 //  --	 6
 // |  |	4 2
 //  --     3
  
#include "common.h"
//#include "ics.h"
//#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "pit.h"
#include "sysinit.h"
#include "start.h"

typedef char byte;

  // definition for the displayable ASCII chars
const byte FONT_DEFAULT[] = {
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (0)  none
  0b00000000, // (32)  <space>
  0b10000110, // (33)   !
  0b00100010, // (34)   "
  0b01111110, // (35)   #
  0b01101101, // (36)   $
  0b00101101, // (37)   %
  0b00000000, // (38)   &
  0b00000010, // (39)   '
  0b00110000, // (40)   (
  0b00000110, // (41)   )
  0b01100011, // (42)   *
  0b00000000, // (43)   +
  0b00000100, // (44)   ,
  0b01000000, // (45)   -
  0b10000000, // (46)   .
  0b01010010, // (47)   /
  0b00111111, // (48)   0
  0b00000110, // (49)   1
  0b01011011, // (50)   2
  0b01001111, // (51)   3
  0b01100110, // (52)   4
  0b01101101, // (53)   5
  0b01111101, // (54)   6
  0b00100111, // (55)   7
  0b01111111, // (56)   8
  0b01101111, // (57)   9
  0b00000000, // (58)   :
  0b00000000, // (59)   ;
  0b00000000, // (60)   <
  0b01001000, // (61)   =
  0b00000000, // (62)   >
  0b01010011, // (63)   ?
  0b01111011, // (64)   @
  0b01110111, // (65)   A
  0b01111111, // (66)   B
  0b00111001, // (67)   C
  0b00111111, // (68)   D
  0b01111001, // (69)   E
  0b01110001, // (70)   F
  0b00111101, // (71)   G
  0b01110110, // (72)   H
  0b00000110, // (73)   I
  0b00011110, // (74)   J
  0b01101001, // (75)   K
  0b00111000, // (76)   L
  0b00010101, // (77)   M
  0b00110111, // (78)   N
  0b00111111, // (79)   O
  0b01110011, // (80)   P
  0b01100111, // (81)   Q
  0b00110001, // (82)   R
  0b01101101, // (83)   S
  0b01111000, // (84)   T
  0b00111110, // (85)   U
  0b00101010, // (86)   V
  0b00011101, // (87)   W
  0b01110110, // (88)   X
  0b01101110, // (89)   Y
  0b01011011, // (90)   Z
  0b00111001, // (91)   [
  0b01100100, // (92)   \ (this can't be the last char on a line, even in comment or it'll concat)
  0b00001111, // (93)   ]
  0b00000000, // (94)   ^
  0b00001000, // (95)   _
  0b00100000, // (96)   `
  0b01011111, // (97)   a
  0b01111100, // (98)   b
  0b01011000, // (99)   c
  0b01011110, // (100)  d
  0b01111011, // (101)  e
  0b00110001, // (102)  f
  0b01101111, // (103)  g
  0b01110100, // (104)  h
  0b00000100, // (105)  i
  0b00001110, // (106)  j
  0b01110101, // (107)  k
  0b00110000, // (108)  l
  0b01010101, // (109)  m
  0b01010100, // (110)  n
  0b01011100, // (111)  o
  0b01110011, // (112)  p
  0b01100111, // (113)  q
  0b01010000, // (114)  r
  0b01101101, // (115)  s
  0b01111000, // (116)  t
  0b00011100, // (117)  u
  0b00101010, // (118)  v
  0b00011101, // (119)  w
  0b01110110, // (120)  x
  0b01101110, // (121)  y
  0b01000111, // (122)  z
  0b01000110, // (123)  {
  0b00000110, // (124)  |
  0b01110000, // (125)  }
  0b00000001, // (126)  ~
};

#define SWITCH_POS	7

const GPIO_PinType SEG1[8] =
{
	GPIO_PTC6,	// a
	GPIO_PTA0,	// b
	GPIO_PTA1,	// c
	GPIO_PTA3,	// d
	GPIO_PTA2,	// e
	GPIO_PTC7,	// f
	GPIO_PTE3,	// g
	GPIO_PTE2	// switch
};

const GPIO_PinType SEG2[8] =
{
	GPIO_PTE4,	// a
	GPIO_PTD2,	// b
	GPIO_PTD3,	// c
	GPIO_PTF0,	// d
	GPIO_PTD4,	// e
	GPIO_PTA6,	// f
	GPIO_PTF1,	// g
	GPIO_PTA7	// switch
};

#define INT_BUFF_LEN	50
char buff[INT_BUFF_LEN] = "8888\0";
static char l_isBlink = 0;
static char l_isShift = 0;

static void clearAll()
{
	for(int i = 0; i < 7; ++i)
	{
		GPIO_PinClear(SEG1[i]);
		GPIO_PinClear(SEG2[i]);
	}
}


static void configurePinOut(const byte1, const byte2)
{
	for(int i = 0; i < 7; ++i)
	{
		if ( byte1 & (1 << i) )
			GPIO_PinSet(SEG1[i]);
		else 
			GPIO_PinClear(SEG1[i]);
		if ( byte2 & (1 << i) )
			GPIO_PinSet(SEG2[i]);
		else 
			GPIO_PinClear(SEG2[i]);
	}
}

static void configureDigitNo(int digitNo)
{
	if (digitNo == 0)
	{
		GPIO_PinSet(SEG1[SWITCH_POS]);
		GPIO_PinSet(SEG2[SWITCH_POS]);
	}
	else
	{
		GPIO_PinClear(SEG1[SWITCH_POS]);
		GPIO_PinClear(SEG2[SWITCH_POS]);		
	}
}

#define TIMER_TIME_MS	  5 //ms
#define BLINK_TIME_MS	250 //ms
#define SHIFT_TIME_MS  1000 //ms

#define BLINK_TIME		BLINK_TIME_MS / TIMER_TIME_MS	// 150 * 5 = 750ms
#define SHIFT_TIME		SHIFT_TIME_MS / TIMER_TIME_MS	// 100 * 5 = 500ms


void display_timer(void) // called every 5 ms
{
	static char currDigit = 0;
	static char currCharNo = 0;
	static char intCounter = 0;
	static int blnkCounter = 0;

	if (++intCounter == SHIFT_TIME) // time to shift to next char?
	{			
		intCounter = 0;
		if ( currCharNo < sizeof(buff) - 4 )
		{
			if ( buff[currCharNo + 4] != 0 )
				++currCharNo;
			else
				currCharNo = 0;
		}
		else 
			currCharNo = 0;
	}
	char *string = buff + (l_isShift ? currCharNo : 0);

	clearAll();	
	configureDigitNo(currDigit);
	if (l_isBlink)
	{
		++blnkCounter;
		if (blnkCounter < BLINK_TIME)
		{
			configurePinOut(FONT_DEFAULT[string[currDigit]], FONT_DEFAULT[string[2 + currDigit]]);
		}
		else if (blnkCounter > BLINK_TIME * 2)
		{
			blnkCounter = 0;
		}
	}
	else
	{
		blnkCounter = 0;
		configurePinOut(FONT_DEFAULT[string[currDigit]], FONT_DEFAULT[string[2 + currDigit]]);
	}
	currDigit ^= 1; // toggle bit
}

PIT_ConfigType  sPITConfig;

void initDisplay()
{
	/* configure PIT module in chain mode */
	/* PIT clock source is bus clock,24MHz */
	/* PIT channel 0 load value = (120000-1) */
	uint32_t u32LoadValue0   = TIMER_TIME_MS * 24000 - 1;                  /*!< PIT ch0 timer load value  */

	for(int i = 0; i < 8; ++i)
	{
		GPIO_PinInit(SEG1[i], GPIO_PinOutput);
		GPIO_PinInit(SEG2[i], GPIO_PinOutput);
	}

	sPITConfig.u32LoadValue      = u32LoadValue0;
	sPITConfig.bFreeze           = FALSE;
	sPITConfig.bModuleDis        = FALSE;    /*!< enable PIT module */
	sPITConfig.bInterruptEn      = TRUE;
	sPITConfig.bChainMode        = FALSE;
	sPITConfig.bTimerEn          = TRUE;

	PIT_Init(PIT_CHANNEL0, &sPITConfig);

	PIT_SetCallback(PIT_CHANNEL0, display_timer);
}


#define min(a,b) ( (a) < (b) ? (a) : (b))
void setString(char* string, int length, char isBlink, char isShift)
{
	memset(buff, 0, INT_BUFF_LEN);
	strncpy(buff, string, min(length, INT_BUFF_LEN));
	l_isBlink = isBlink;
	l_isShift = isShift;
}
