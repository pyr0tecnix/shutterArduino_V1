
/*
  shutterOSC.h

  Librairie contenant les fonctions relatives à l'OSC

  Librairie utilisée : OSC for Arduino and Related Microcontrollers 1.3c
  Lien : http://cnmat.berkeley.edu/oscuino
         https://github.com/CNMAT/OSC


  Glossaire des trames :

    /shutter_on/        Passe le shutter à l'état actif (cache devant la lentille du VP)
    /shutter_off/       Passe le shutter à l'état inactif (cache relevé)
    /shuter_position/   Requiert la position du shutter qui répond /shutter_position/p où p est un entier qui a deux valeurs possibles.
                        La plus petite correspond à la position de repos (shutter inactif). Les valeurs correspondent directement à l'angle du servomoteur
    /shutter_reboot     Redémarre le shutter
    /shutter_move       Déplace le servomoteur à la position demandée entre 0 et 100 qui corresponde à la position min et max du servomoteur

  1 . initOSC : Fonction d'initialisation de la liaison ethernet qui sert de support au protocole OSC.
                Les adresses IP sont statiques et stockées sur la carte SD avec les ports entrant et sortant.
                A la fin de l'initialisation on appelle testShutter qui ouvre puis ferme le shutter

  2 . getPositionShutter : Fonction qui renvoie la position actuelle du shutter sous forme d'entier.
                           Cette position correspond directement à l'angle du servomoteur. Il n'y a
                           que deux valeurs possibles, la plus petite correspond à la position de repos
                           shutter inactif.
                           
  3. rebootShutter : Fonction permettant de faire un soft reboot du shutter. Il y a plusieurs valeurs de timeout prédéfinies. Nous utilisons 1S

  4. receiveOSC : Fonction chargée de recevoir les trames OSC et faire le dispatch
                    /shutter_on       closeShutter()  située dans shutterServo
                    /shutter_off      openShutter()   située dans shutterServo
                    /shutter_position getPositionShutter()
                    /shutter_reboot   shutterReboot()
                    /shutter_move     shutterMove() située dans shutterServo

  modifié le 10 novembre 2015
  par Patrice Vieyra
 */

#ifndef shutterOSC
#define shutterOSC

#include <Ethernet.h> //Libraire Arduino qui gère l'ethernet
#include <EthernetUdp.h> //Librairie Arduino qui gère l'UDP
#include <OSCBundle.h> //Librairie OSC qui gére l'envoi et la réception de message
#include <avr/wdt.h> //Librairie utilisée pour le reset software de l'Arduino
#include "shutterServo.h" //Librairie qui contient les fonctions propres au servomoteur

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //Adresse MAC du shield ethernet. Se lit sur l'autocollant présent dans le boîter ou se défini aléatoirement en faisont attention à ne pas utiliser une adresse MAC utilisée par un autre appareil
IPAddress shutter_osc_ip; //Adresse IP du shutter stockée sur la carte SD
IPAddress master_osc_ip; //Adresse IP du maître stockée sur la carte SD
unsigned int shutter_osc_port; //Port d'écoute du shutter stockée sur la carte SD
unsigned int master_osc_port; //Port d'envoi vers le maître stockée sur la carte SD
EthernetUDP Udp;

/*
 * Fonction d'initialisation de la communication via OSC
 * Cette fonction doit être appelée après avoir récupéré le fichier de configuration stockée dans la carte SD
 * A la fin de l'initialisation on test le shutter en envoyant les commandes d'ouverture et de fermeture à la suite
 */
void initOSC() {

  Serial.print("Initialisation OSC...");
  Ethernet.begin(mac, shutter_osc_ip);
  Udp.begin(shutter_osc_port);
  Serial.println("Ok");
  Serial.print("Adresse IP du shutter : ");
  Serial.print(Ethernet.localIP());

  Serial.print(" Port : ");
  Serial.println(shutter_osc_port);

  Serial.print("Adresse IP du contrôlleur : ");
  Serial.print(master_osc_ip);

  Serial.print(" Port : ");
  Serial.println(master_osc_port);
  testServo();
}
/* Fonction qui envoie la position du shutter
 * Paramètres : référence vers un OSCMessage - inutilisée dans notre application
 *              entier représentant un offset - inutilisé dans notre application
 */
void getPositionShutter(OSCMessage &msg, int addrOffset) {
  msg.empty();
  msg.add("/shutter_position/");
  msg.add((int)positionShutter);
  Serial.print("Envoi de la position : ");
  Serial.println(positionShutter);
  Udp.beginPacket(master_osc_ip, master_osc_port); //Début de l'émission et envoie de l'entête
  msg.send(Udp); // Envoie du paquet UDP
  Udp.endPacket(); // Envoie du fin de transmissions

  msg.empty(); // free space occupied by message
}

/*
 * Fonction de soft reset de l'Arduino. Basée sur le Watchdog, on reset tout le hardware
 */
void rebootShutter(OSCMessage &msg, int addrOffset) {
  /* Différentes valeurs de Timeout
    WDTO_15MS
    WDTO_30MS
    WDTO_60MS
    WDTO_120MS
    WDTO_250MS
    WDTO_500MS
    WDTO_1S
    WDTO_2S
    WDTO_4S
    WDTO_8S
  */
  Serial.println("Le shutter va redémarrer dans 1s..");
  wdt_enable(WDTO_1S);
  while (1)
  {
  }
}

/* Fonction de réception des trames OSC
 * Cette fonction s'occupe également de dispatcher les commandes
 * /shutter_on       closeShutter()  située dans shutterServo
 * /shutter_off      openShutter()   située dans shutterServo
 * /shutter_position getPositionShutter()
 */
void receiveOSC() {
  OSCMessage msgIN;
  int size;
  if ((size = Udp.parsePacket()) > 0) {
    while (size--)
      msgIN.fill(Udp.read());
    if (!msgIN.hasError()) {
      msgIN.route("/shutter_on", closeShutter);
      msgIN.route("/shutter_off", openShutter);
      msgIN.route("/shutter_position", getPositionShutter);
      msgIN.route("/shutter_reboot", rebootShutter);
      msgIN.route("/shutter_move", moveShutter);

    }
  }
}



#endif
