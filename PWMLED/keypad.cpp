#include "keypad.h"
#include <Arduino.h>

int pressedKey = -1;
int pressedKeyLast = -2;


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

//Odotetaan, että näppäimistö painetaan.
void waitInput()
{
	//Sijotetaan globaliin "pressedKey"-muuttajaan syöte näppäimitöltä.
	//Looppi pyörii, kun näpäimistön luentafunktio palauttaa epäonnistumisen (näppäimistöä ei ole painettu).
	while ((pressedKey = readKeyPress()) == KEYPADFAILURE) {}
	//Kun funktio palauttaa jotain muuta kuin epäonnistumisen, looppi loppuu ja päästään eteen päin.
	//"pressedKey"-muuttaja sisältää painetun napin.
}