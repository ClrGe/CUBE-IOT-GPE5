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






GENERALITES

    La societe Le Petit a pour objectif de creer une station meteo en reduisant les couts par rapport au modele precedent en passsant d'un modele cloud a une utilisation sur reseau local.
    Le projet est composé d'un serveur local (Raspberry Pi) et d'un microcontroleur wifi avec sonde et écran pour les relevés de température. D'un point de vue logiciel, les données sont stockées dans une base MariaDB, une API python-flask permet d'orchestrer le projet et sert le front-end.
    
    
HARDWARE

Liste matériel :
 - 1 breadboard
 - Cables Dupont
 - Écran LCD i2c 1602
 - Raspberry Pi 3B
 - Arduino MKR wifi 1010
 - Capteur DHT11
 
 L'Arduino est programmé en C++ et flashé dans le logiciel Arduino IDE
