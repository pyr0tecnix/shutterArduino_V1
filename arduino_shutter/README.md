# README Shutter#
#Version 1 - 22/09/2015#
#Rédacteur : Patrice Vieyra#
#Contact : contact@magicofthings.fr
### What is this repository for? ###

* Présentation :

Shutter pour vidéoprojecteur, commandé via OSC.
Liste des commandes :
* Ouverture
* Fermeture
* Demande de position
* Reboot
* Déplacement à une position donnée

* V1
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

## Revue hardware ##

* Arduino UNO DIP-rev3
* Neuftech® Ethernet shield module W5100 avec module Micro SD
* Microservo tower pro SG90
* Adafruit motor/stepper/servo shield pour arduino V2.3 (facultatif)
* Carte microSD formatée en FAT32
## Câblage ##

Assembler les différents shield en les emboîtants

## Mise en route ##

* Connecter le câble ethernet
* Vérifier que la carte SD est bien insérée
* Brancher l’alimentation du shutter
* A la fin de l'initialisation le shutter se ferme et se rouvre : Après il est prêt à l'emploi
* Il est possible de brancher un câble USB pour avoir les informations de débug par liaison série (9600 baud)

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

#Fichier de configuration du shutter 22/09/2015 by Patrice Vieyra
#Toujours laisser une ligne vide à la fin

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

## Glossaire des trames ##

* shutter_on
	* Mets le shutter en position de travail (cache devant la lentille)
* shutter_off
	* Mets le shutter en position de repos (cache relevé)

* shutter_position
	* Envoie une demande de position au shutter qui répond /shutter_position/(int)

* shutter_reboot
	* Envoie une demande de reboot au shutter qui redémarre 1 seconde après

* shutter_move/(int)
	* Envoie une demande de déplacement au shutter : le int est compris entre 0(min) et 100(max).
		Le servomoteur se déplace alors d'une valeur comprise entre 0 et 180°.
		Attention le débatement angulaire est différent pour chaque référence de servomoteur.

## Bibliothèques utilisées ##

* Standards Arduino

SPI
SD
Ethernet
EthernetUdp
Servo


* OSC
http://cnmat.berkeley.edu/oscuino
http://cnmat.berkeley.edu/book/export/html/11389

## Tests ##

## Liens intéressants ##
http://cnmat.berkeley.edu/oscuino
http://cnmat.berkeley.edu/book/export/html/11389
http://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
https://www.nickv.codes/blog/arduino-osc-iphone-and-dmx/


### Who do I talk to? ###

pyr0tecnix - contact@magicofthings.fr
