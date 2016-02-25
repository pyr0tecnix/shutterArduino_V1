
/*
  shutter.h

  Librairie contenant les fonctions et types généraux du shutter

  1 . commandMode : Enumération des modes de commande du shutter : OSC - DMX - Manuel

  2 . findCommandMode : Fonction renvoyant le mode de commande sélectionné. Pour le moment le mode OSC est fixé en dur
  

  modifié le 25 février 2016
  par Patrice Vieyra
 */

#ifndef shutter
#define shutter

#include "shutterSD.h" //Librairie contenant les fonctions liées à la manipulation de la carte SD
#include "shutterOSC.h" //Librairie contenant les fonctions liées au protocole OSC



/*Définitions des variables*/

/*Enumération listant les différents modes de contrôles du shutter : OSC - DMX - Manuel
 */
enum commandMode {
  osc,
  manual,
  dmx
};


/*Définitions des fonctions*/

/* Retour : une variable de type commandMode indiquant le mode de contrôle du shutter sélectionné
 * Le mode est fixé en dur pour le moment
 */
commandMode findCommandMode() {

  return osc;
}



#endif
