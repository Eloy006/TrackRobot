// 
// 
// 
#include "WemosKeypad.h"
#include "ProgrammingMode.h"

Adafruit_SSD1306 *_display;
char progCommandsCh[] = { 'A','A','1','0','*','0','5','#','A','B','1','0','#','A','A','1','0','#','#'};
int commandIndex = 0;
int commandLen = 19;



PadKeys GetInput()
{
	
	return keyPad.GetKey();
}



char GetNumName(PadKeys action)
{
	switch (action)
	{
	case K_A:
		return 'A';
	case K_B:
		return 'B';
	case K_C:
		return 'C';
	case K_D:
		return 'D';
	case K_SS:
		return '*';
	case K_EE:
		return '#';
	case K_0:
		return '0';
	case K_1:
		return '1';
	case K_2:
		return '2';
	case K_3:
		return '3';
	case K_4:
		return '4';
	case K_5:
		return '5';
	case K_6:
		return '6';
	case K_7:
		return '7';
	case K_8:
		return '8';
	case K_9:
		return '9';
	default:
		return 'U';
	}
}

bool IsCommand(PadKeys command)
{
	switch (command)
	{
		case K_A:
		case K_B:
		case K_C:
		case K_D:
		case K_EE:
		case K_SS:
			return true;
		default:
			return false;
	}
}

bool IsNumber(PadKeys command)
{
	switch (command)
	{
	case K_0:
	case K_1:
	case K_2:
	case K_3:
	case K_4:
	case K_5:
	case K_6:
	case K_7:
	case K_8:
	case K_9:
		return true;
	default:
		return false;
	}
}

int getNumber(PadKeys command)
{
	switch (command)
	{
	case K_0:
		return 0;
	case K_1:
		return 1;
	case K_2:
		return 2;
	case K_3:
		return 3;
	case K_4:
		return 4;
	case K_5:
		return 5;
	case K_6:
		return 6;
	case K_7:
		return 7;
	case K_8:
		return 8;
	case K_9:
		return 9;
		
	default:
		return -1;
	}
}


CActionCommand *actionCommand = new CActionCommand();


void WaiteContinue()
{
	delay(100);
}

void DoSubCommand(PadKeys*commandStack)
{
	PadKeys inputCommand = Unknow;
	String string="";
	int number=-1;

	int state = 0;
	int speed=0;
	int time = 0;
	CActionCommandItems commandItem;
	_display->print("|");
	while ((inputCommand = GetInput()) != Unknow)
	{
		if (inputCommand == Waite)
		{
			WaiteContinue();
			continue;
		}
		_display->print(GetNumName(inputCommand));
		_display->display();
		if ((number = getNumber(inputCommand)) != -1)
		{
	
			char s;
			itoa(number,&s,10);
			string += s;
		}

		if ((inputCommand == K_SS) || (inputCommand == K_EE))
		{
			if (state == 1) speed = atoi(string.c_str());
			if (state == 0) time = atoi(string.c_str());
			
			state++;
			string = "";
		}
		if(inputCommand==K_EE)
		{
			commandItem.leftTrack.time = time;
			commandItem.rightTrack.time = time;

			commandItem.leftTrack.speed = speed != 0 ? speed : commandItem.leftTrack.speed;
			commandItem.rightTrack.speed = speed != 0 ? speed : commandItem.leftTrack.speed;
			commandItem.leftTrack.state = 0;
			commandItem.rightTrack.state = 0;
	
			
			if ((commandStack[0] == K_A) && (commandStack[1] == K_A))
			{
				commandItem.leftTrack.state = 1;
				commandItem.rightTrack.state = 1;
			}

			if ((commandStack[0] == K_A) && (commandStack[1] == K_B))
			{
				commandItem.leftTrack.state = 1;
				commandItem.rightTrack.state = 2;
			}

			if ((commandStack[0] == K_B) && (commandStack[1] == K_A))
			{
				commandItem.leftTrack.state = 2;
				commandItem.rightTrack.state = 1;
			}

			if ((commandStack[0] == K_B) && (commandStack[1] == K_B))
			{
				commandItem.leftTrack.state = 2;
				commandItem.rightTrack.state = 2;
			}

			if ((commandStack[0] == K_D) && (commandStack[1] == K_D))
			{
				commandItem.light.state = time;
				commandItem.light.time = speed;
			}
		
			if ((commandStack[0] == K_C) && (commandStack[1] == K_C))
			{
				commandItem.fan.state = 1;
				commandItem.fan.time = speed;
			}
			actionCommand->Add(commandItem);
			_display->println("");
			return;
		}
		delay(100);
	}
	
}

