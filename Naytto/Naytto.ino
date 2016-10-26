#include <LiquidCrystal.h>

#define K0 0
#define K1 1
#define K2 2
#define K3 3
#define K4 4
#define K5 5
#define K6 6
#define K7 7
#define K8 8
#define K9 9
#define KA 10
#define KB 11
#define KC 12
#define KD 13
#define KE 14
#define KF 15
#define Kstar 16
#define Khastag 17

#define keyDelay 5 //ms

#define RSlcd 1
#define EnableLcd 0
#define data0Lcd 5
#define data1Lcd 4
#define data2Lcd 3
#define data3Lcd 2

#define rowsLcd 2
#define columnsLcd 16

#define KEYPADFAILURE -100

/*
const char keys[4][4] =
{ { 'A', '3', '2', '1' },
{ 'B', '6', '5', '4' },
{ 'C', '9', '8', '7' },
{ 'D', '#', '0', '*' }
};
*/

const uint8_t keys[4][4] =
{ { KA, K3, K2, K1 },
{ KB, K6, K5, K4 },
{ KC, K9, K8, K7 },
{ KD, Khastag, K0, Kstar }
};

const char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '*', '#' };

//nkn = nestekidenäyttö
LiquidCrystal nkn(RSlcd, EnableLcd, data0Lcd, data1Lcd, data2Lcd, data3Lcd); 

int pressedKey = -1;
int pressedKeyLast = -2;

void setup()
{
	nkn.begin(columnsLcd, rowsLcd);

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
	aPlusB();

	//setValue();
	//printTo50();
}


int readKeyHit()
{
	//Tämä funktio tutkii mikä näppäin on painettuna.
	//Jos haluaa lukea näppäimistöä, tämän funkiton on pyörittävä jatkuvasti.
	//Mikrokontrolleri testaa näppäin näppäimeltä onko se painettuna.

	//Alustetaan maskit
	uint8_t mask1 = 0b1110; //Rivit
	uint8_t mask2 = 0b1110; //Sarakkeet

	//Ulompi for-looppi käy läpi näppäimistön rivit testateekseen onko jotain sen saraketta painettu.
	for (uint8_t i = 0; i < 4; i++)
	{
		//Asetetaan B-portti rivi-maskin mukaiseksi.
		//1. kierros 00001110, 2. 00001101, 3. 00001011, 4. 00000111
		//0 = maa (GND)
		PORTB = mask1 & 0b1111; 
		//Kun joku rivi on astuettu maahan, voidaan testata mitä sen rivin saraketta on painettu.
		//Kaikki rivit käydään erikseen läpi.

		mask2 = 0b1110; //Asetetaan sarakkeiden maski alku asentoon.

		//Sisempi for-looppi tarkistaa mitä saraketta on painettu.
		for (uint8_t j = 0; j < 4; j++)
		{
			//Luetaan sarakkeiden input (C-porttia, neljä vähiten merkitsevää bittiä).
			//Jos jotain tällä hetkellä testattavan rivin saraketta on painettu.
			if (0b1111 != (PINC & 0b1111))
			{
				delay(keyDelay); //Odotetaan vähän aikaa, että näppäin asettuu.

				// Luetaan näppäimistö uudelleen ja tarkistetaan sarake-maskilla, mitä rivin saraketta on painettu.
				if ((mask2 & 0b1111) == (PINC & 0b1111))
				{
					//Palautetaan kaksiulotteisestataulukosta painetun näppäimen arvo.
					//Indeksoidaan taulukkukko looppien laskureiden avulla. i on rivi ja j on sarake.
					//Esim.: Painetaan nappia 5. Se on toisella rivillä (ylhäältä alas) ja kolmannessa sarakkeessa (vasmmalta oikealle).
					//Joten ulompi for-looppi pitää olla toisella kierroksella (i = 1) ja sisempi kolmannella (j = 2).
					//Palautetaan keys[1][2], minkä arvo on 5. Loopit jää kesken, koska löydettiin osuma.
					return keys[i][j];
				}
			}
			//Muokataan sarake-maskia.
			mask2 = mask2 << 1; //1110 -> 1100
			mask2++; //1100 -> 1101
			//Loopin alkuu -> Testataan seuraava sarake.
		}
		//Mukataan rivi-maskia.
		mask1 = mask1 << 1; //1110 -> 1100
		mask1++; //1100 -> 1101
		//Loopin alkuu -> Testataan seuraava rivi.
	}
	
	//Ei osumia, näppäimistöä ei ole painettu. Palatetaan epäonnistuminen.
	return KEYPADFAILURE;
}

