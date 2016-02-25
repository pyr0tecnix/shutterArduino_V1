
/*
  Arduino_shutterV1

  Programme de commande du shutterAM
  Protocole de commande : OSC
  Stockage de l'adresse IP sur carte miniSD

 Glossaire des trames : 

    /shutter_on/        Passe le shutter à l'état actif (cache devant la lentille du VP)
    /shutter_off/       Passe le shutter à l'état inactif (cache relevé)
    /shuter_position/   Requiert la position du shutter qui répond /shutter_position/p où p est un entier qui a deux valeurs possibles. 
                        La plus petite correspond à la position de repos (shutter inactif). Les valeurs correspondent directement à l'angle du servomoteur

  modifié le 25 février 2016
  par Patrice Vieyra
 */

#include <SPI.h> //Librairie qui gère le bus SPI. Ce bus est utilisé pour la communication avec la carte SD
#include <SD.h> //Librairie Arduino qui gère les cartes SD
#include <Ethernet.h> //Libraire Arduino qui gère l'ethernet
#include <OSCBundle.h> //Librairie OSC qui gére l'envoi et la réception de message
#include <EthernetUdp.h> //Librairie Arduino qui gère l'UDP
#include <Servo.h> // Librairie Arduino qui gère les servomoteurs

#include "shutter.h" //Librairie contenant les fonctions et types généraux du shutter

/*Déclarations des define*/
#define W5200_CS  10 //PIN utilisé par la communication ethernet
#define SDCARD_CS 4 //PIN utilisé par la communication avec la carte SD


/*Déclarations des variables globales*/

//Sélections du mode de commande
commandMode selectedMode;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // On inhibe la partie ethernet pour pouvoir utiliser la carte SD
  pinMode(W5200_CS, OUTPUT);
  digitalWrite(W5200_CS, HIGH);
  
  initSD();
  setupConfig("config.txt", &shutter_osc_ip, &master_osc_ip, &shutter_osc_port, &master_osc_port);

  // On inhibe la carte SD
  digitalWrite(SDCARD_CS, HIGH);

  //Récupération du mode de commande
  selectedMode = findCommandMode();

  initServo();

  initOSC();
}

// the loop function runs over and over again forever
void loop() {
  switch (selectedMode)
  {

    case osc:
      receiveOSC();
      break;

    case dmx :

      break;

    case manual :

      break;
  }

}
