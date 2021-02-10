// ActionCommand.h
#pragma once
#ifndef _ACTIONCOMMAND_h
#define _ACTIONCOMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



class CActionCommandItem
{
public:
	int state=0;
	int time=5;
	int speed=200;

};

class CActionCommandItems
{
public:
	CActionCommandItem leftTrack;
	CActionCommandItem rightTrack;
	CActionCommandItem light;
	CActionCommandItem fan;

};


class CActionCommand
{
	private:
		CActionCommandItems *_actionCommand;
		int _actionIndex = 0;
	public:
		CActionCommand()
		{
			_actionCommand = new CActionCommandItems[100];
			memset(_actionCommand, 0, sizeof(_actionCommand));
		}
		void Add(CActionCommandItems item)
		{
			_actionCommand[_actionIndex++] = item;
		}

		int Count()
		{
			return _actionIndex;
		}

		CActionCommandItems Item(int index)
		{
			if((index>=0)&&(index< _actionIndex))
			return _actionCommand[index];
		}
		

		~CActionCommand()
		{
			delete[] _actionCommand;
		}

};

#endif

