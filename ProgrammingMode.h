// ProgrammingMode.h

#ifndef _PROGRAMMINGMODE_h
#define _PROGRAMMINGMODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "ActionCommand.h"
#define MOTOR_LEFT_1 13
#define MOTOR_LEFT_2 12
#define MOTOR_RIGHT_1 0
#define MOTOR_RIGHT_2 2

#define MOTOR_LEFT_C1 0
#define MOTOR_LEFT_C2 1
#define MOTOR_RIGHT_C1 2
#define MOTOR_RIGHT_C2 4



void AgregateProgrammingCodes(Adafruit_SSD1306 *mdisplay);

#endif

