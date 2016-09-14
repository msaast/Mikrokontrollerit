
#define DIFFERENTCHAR 15
#define D 500 //Char display time
//													0			1			2			3			4			5			6			7			8			9			.			blank
const uint8_t sevenNumberBits[DIFFERENTCHAR] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b01000111, 0b01111111, 0b01101111, 0b10000000, 0b00000000,
0b01110111, 0b01111001, 0b01110001 };
//													A			E			F
const char sevenNumberChars[DIFFERENTCHAR] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ' ', 'A', 'E', 'F' };

char wordChar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '8', '6', '4', '2', '0', ' ' };
unsigned int wordDelay[] = { D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D,   D * 2 };
unsigned long wordTimeOld = 0;
unsigned int wordIndex = 0;
bool wordStart = true;
void setup()
{
	DDRD = 255;

}



void loop()
{
	//sevenSeg('0', false);
	displayCharacters(wordChar, wordDelay, sizeof(wordChar), wordTimeOld, wordIndex, wordStart);


}

int sevenSeg(char character, bool decimal)
{
	for (uint8_t i = 0; i < DIFFERENTCHAR; i++)
	{
		if (sevenNumberChars[i] == character)
		{
			if (decimal == true)
			{
				if (character == '.')
				{
					PORTD = sevenNumberBits[i];
				}
				else
				{
					PORTD = sevenNumberBits[i] + 0b10000000;
				}
				return EXIT_SUCCESS;
			}
			else
			{
				PORTD = sevenNumberBits[i];
				return EXIT_SUCCESS;
			}

		}
	}
	return EXIT_FAILURE;
}

void displayCharacters(char word[], unsigned int time[], unsigned int lenght, unsigned long &timeOld, unsigned int &patternIndex, bool &start)
{
	if (patternIndex < lenght)
	{
		if (patternIndex == 0 && start == true)
		{
			sevenSeg(word[patternIndex], false);
			start = false;
			timeOld = millis();
		}
		else
		{
			if (millis() - timeOld > time[patternIndex])
			{
				patternIndex++;
				sevenSeg(word[patternIndex], false);
				timeOld = millis();
			}
		}
	}
	else
	{
		patternIndex = 0;
		start = true;
	}

}