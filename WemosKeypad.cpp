// 
// 
// 

#include "WemosKeypad.h"

KeyPad keyPad;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 0, 1, 2, 3 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6,7 }; //connect to the column pinouts of the keypad

Keypad_I2C kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CKeyPadADDR, PCF8574);

void KeyPad::Initialize()
{
	kpd.begin(makeKeymap(keys));
}

PadKeys KeyPad::GetKey()
{
	 char key=kpd.getKey();
     if (!key)return Waite;

	 switch (key)
	 {
		case '0':
		  return K_0;
	 	case '1':
			return K_1;
		case '2':
			return K_2;
		case '3':
			return K_3;
		case '4':
			return K_4;
		case '5':
			return K_5;
		case '6':
			return K_6;
		case '7':
			return K_7;
		case '8':
			return K_8;
		case '9':
			return K_9;
		case 'A':
			return K_A;
		case 'B':
			return K_B;
		case 'C':
			return K_C;
		case 'D':
			return K_D;
		case '*':
			return K_SS;
		case '#':
			return K_EE;
		default:
			return Unknow;
	 }
	
}




