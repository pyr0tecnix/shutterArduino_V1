# README Shutter OSC
Version 1 - 24/02/2016


Shutter pour vidéoprojecteur, commandé via OSC.
Ouverture et fermeture commandé par OSC, le shutter est également capable de renvoyer sa position.


## Configuration hardware testée

* Micro-contrôleur :
	* [Arduino UNO DIP-rev3](http://boutique.semageek.com/fr/2-arduino-uno-dip-rev3-7640152110092.html)
* Shield ethernet :
	* [Neuftech® Ethernet shield module W5100 avec module Micro SD](http://www.amazon.fr/dp/B00PQA6G8O/ref=sr_ph?ie=UTF8&qid=1456398408&sr=1&keywords=neuftech)
* Servomoteur :
	* [Microservo tower pro SG90](http://boutique.semageek.com/fr/104-micro-servo-tower-pro-sg90.html)
	* [Servo numérique 8525 MG-D](http://www.a2pro-online.com/servo-numerique-7525-mg-d.html)


## Mise en route ##

* Connecter le câble ethernet
* Vérifier que la carte SD est bien insérée et contient bien le fichier de configuration **"config.txt"**
* Brancher l’alimentation du shutter
* A la fin de l'initialisation le shutter se ferme et se rouvre : après il est prêt à l'emploi
* Il est possible de brancher un câble USB pour avoir les informations de débug par liaison série (9600 baud)

## Lire les retours série de l'arduino ##

### Linux et Mac Os ###

Ouvrir le terminal et lancer `screen /dev/cu.usbmodem1411 9600`

Pour quitter `screen` appuyer sur **CTRL-A

### Windows ###

Utiliser [PuTTY](http://www.putty.org/)

## Glossaire des trames ##

* shutter_on
	* Mets le shutter en position de travail (cache devant la lentille)
* shutter_off
	* Mets le shutter en position de repos (cache relevé)

* shutter_position
	* Envoie une demande de position au shutter qui répond /shutter_position/(int)

* shutter_reboot
	* Envoie une demande de reboot au shutter

* shutter_move/(int)
	* Envoie une demande de déplacement au shutter : le int est compris entre 0(min) et 100(max)

## Bibliothèques utilisées ##
* Standards Arduino :
	* SPI
	* SD
	* Ethernet
	* EthernetUdp
	* Servo
* OSC
	* http://cnmat.berkeley.edu/oscuino
	* http://cnmat.berkeley.edu/book/export/html/11389

## Revue software ##

 * arduino_shutter.ino

 * shutter.h

 Librairie contenant les fonctions et types généraux du shutter

 1 . commandMode : Enumération des modes de pilotage du shutter : OSC - DMX - Manuel

 2 . findCommandMode : Fonction renvoyant le mode de pilotage sélectionné. Pour le moment le mode OSC est fixé en dur


 * shutterOSC.h

 Librairie contenant les fonctions relatives à l'OSC

 Librairie utilisée : OSC for Arduino and Related Microcontrollers 1.3c
 Lien : http://cnmat.berkeley.edu/oscuino
 			 https://github.com/CNMAT/OSC

 1 . initOSC : Fonction d'initialisation de la liaison ethernet qui sert de support au protocole OSC.
 							Les adresses IP sont statiques et stockées sur la carte SD avec les ports entrant et sortant.
 							A la fin de l'initialisation on appelle testShutter qui ouvre puis ferme le shutter

 2 . getPositionShutter : Fonction qui renvoie la position actuelle du shutter sous forme d'entier.
 												 Cette position correspond directement à l'angle du servomoteur. Il n'y a
 												 que deux valeurs possibles, la plus petite correspond à la position de repos
 												 shutter inactif.

 3 . rebootShutter : Fonction permettant de faire un soft reboot du shutter. Il y a plusieurs valeurs de timeout prédéfinies. Nous utilisons 1S

 4 . receiveOSC : Fonction chargée de recevoir les trames OSC et faire le dispatch
 									/shutter_on       closeShutter()  située dans shutterServo
 									/shutter_off      openShutter()   située dans shutterServo
 									/shutter_position getPositionShutter()
 									/shutter_reboot   shutterReboot()
 									/shutter_move     shutterMove() située dans shutterServo

 * shutterSD.h

 Librairie contenant les librairies propres à la manipulation de la carte SD
 La carte SD sert ici à concerver le fichier de configuration qui doit imperativement s'appeler CONFIG.TXT

 Exemple de ficher :

	\#Fichier de configuration du shutter 22/09/2015 by Patrice Vieyra
	\#Toujours laisser une ligne vide à la fin

	shutter_osc_Ip : 192.168.0.9
	shutter_osc_Port : 8000
	master_osc_Ip : 192.168.0.40
	master_osc_Port : 9000
	shutter_dmx_channel : 201
	shutter_dmx_nb : 3


 Les éléments relatifs au DMX sont juste là à titre indicatif et ne sont pas utilisés dans cette version

 1 . initSD : Fonction d'initialisation de la communication avec la carte SD

 2. setupConfig : Fonction de lecture de la config. Les valeurs lues servent à mettre à jour les variables du programme

 * shutterServo.h

 Librairie contenant les fonctions relatives au servomoteur

 Terminologie :

 shutter On = position de travail = cache baissé
 shutter Off = position de repos = cache relevé

 Branchement :

 Sur la PIN10 de l'Arduino (et entre 5V et 0V pour l'alimentation) ou sur Servo1 sur la platine Adafruit

 1 . testServo : Fonction de test du servomoteur. Mets le shutter en position de travail, attends 2s et le remets en position de repos

 2 . initServo : Fonction d'initialisation du servomoteur du shutter. Sur le PIN9 qui correspond au PIN9 de l'Arduino ou à Servo1 de la platine Adafruit
 							 Le shutter est placé en position de repos

 3 . openShutter : Fonction mettant le shutter en position de travail s'il était dans une position différente

 4 . closeShutter : Fonction mettant le shutter en position de repos s'il était dans une position différente


## Liens intéressants ##

* http://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
* https://www.nickv.codes/blog/arduino-osc-iphone-and-dmx/
* https://skyduino.wordpress.com/2012/06/19/arduino-charger-un-fichier-de-configuration-depuis-une-carte-sd/

### Contact ###

pyrotecnix - contact@magicofthings.fr
