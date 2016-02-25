
/*
  Arduino_shutterV1

  Librairie contenant les fonctions relatives au servomoteur

  Terminologie :

  shutter On = position de travail = cache baissé
  shutter Off = position de repos = cache relevé

  Branchement :

  Sur la PIN10 de l'Arduino (et entre 5V et 0V pour l'alimentation) ou sur Servo1 sur la platine Adafruit

  1. testServo : Fonction de test du servomoteur. Mets le shutter en position de travail, attends 2s et le remets en position de repos

  2. initServo : Fonction d'initialisation du servomoteur du shutter. Sur le PIN9 qui correspond au PIN9 de l'Arduino ou à Servo1 de la platine Adafruit
                 Le shutter est placé en position de repos

  3. openShutter : Fonction mettant le shutter en position de travail s'il était dans une position différente

  4. closeShutter : Fonction mettant le shutter en position de repos s'il était dans une position différente

  modifié le 25 février 2016
  par Patrice Vieyra
*/

#ifndef shutterServo
#define shutterServo

#include <Servo.h>

#define dShutterOn 105 // Position de travail, cache baissé
#define dShutterOff 5 // Position de repos, cache relevé
#define UNUSED(x) (void)(x) //Macro permettant d'éviter le warning unused but set parameter

Servo shutterServomoteur;  // Création de l'objet servomoteur
int positionShutter = 0; // Permet de conserver la position du servomoteur

void testServo() {
  shutterServomoteur.write(dShutterOn);
  delay(2000);
  shutterServomoteur.write(dShutterOff);
}

/* Fonction d'initialisation du servomoteur
   Le shutter est automatiquement mis en position de repos
*/
void initServo() {
  Serial.print("Initialisation du servomoteur...");
  shutterServomoteur.attach(9); //Connexion du servomoteur sur la PIN110 de l'aruino <=> Servo1 sur la platine Adafruit
  shutterServomoteur.write(positionShutter); //Initialisation du servomoteur à la position de repos
  positionShutter = dShutterOff;
  Serial.println("Ok");
}

/* Fonction permettant de mettre le shutter en position de repos s'il est dans une position différente
   Paramètres : référence vers un OSCMessage - inutilisée dans notre application
                   entier représentant un offset - inutilisé dans notre application
*/
void openShutter(OSCMessage &msg, int addrOffset) {

  /*Opérations permettant de supprimer les warnings de non utilisation des variables*/
  msg.empty();
  UNUSED(addrOffset);
  
  //Si le servomoteur est dans une position autre que sa position de repos, on le met en position de repos
  if (positionShutter != dShutterOff) {
    shutterServomoteur.write(dShutterOff);
    positionShutter = dShutterOff;
    Serial.println("Shutter ouvert");
  }

}

/* Fonction permettant de mettre le shutter en position de travail s'il est dans une position différente
   Paramètres : référence vers un OSCMessage - inutilisée dans notre application
                entier représentant un offset - inutilisé dans notre application
*/
void closeShutter(OSCMessage &msg, int addrOffset) {

  /*Opérations permettant de supprimer les warnings de non utilisation des variables*/
  msg.empty();
  UNUSED(addrOffset);
  
  //Si le servomoteur est dans une position autre que sa position de travail, on le met en position de travail
  if (positionShutter != dShutterOn) {
    shutterServomoteur.write(dShutterOn);
    positionShutter = dShutterOn;
    Serial.println("Shutter fermé");
  }

}

/* Fonction permettant mapper une valeur bornée dans les bornes du servomoteur
   Paramètres : référence vers un OSCMessage - va contenir la valeur entre 0 et 100
                entier représentant un offset - inutilisé dans notre application
*/
void moveShutter(OSCMessage &msg, int addrOffset) {
  /*Non testé*/
  /*Opérations permettant de supprimer les warnings de non utilisation des variables*/
  UNUSED(addrOffset);
  
  if (msg.isInt(0)) {
    positionShutter = msg.getInt(0);
    shutterServomoteur.write(map(positionShutter, 0, 100, 0, 180));
    Serial.println("Shutter en position :" + msg.getInt(0));
  }
}

#endif
