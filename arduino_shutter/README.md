# README Shutter#
#Version 1 - 22/09/2015#
#Rédacteur : Patrice Vieyra#

### What is this repository for? ###

* Présentation :

Shutter pour vidéoprojecteur, commandé via OSC.
Ouverture et fermeture commandé par OSC, le shutter est également capable de renvoyer sa position.

* V1
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

## Revue hardware ##

* Arduino UNO DIP-rev3
* Neuftech® Ethernet shield module W5100 avec module Micro SD
* Microservo tower pro SG90
* Adafruit motor/stepper/servo shield pour arduino V2.3 (facultatif)

## Câblage ##


## Mise en route ##

* Connecter le câble ethernet
* Vérifier que la carte SD est bien insérée
* Brancher l’alimentation du shutter
* A la fin de l'initialisation le shutter se ferme et se rouvre : Après il est prêt à l'emploi
* Il est possible de brancher un câble USB pour avoir les informations de débug par liaison série (9600 baud)

## Revue software ##
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

## Tests ##

## Liens intéressants ##
http://cnmat.berkeley.edu/oscuino
http://cnmat.berkeley.edu/book/export/html/11389
http://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
https://www.nickv.codes/blog/arduino-osc-iphone-and-dmx/


### Who do I talk to? ###

pyrotecnix - contact@vieyra.fr