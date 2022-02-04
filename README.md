**CESI - Cube IoT - Groupe5**

**Vue d'ensemble et documentation**
---

Dernier edit : 2022-02-01T13:59:10Z

---

### **SOMMAIRE**

- GÉNÉRALITÉS

- HARDWARE

- SERVEUR

- APIs
  
- UI

- BASE DE DONNÉES

- BUDGET

## GENERALITES

La societe Le Petit a pour objectif de creer une station meteo en reduisant les couts par rapport au modele precedent en passsant d'un modele cloud a une utilisation sur reseau local.
Le projet est composé d'un serveur local (Raspberry Pi) et d'un microcontroleur wifi avec sonde et écran pour les relevés de température. D'un point de vue logiciel, les données sont stockées dans une base MariaDB, une API python-flask permet d'orchestrer le projet et sert le front-end.
    
    
## HARDWARE

Liste matériel :
 - 1 breadboard
 - Cables Dupont
 - Écran LCD i2c 1602
 - Raspberry Pi 3B
 - Arduino MKR wifi 1010
 - Capteur DHT11
 
 L'Arduino est programmé en C++ et flashé dans le logiciel Arduino IDE

Librairies utilisées :

WIFINA pour la connection au réseau
ADAFRUIT_SENSOR et DHT pour le capteur DHT11
LiquidCrystal pour le LCD

## SERVEUR 

Raspberry Pi 3B avec Raspberry Pi OS LITE qui héberge l'application FLask ainsi que la base de données MariaDB.
NGINX permet l'authentification des utilisateurs accédant au site grace au module httpd-tools, les mots de passe sont hashés puis sotckés dans un fichier .httpwd

## APIs

L'API en python-flask permet d'effectuer des opérations CRUD sur la base de données MariaDB sur les tables 'data' et 'sensors' de manière à pouvoir manipuler les données, ajouter ou supprimer des capteurs etc.
Le front-end est servi grâce au module 'render_template'. On trouve l'HTML dans le dossier /template et le CSS dans le dossier  Static.



## UI

Front-end de l'application :

Le projet comprend deux pages pour l'application web :
  
   - Une page login pour s'authentifier et passer sur la page d'accueil;
   - Une page d'accueil pour consulter les données des différentes sondes, ainsi que leurs géolocalisation
   - Des pages d'erreur 401 / 403 / 404 / 501

Deux zones sont sur la page d'accueil : un graphique à gauche et une carte interactive à droite.
Les boutons au centre permettent de partager, d'accéder au données d'OpenWeatherMap pour la région et d'afficher le tableau des données.
L'API d'OpenStreetMap permet de visualiser la position des capteurs sur une carte grâce aux données de position contenues en base de données.
L'API d'OpenWeatherMap  permet de voir les données météo de la région pour avoir un éléments de comparaison.


## BASE DE DONNÉES

Une base  de données cubeIoT sur MariaDB recueille les données de température et d'humidité collectées par le DHT11 dans une table 'data' et des informations sur le capteur lui-même (identifiants, longitude, latitude ...) dans une table dédiée.

## BUDGET

Le cout de la version précédente (CLOUD) du projet était de 169 euros. Cette version réduit les coûts de la manière suivante :

- Raspberry Pi 3 - 30€
- Arduino MKR wifi - 25€
- Capteur DHT11 - < 2€
- Matériel prototypage (cables, breadboard, alim...) - 10€

Les frais matériel seuls pour un prototype sont donc d'environ 67€
