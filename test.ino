/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/
// Загружаем библиотеку для WiFi:
#include "WemosKeypad.h"
#include "ActionCommand.h"
#include "ProgrammingMode.h"
#include <WiFi.h>
#include <Wire.h>







#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);





WiFiServer server(80);
const char* ssid = "ESP32-Robot";
const char* password = "123456789";

int currentMode = 0;

int currentState = 0;

const int freq = 5000;

const int resolution = 8;



void setup() {
	Serial.begin(115200);
	analogReadResolution(12);
	analogSetWidth(12);
	analogSetAttenuation(ADC_11db);
	analogSetClockDiv(255);


	

	
//	adcAttachPin(15);
	
	
	// Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
	Wire.begin(5, 4);

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;);
	}
	//ADC12

	ledcSetup(MOTOR_LEFT_C1, freq, resolution);
	ledcSetup(MOTOR_LEFT_C2, freq, resolution);
	ledcSetup(MOTOR_RIGHT_C1, freq, resolution);
	ledcSetup(MOTOR_RIGHT_C2, freq, resolution);

	ledcAttachPin(MOTOR_RIGHT_1, MOTOR_RIGHT_C1);
	ledcAttachPin(MOTOR_RIGHT_2, MOTOR_RIGHT_C2);

	ledcAttachPin(MOTOR_LEFT_1, MOTOR_LEFT_C1);
	ledcAttachPin(MOTOR_LEFT_2, MOTOR_LEFT_C2);

	ledcWrite(MOTOR_LEFT_C1, 0);
	ledcWrite(MOTOR_LEFT_C2, 0);

	ledcWrite(MOTOR_RIGHT_C1, 0);
	ledcWrite(MOTOR_RIGHT_C2, 0);
	
	/*
	pinMode(MOTOR_RIGHT_2, OUTPUT);
	pinMode(MOTOR_RIGHT_1, OUTPUT);

	pinMode(MOTOR_LEFT_2, OUTPUT);
	pinMode(MOTOR_LEFT_1, OUTPUT);

	digitalWrite(MOTOR_LEFT_1,0);
	digitalWrite(MOTOR_LEFT_2, 0);

	digitalWrite(MOTOR_RIGHT_1, 0);
	digitalWrite(MOTOR_RIGHT_2, 0);
	*/
	
	
	delay(2000); // Pause for 2 seconds
	
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE, BLACK);
	
	display.setCursor(0, 0);
	//21x8
	display.println(F("Select mode:"));
	display.println(F("1. Manual"));
	display.println(F("2. KeyLoger"));
	display.display();
	//15
	pinMode(15, INPUT);

	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
	keyPad.Initialize();
	
	/*CreateWifiPoint();
	
	server.begin();
	
	showApInfo();
	*/
	// Clear the buffer.

}



void CreateWifiPoint()
{
	display.println("Create WiFi AP");
	delay(2000);
	display.display();
	WiFi.softAP(ssid, password);
	display.println("ok");
	display.display();
	delay(1000);
}


void showRSSI()
{
	long rssi = WiFi.RSSI();
	display.print("RSSI:");
	display.print(rssi);
	display.println(" dBm");
	display.display();
}

void showApInfo()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	IPAddress IP = WiFi.softAPIP();
	display.print("IP: ");  //  "IP-адрес точки доступа"
	display.println(IP);

	display.print("SSID: ");
	display.println(ssid);
	display.print("PWD: ");
	display.println(password);
	display.display();
	showRSSI();
}

void showConnectInfo(WiFiClient *client)
{
	display.clearDisplay();
	display.setCursor(0, 0);
	display.println("Client connected...");
	display.print("IP: ");
	display.println(client->remoteIP());
	display.display();
}

void showDisconnect()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	display.println("Client disconnected");
	display.display();
}

char waitChar[] = {'\\','|','/','-'};
int mChar = 3;
int mIndex = 0;

void animatedCursor()
{
	if (mIndex > mChar) mIndex = 0;

	display.setCursor(0, display.getCursorY());
	display.print(waitChar[mIndex++]);
	display.display();

	delay(100);


}

void prepareClient(WiFiClient *client)
{
	showConnectInfo(client);
	String currentLine = "";
	while (client->connected())
	{


		if (client->available())
		{
			animatedCursor();
			char c = client->read();


			client->println("HTTP/1.1 200 OK");
			client->println("Content-type:text/html");
			client->println("Connection: close");
			//  "Соединение: отключено"
			client->println();
			client->println();
		}

		client->println();
	}
}

int readKey()
{

}

void RunProgrammingMode()
{
	if (currentState == 0)
	{
		AgregateProgrammingCodes(&display);
		currentState = 0;
	}

}


void changeMode(int newMode)
{
	if (newMode == currentMode)return;

	if (newMode == 3)
	{
		display.clearDisplay();
		display.println(F("Initialize..."));
		display.display();
		sleep(1);
		CreateWifiPoint();
		server.begin();
		showApInfo();
		
	}
	display.println("Change mode");
	display.display();
	currentMode = newMode;

}

char GetNumName2(PadKeys action)
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


void KeyLogger()
{
	//uint16_t a=analogRead(15);
	
	char key = keyPad.GetKey();
	//key.key = Unknow;
	//key.max= analogRead(15);
	//key.min = key.max;
	if(key)
	{
		display.setCursor(0, 0);
		display.clearDisplay();
		display.println(key);
		
		
		display.display();
	}

	return;
}



void loop() {
	/*
	if (char key = keyPadConsole.GetProgKey()) {
		Serial << "Key: " << key << " State: ";
		
		
		switch (keyPadConsole.KeyState()) {
		case PRESSED:
			Serial.println("PRESSED");
			display.setCursor(0, 0);
			display.clearDisplay();
			display.println("PRESSED");
			display.println(key);
			display.display();
			break;

		case RELEASED:
			Serial.println("RELEASED");
			display.setCursor(0, 0);
			display.clearDisplay();
			display.println("RELEASED");
			display.println(key);
			display.display();
			break;

		case HELD:
			Serial.println("HOLDING");
			display.setCursor(0, 0);
			display.clearDisplay();
			display.println("HOLDING");
			display.println(key);
			display.display();
			break;
		}
	}
	/**/

	
	//KeyLogger();
	//return;
	if (currentMode == 2)
	{
		KeyLogger();
		return;
	}
	if (currentMode == 0)
	{
		int16_t key= keyPad.GetKey();
		if (key == K_1)
		{
			changeMode(1);

				return;
		}
		if(key==K_2)
		{
			changeMode(2);
		}
	}

	if (currentMode == 1)
	{
		display.println("RunProgrammingMode");
		
		RunProgrammingMode();
	}
	
	/*
	if (currentMode == 3)
	{
		WiFiClient client = server.available();



		if (client)
		{
			prepareClient(&client);
			client.stop();
			showApInfo();
		}

		delay(1000);
		animatedCursor();
	}
	*/
}