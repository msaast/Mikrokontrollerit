#include <LiquidCrystal.h>
#include "keypad.h"

#define LEDOUT 6
#define ANALOGIN A4

#define RSlcd 1
#define EnableLcd 0
#define data0Lcd 5
#define data1Lcd 4
#define data2Lcd 3
#define data3Lcd 2

#define rowsLcd 2
#define columnsLcd 16

LiquidCrystal nkn(RSlcd, EnableLcd, data0Lcd, data1Lcd, data2Lcd, data3Lcd);

void setup()
{
	nkn.begin(columnsLcd, rowsLcd);

	analogWrite(LEDOUT, 1);
	OCR0A = 0;

	pinMode(ANALOGIN, INPUT);

	//C-portti input ylösveto
	//Sarakkeet
	DDRC = 0b000000000;
	PORTC = 0b00001111;

	//B-portti output
	//Rivit
	DDRB = 0b00001111;
	PORTB = 0b00000000;

}


void loop()
{

	//ledBrightnessKeypad();
	//ledBrightness2();
	ledBrightnessPot();
}


void ledBrightness()
{
	#define second 1000
	const int delayTo2Sec = (2000 / float(0xFF));
	const int delayTo1_5Sec = (1500 / float(0xFF));

	for (size_t i = 0; i < 255; i++)
	{
		OCR0A = i;
		delay(delayTo2Sec);
	}
	for (size_t i = 0; i < 255; i++)
	{
		OCR0A = 255 - i;
		delay(delayTo1_5Sec);
	}

	OCR0A = 0;
	delay(second);
}

void ledBrightness2()
{
	#define second 1000

	unsigned long timeIn = millis(), time = 0;

	//Suoran yhtälö y-y0=k(x-x0) => y=k*x-k*x0+y0
	//Kiihdytys suoranfunktio f(x)=k*x+b
	//Kulmakerroin k=(y0-y1)/(x0-x1)
	//Vakio b=-k*x0+y0

	 float k = 0xFF / float(2000);
	do
	{
		time = millis() - timeIn;

		OCR0A = int(k * time);

	} while (time < 2000);

	timeIn = millis();
	k = -0xFF / float(1500);
	do
	{
		time = millis() - timeIn;

		OCR0A = int(255 +  k * time);

	} while (time < 1500);


	OCR0A = 0;
	delay(second);
}

void ledBrightnessKeypad()
{
	const float  k = 1.5;
	double brightness = 0;
	int tempKey;
	while (true)
	{
		tempKey = readKeyHit();

		if (tempKey != KEYPADFAILURE)
		{
			if (tempKey == K8)
			{
				brightness = 255;
				OCR0A = 255;
			}
			else if (tempKey == K0)
			{
				brightness = 0;
				OCR0A = 0;
			}
			else
			{
				while (readKeyHit() == K3)
				{
					brightness += k;
					ledPWM(brightness);
				}
				while (readKeyHit() == K1)
				{
					brightness -= k;
					ledPWM(brightness);
				}
			}

		}

	}
}

void ledPWM(double& brightness)
{
	if (brightness > 255)
	{
		OCR0A = 255;
		brightness = 255;
	}
	else if (brightness < 0)
	{
		OCR0A = 0;
		brightness = 0;
	}
	else
	{
		OCR0A = round(brightness);
	}
}

void ledBrightnessPot()
{
	#define max  1023
	#define half 511

	const float step = 256 / float(20);
	const float step2 = 512 / float(20);
	
	int input;
	int output = 0, outputLast = 1, outPutLED;


	while (true)
	{
		input = analogRead(ANALOGIN);

		if (input > half)
		{
			//outPutLED = map((1023 - input), 0, half, 0, 255);

			for (uint8_t i = 1; i == 20; i++)
			{
				if ((i * step2) > (input - half))
				{
					output = i - 1;
					OCR0A = round(i * step);
					break;
				}
			}
		}
		else if (input < half)
		{
			//outPutLED = map(input, 0, half, 0, 255);

			for (uint8_t i = 1; i == 20; i++)
			{
				if ((i * step2) > input)
				{
					output = -(i - 1);
					OCR0A = round(i * step);
					break;
				}
			}
		}

		if (output != outputLast)
		{

			nkn.clear();
			nkn.setCursor(7, 0);
			nkn.print(output);
			nkn.setCursor(7, 1);
			nkn.print(input);
		}


	}
}