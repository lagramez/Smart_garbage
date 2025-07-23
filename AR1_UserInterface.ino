#include <SoftwareSerial.h>       	// Cette bibliothèque émule une voie série.

// BT Setup
SoftwareSerial mySerial(11, 10);  	// affectation des broches (RX, TX) et création de l'objet mySerial
int readData = 0; 			// Le nombre a lire par BT

// Communication série avec le 74HC595N
// Il nous permet de commander indépendamment 8 sorties digitales à partir de 3 pins
int latchPin = 4;   	// STCP 	- Latch (Storage Register Clock Input / STcp )
int clockPin = 7;   	// SHCP 	- Clock (Shift Register Clock / SHcp)
int dataPin = 6;    	// DataS 	- Data (Serial Data Input / DS)

void setup() {
  // Les pins de 74HC595N, mis on OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // La Baude de communiction entre les deux module BT
  // Les baudes ont ete configure avec AT-commands 
  Serial.begin(9600);
  mySerial.begin(9600); 
}

// UTF-8 code a envoyer vers 74HC595N pour afficher les nombres convenables
const int int_equivalant_of[11] =
{
  63,   // 0
  6,	// 1
  91,   // 2
  79,   // 3
  102,  // 4
  109,  // 5
  125,  // 6
  7,	// 7
  127,  // 8
  111,  // 9
  113   // 10 - F
};


// fonction pour afficher les nombres avec le 7 segment, en envoyant un valeur a 74HC595N
void update_to_nombre(int nombre)
{	// Bloque le latch
	digitalWrite(latchPin, LOW);
	
	// dataPin >> pour envoyer chaque bit 
	// clockPin >> vas etre osciller, 74HC595N enregistre le bit et effectue un décalage pour lire le bit suivant
	// MSBFIRST (Most Significant Bit First) >> comencer par le MSF
	// int_equivalant_of[nombre] >> c'est le byte a envoyer
	shiftOut(dataPin, clockPin, MSBFIRST, int_equivalant_of[nombre]);
	
	// Active le latch
	// Prendre en compte les modifications
	digitalWrite(latchPin, HIGH);
	delay(500);
}

void loop()
{
      	// Bouclez ici jusqu'à ce que l'entrée soit détectée
	while ( !mySerial.available() ); 
	
	// Lire l'entrée
	// readData = 3 symboles UTF-08  (input number, ctl U+000D,  ctl U+000A)
      	readData = mySerial.read(); // UTF-08 of +3symboles (input number+ ctl U+000D 13 + ctl U+000A 10)
	
      	// vérifier si les données lues sont valides
	// les nombres 0 - 9 sont compris entre 57 et 48
      	if ( readData <= 57 && readData >= 48) {
        	update_to_nombre(readData - 48); // UTF-08 : 0 = 48
      	} else if ( readData == 70) { // UTF-08 70 = F
        	update_to_nombre(10);
      	}
}
	