//Tämä funktio palauttaa näppäimen arvon, kun näppäin päästetään ylös.
int readKeyPress()
{
	int temp = readKeyHit(); //Luetaan mikä näppäin on painettuna väliaikais muuttujaan.
	//Jos joku näppain on painettuna.
	if (temp >= 0)
	{
		//Pyöritetään looppia niin kauvan kuin sama nappi on painettuna.
		while (temp == readKeyHit()) {}
		//Nappi päästettiin ylös.
		delay(keyDelay); //Odotetaan, että nappi asettuu.

		//Tarkistetaan uudelleen, että nappi on ylhäällä.
		if (readKeyHit() == KEYPADFAILURE)
		{
			//Nappi päästetty ylös onnistuneesti. Palautetaan pohjassa olleen napin arvo.
			return temp;
		}
	}
	//Näppäimistöä ei ole painettu. Palautetaan epäonnistuminen.
	return KEYPADFAILURE;
}

void printTo50()
{
	const uint8_t iMax = 50;
	for (uint8_t i = 0; i < iMax; i++)
	{
		nkn.clear();

		nkn.setCursor(0, 0);
		nkn.print(i);
		nkn.setCursor(0, 1);
		nkn.print(sqrt(i), 3);

		nkn.setCursor(columnsLcd - 2, 0);
		nkn.print(iMax - i);
		nkn.setCursor(columnsLcd - 3, 1);
		nkn.print(-(iMax - i));
		
		delay(400);
	}
}

void displayKeyLcd()
{
	waitInput();

	if (pressedKey != pressedKeyLast )
	{
		nkn.clear();
		nkn.setCursor(columnsLcd / 2 - 1, 0);
		nkn.print(chars[pressedKey]);
	}
}

void aPlusB()
{
	#define firstChar 4
	#define row 0
	nkn.clear();
	nkn.setCursor(firstChar, row);
	const char line[] = { " + =" };
	nkn.print(line);
	int temp0, temp1;

	waitInput();

	temp0 = pressedKey;
	nkn.setCursor(firstChar, row);
	nkn.print(chars[temp0]);

	waitInput();

	temp1 = pressedKey;

	nkn.setCursor(firstChar + 2, row);
	nkn.print(chars[temp1]);
	nkn.setCursor(firstChar + sizeof(line) - 1, row);

	nkn.print(temp0 + temp1);

	waitInput();
}

int setValue()
{
	int value = 0;
	while (true)
	{
		nkn.clear();
		nkn.setCursor(0, 0);
		char line[] = { "Set value=" };
		nkn.print(line);
		nkn.setCursor(sizeof(line) + 1, 0);
		nkn.print(value);

		waitInput();

		switch (chars[pressedKey])
		{
		case '1':
			value++;
			break;
		case '4':
			value--;
			break;
		case '3':
			value+= 10;
			break;
		case '6':
			value-= 10;
			break;
		case 'D':
			return value;
			break;
		default:
			break;
		}
	}	
}

//Odotetaan, että näppäimistö painetaan.
void waitInput()
{
	//Sijotetaan globaliin "pressedKey"-muuttajaan syöte näppäimitöltä.
	//Looppi pyörii, kun näpäimistön luentafunktio palauttaa epäonnistumisen (näppäimistöä ei ole painettu).
	while ((pressedKey = readKeyPress()) == KEYPADFAILURE) {}
	//Kun funktio palauttaa jotain muuta kuin epäonnistumisen, looppi loppuu ja päästään eteen päin.
	//"pressedKey"-muuttaja sisältää painetun napin.
}