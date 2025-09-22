# 🌅 Projets ISMIN – Traitement de données & Surveillance utilisateur  

Ce dépôt regroupe deux projets réalisés en **C** sur **STM32** dans le cadre du cycle **ISMIN (Ingénieur Systèmes Microélectroniques et Informatique)** à l’**École des Mines de Saint-Étienne**.  

---

## 🚀 Projet Simulateur d'aube  

- ⏰ **Objectif** : conception d’un système embarqué favorisant un réveil naturel et la surveillance de la qualité du sommeil.  
- 📂 **Fonctionnalités principales** :  
  - Simulation de lever/coucher de soleil avec LEDs RGB (**PWM + DMA**)  
  - Détection de mouvement et position du dormeur via capteurs **ToF VL53L1X**  
  - Mesure de la température et de la qualité de l’air (**SPI + I2C avec capteur SGP40**)  
  - Affichage dynamique sur écran **OLED SSD1306**  
  - Interface **Bluetooth** pour la configuration de l’alarme depuis un smartphone  
  - Interfaces **tactiles capacitatives (TSC)** pour le contrôle local  
  - Communication **CAN** entre deux cartes embarquées  

---

## 🏗️ Architecture du code  
Le code est structuré en fichiers **.c / .h** pour chaque module :  

- `led.c / led.h` → gestion des LEDs RGB  
- `buzzer.c / buzzer.h` → génération de mélodies d’alarme  
- `tof.c / tof.h` → détection de mouvement et position du corps  
- `sensors.c / sensors.h` → capteurs de température et qualité de l’air  
- `oled.c / oled.h` → gestion de l’écran OLED SSD1306  
- `can.c / can.h` → communication inter-cartes  
- `touch.c / touch.h` → interfaces tactiles  
- `main.c` → boucle principale et logique système  

---

## ⚙️ Matériel utilisé  
- Microcontrôleur **STM32L476** (ARM Cortex-M4)  
- Carte électronique ISMIN (conçue pour projets pédagogiques)  incluant:
- Capteurs **ToF (VL53L1X)**  
- Capteur de qualité de l’air **SGP40**  
- Capteur de température via **SPI**  
- Écran **OLED SSD1306** (I2C)  
- LEDs RGB adressables  
- Module **Bluetooth**  
- Bus **CAN** pour communication inter-cartes  

---

## 📦 Compilation & Flashage  
1. Importer le projet dans **STM32CubeIDE**   
3. Compiler le projet  
4. Flasher la carte **STM32L476**  
5. Connecter les capteurs et les cables nécessaires et tester le fonctionnement  





## 👩‍💻 Auteur  
- **Halima Ghannoum** – Promotion EI23 – École des Mines de Saint-Étienne  
