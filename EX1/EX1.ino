//https://upload.wikimedia.org/wikipedia/commons/b/b5/International_Morse_Code.svg
#define dot 100
#define dash dot * 3
#define space dot
#define spaceLetter dot * 3
#define spaceWord dot * 7

unsigned long sosTime = 0;
bool sosLedHigh = false;
unsigned int sosIndex = 0;

unsigned int sos[] = { dot, dot, dot, dash, dash, dash, dot, dot, dot };

void blingPattern(unsigned int pattern[], unsigned int length, unsigned long &timeOld, unsigned int &patternIndex, bool &ledHigh);
void ledControl(uint8_t status);

void setup()
{
	// D-register pin 3 to output = arduino nano pin D3
	DDRD = DDRD | 0b00001000;
	//Arduino LED
	DDRB = DDRB | 0b00100000;
}

void loop()
{
	blingPattern(sos, sizeof(sos) / sizeof(int), sosTime, sosIndex, sosLedHigh);
}

void ledControl(uint8_t status)
{
	if (status == HIGH)
	{
		//Lue rekisterin bittien tilat ykkösillä AND-operattorilla. 0 nollaa halutun bitin. OR-operaattorilla kijoitettaan haluttu tila nollattuun bittiin.
		PORTD = (PORTD & 0b11110111) | 0b1000;
		PORTB = (PORTB & 0b11011111) | 0b100000;
	}
	else if (status == LOW)
	{
		PORTD = PORTD & 0b11110111;
		PORTB = PORTB & 0b11011111;
	}
}

void blingPattern(unsigned int pattern[], unsigned int length, unsigned long &timeOld, unsigned int &patternIndex, bool &ledHigh)
{
	if (patternIndex < length)
	{
		if (ledHigh == true)
		{
			if (millis() - timeOld > pattern[patternIndex])
			{
				ledControl(LOW);
				ledHigh = false;
				patternIndex++;
				timeOld = millis();
			}
		}
		else
		{
			if (millis() - timeOld > space)
			{
				ledControl(HIGH);
				ledHigh = true;
				timeOld = millis();
			}
		}
	}
	else
	{
		if (millis() - timeOld > spaceWord)
		{
			patternIndex = 0;
			timeOld = millis();
		}
	}
}