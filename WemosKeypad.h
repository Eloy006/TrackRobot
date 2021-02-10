// WemosKeypad.h

#ifndef _WEMOSKEYPAD_h
#define _WEMOSKEYPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <Keypad_I2C.h>

#define I2CKeyPadADDR 0x20


enum PadKeys
{
	Unknow = -1,
	Waite=-2,
	K_0 = 0,
	K_1 = 1,
	K_2 = 2,
	K_3 = 3,
	K_4 = 4,
	K_5 = 5,
	K_6 = 6,
	K_7 = 7,
	K_8 = 8,
	K_9 = 9,
	K_A = 10,
	K_B = 11,
	K_C = 12,
	K_D = 13,
	K_SS = 14, //*
	K_EE = 15, //#
};

class KeyPad
{
public:

	PadKeys GetKey();
	void Initialize();
	

};

extern KeyPad keyPad;

#endif

