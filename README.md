# 2020-ECG-Heart-Monitor
## PROJET ACADÉMIQUE
### Les maladies cardio-vasculaires sont la première cause de mortalité dans le monde :
###### Il meurt chaque année plus de personnes en raison de maladies cardio-vasculaires que de toute autre cause.
###### On estime à 17,7 millions le nombre de décès imputables aux maladies cardio-vasculaires, soit 31% de la mortalité mondiale totale. Parmi ces décès, on estime que 7,4 millions sont dus à une cardiopathie coronarienne et 6,7 millions à un AVC (chiffres 2015 l'OMS).
###### On sait que les moniteurs électrocardiographiques classiques sont grands et coûteux et qu'ils sont reliés au patient par des câbles, ce qui limite la mobilité du patient et rend les procédures médicales difficiles.
## L'objectif 
Ce projet porte sur la conception et le développement d'un petit prototype d'ECG sans fil qui peut être affiché sur certains appareils physiques ou numériques, ceux qui permettent la mobilité et l'isolement du patient.


1. Cette conception est appuyée sur un ESP32 cadencé à 240 MHz.
2. Le capteur SEN-KY039HS qui est basé sur une led à infrarouge et sur un phototransistor permettant la mesure des pulsations cardiaques (BPM).
3. Un capteur de température DS18B20 via le protocole et bus 1-wire.
4. Un serveur Blynk pour la transmission de données (TEMP et BMP).
5. L'affichage aussi sur un écran OLED via le protocole SPI.



## Schéma de connexions
![Preview](https://github.com/PaezEdward/2020-ECG-Heart-Monitor/blob/main/img/schema-electrique_fritzing.PNG)

## Test
![Preview](https://github.com/PaezEdward/2020-ECG-Heart-Monitor/blob/main/img/Test_fonctionnel_ECG.png)


Cordialement,
- Edward PAEZ