void StopAllMotor()
{
	ledcWrite(MOTOR_LEFT_C1, 0);
	ledcWrite(MOTOR_LEFT_C2, 0);

	ledcWrite(MOTOR_RIGHT_C1, 0);
	ledcWrite(MOTOR_RIGHT_C2, 0);
}


void trackAction(CActionCommandItem leftTrack, CActionCommandItem rightTrack)
{
	if (leftTrack.state == 0)
	{
		ledcWrite(MOTOR_LEFT_C1, 0);
		ledcWrite(MOTOR_LEFT_C2, 0);
		
	}

	if(rightTrack.state==0)
	{
		ledcWrite(MOTOR_RIGHT_C1, 0);
		ledcWrite(MOTOR_RIGHT_C2, 0);
		
	}

	if(leftTrack.state==1)
	{
		ledcWrite(MOTOR_LEFT_C1, leftTrack.speed);
		ledcWrite(MOTOR_LEFT_C2, 0);
	}
	else
	if (leftTrack.state == 2)
	{
		ledcWrite(MOTOR_LEFT_C1, 0);
		ledcWrite(MOTOR_LEFT_C2, leftTrack.speed);
	}
	
	if (rightTrack.state == 1)
	{

		ledcWrite(MOTOR_RIGHT_C1, rightTrack.speed);
		ledcWrite(MOTOR_RIGHT_C2, 0);
	}
	else
	if(rightTrack.state == 2)
	{

		ledcWrite(MOTOR_RIGHT_C1, 0);
		ledcWrite(MOTOR_RIGHT_C2, rightTrack.speed);
	}
	sleep(leftTrack.time);
}


void DoMoveCommand(CActionCommandItems *action_command)
{

	trackAction(action_command->leftTrack, action_command->rightTrack);
	
	//action_command->fan
	if(action_command->fan.state==1)
	{
		digitalWrite(16, HIGH);
		delay(5000);
		digitalWrite(16, LOW);
	}
	
}

void fastePlayProgram()
{
	_display->clearDisplay();
	_display->setCursor(0, 0);
	_display->println("Faste play mode");
	_display->print("Total actions ");
	_display->println(actionCommand->Count());
	_display->display();
	

	for(int i=0; i<actionCommand->Count();i++)
	{
		_display->setCursor(0, _display->getCursorY());
		_display->print(i);
		_display->display();
		CActionCommandItems command = actionCommand->Item(i);
		DoMoveCommand(&command);
	}

	_display->println("");
	
}



void AgregateProgrammingCodes(Adafruit_SSD1306 *mdisplay)
{
	_display = mdisplay;
	
	PadKeys inputCommand=Unknow;
	PadKeys prevCommand = Unknow;
	PadKeys commandStack[2];
	int commandStackCount = 2;
	int indexCmdStack = 0;

	
	_display->clearDisplay();
	_display->setCursor(0, 0);
	_display->println(F("Input command... "));
	_display->display();
	

	while ((inputCommand = GetInput()) != Unknow)
	{
		if (inputCommand == Waite)
		{
			WaiteContinue();
			continue;
		}
		if (inputCommand == K_EE)
		{

			break;
		}
		prevCommand = inputCommand;
	
		
		
	
		if (!IsCommand(inputCommand))continue;
		_display->print(GetNumName(inputCommand));
		_display->display();
		commandStack[indexCmdStack++] = inputCommand;
		
		if (commandStackCount == indexCmdStack)
		{
			DoSubCommand(commandStack);
			memset(commandStack, 0, 4 * sizeof(char));
			indexCmdStack = 0;
			continue;
		}
		
		delay(100);
	}
	_display->println(F("Exit programming mode"));
	_display->display();
	sleep(3);
	fastePlayProgram();
	StopAllMotor();
	_display->println(F("All done."));
	_display->display();

	
	
}