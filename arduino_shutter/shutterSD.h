
/*
  Arduino_shutterV1

  Librairie contenant les librairies propres à la manipulation de la carte SD
  LA carte SD sert ici à concerver le fichier de configuration qui doit imperativement s'appeler CONFIG.TXT

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

  modifié le 25 février 2016
  par Patrice Vieyra
*/

#ifndef shutterSD
#define shutterSD

#include <SPI.h>
#include <SD.h> //Librairie Arduino qui gère les cartes SD
#include <String.h> //Librairie standard

#define SDCARD_CS 4 //PIN de l'arduino utilisé por la carte SD
#define BUFFER_SIZE 32   //Taille du buffer de ligne

/* Fonction d'initialisation de la communication avec la carte SD
   Port utilisé pour la carte SD : SDCARD_CS 4
*/
void initSD() {

  Serial.print("Initialisation carte SD...");
  //pinMode(SDCARD_CS, OUTPUT);
  if (!SD.begin(SDCARD_CS)) {
    Serial.println("Echec Initialisation");
    return;
  }
  Serial.println("Ok");
}

/* Fonction qui va lire la configuration située dans le fichier CONFIG.TXT et stockée dans la carte SD
   Paramètres : chaîne de caractères contenant le nom du fichier de configuration
                pointeur vers l'adresse IP du shutter
                pointeur vers l'adresse IP du maître
                pointeur vers le port du shutter
                pointeur vers le port du maître
*/
void setupConfig(String configName, IPAddress* shutter_osc_ip, IPAddress* master_osc_ip, unsigned int* shutter_osc_port, unsigned int* master_osc_port) {

  /*Variables locales*/
  char* ip0; //Variable temporaire de stockage du byte[0] des adresses IP
  char* ip1; //Variable temporaire de stockage du byte[1] des adresses IP
  char* ip2; //Variable temporaire de stockage du byte[2] des adresses IP
  char* ip3; //Variable temporaire de stockage du byte[3] des adresses IP


  //Déclare le buffer qui stockera une ligne du fichier, ainsi que les deux pointeurs key et value
  char buffer[BUFFER_SIZE], *key, *value;

  //Déclare l'itérateur et le compteur de lignes
  byte i, buffer_lenght, line_counter = 0;

  //Ouverture du fichier de configuration
  Serial.println("Début de la lecture du fichier de configuration..");
  File configFile = SD.open("config.txt");
  if (!configFile) { // Gère les erreurs
    Serial.print("Erreur d'ouverture du fichier ");
    Serial.println(configName);

    for (;;);
  }



  /* Tant que non fin de fichier */
  while (configFile.available() > 0 ) {

    /* Récupère une ligne entière dans le buffer */
    i = 0;
    while ((buffer[i++] = configFile.read()) != '\n') {
      /* Si la ligne dépasse la taille du buffer */
      if (i == BUFFER_SIZE) {

        /* On finit de lire la ligne mais sans stocker les données */
        while (configFile.read() != '\n');
        break; // Et on arrête la lecture de cette ligne
      }
    }
    /* On garde de côté le nombre de char stocké dans le buffer */
    buffer_lenght = i;
    /* Finalise la chaine de caractéres ASCIIZ en supprimant le \n au passage */
    buffer[--i] = '\0';

    /* Incrémente le compteur de lignes */
    ++line_counter;

    /* Ignore les lignes vides ou les lignes de commentaires */
    if (buffer[0] == '\0' || buffer[0] == '#') continue;

    /* Cherche l'emplacement de la clef en ignorant les espaces et les tabulations en début de ligne */
    i = 0;
    while (buffer[i] == ' ' || buffer[i] == '\t') {
      if (++i == buffer_lenght) break; // Ignore les lignes contenant uniquement des espaces et/ou des tabulations
    }
    if (i == buffer_lenght) continue; // Gère les lignes contenant uniquement des espaces et/ou des tabulations
    key = &buffer[i];

    /* Cherche l'emplacement du séparateur = en ignorant les espaces et les tabulations âpres la clef */
    while (buffer[i] != ':') {

      /* Ignore les espaces et les tabulations */
      if (buffer[i] == ' ' || buffer[i] == '\t') buffer[i] = '\0';

      if (++i == buffer_lenght) {
        break; // Ignore les lignes mal formé
      }
    }
    if (i == buffer_lenght) continue; // Gère les lignes mal formé

    /* Transforme le séparateur = en \0 et continue */
    buffer[i++] = '\0';

    /* Cherche l'emplacement de la valeur en ignorant les espaces et les tabulations âpres le séparateur */
    while (buffer[i] == ' ' || buffer[i] == '\t') {
      if (++i == buffer_lenght) {
        break; // Ignore les lignes mal formé
      }
    }
    if (i == buffer_lenght) continue; // Gère les lignes mal formées
    value = &buffer[i];

    /* Transforme les données texte en valeur utilisable */

    /*OSC*/
    if (strcmp(key, "shutter_osc_Ip") == 0) {
      char* t;
      ip0 = strtok_r(value, ".", &t); //Split la variable value en deux chaînes en utilisant comme séparateur le "." Renvoie la partie gauche et stock le reste dans le pointeur t
      ip1 = strtok_r(t, ".", &t);
      ip2 = strtok_r(t, ".", &t);
      ip3 = strtok_r(t, ".", &t);
      *shutter_osc_ip = IPAddress(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3));
    }
    else if (strcmp(key, "shutter_osc_Port") == 0) {
      *shutter_osc_port = atoi(value);

    }
    else if (strcmp(key, "master_osc_Ip") == 0) {
      char* t;
      ip0 = strtok_r(value, ".", &t);
      ip1 = strtok_r(t, ".", &t);
      ip2 = strtok_r(t, ".", &t);
      ip3 = strtok_r(t, ".", &t);
      *master_osc_ip = IPAddress(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3));

    }
    else if (strcmp(key, "master_osc_Port") == 0) {
      *master_osc_port = atoi(value);
    }
  }

  /* Ferme le fichier de configuration */
  configFile.close();
}

#endif
