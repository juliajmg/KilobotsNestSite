# Protocolo experimental en Kilobots

## Antes de empezar

1. **Asegurarse que todos tengan ID**:
   Iniciar el script *id_debug.hex*. Los kilobots encenderán la luz si tienen un ID $\mod 10$, i.e. el bot con ID = 1 encenderá del mismo color que el ID 11, 21, 31 y 41. Si no enciende luz, es que no tiene ID.
2. **Patas de kilobots rectas**:
   Utilizar el aparato 3D printed.


3. **Comprobar que los kilobots estén calibrados**
   Iniciar el script *go_straight.hex*. Se considera que un bot está calibrado si pueden ir recto 15 cm, en una anchura de 7.5 cm.
   Calibrar a los kilobots que se necesite con base en los valores de la tabla:
    https://docs.google.com/spreadsheets/d/1lLEAYlaw0cSjxO7ze2P7xQTy3PDm-wXrka6_WbAYmzk/edit#gid=1138819062.
   Si estos valores no aseguran ir recto, modificarlos y documentarlo en la tabla.

## Pruebas

### 1. Cuántos mensajes reciben por segundo

script: messages.hex

#### Protocolo

1. Conectar cable debugging a kilobot, abrir "Serial .." en KiloGUI.
2. Aparecerá el número de mensajes recibidos y los kiloticks.
3. Número de vecinos de 1 a 10.



### 2. Bots Vistos
script: unique_bots_seen.c
1. Bot emite un mensaje con su ID
2. Bot recibe mensajes con ID.
3. Si es la primera vez que recibe ese ID, guarda ID en vector y sumas 1 a variable de bots vistos.
4. Si recibe un mensaje con el mismo ID, no hace nada.
5. Dependiendo de número de bots vistos, enciende led:
  - 0: no luz
  - 1-3 luz verde
  - 4-6 luz azul
  - 7-10 luz roja
  - 11 en adelante: luz morada


#### 2.1. Bots vistos en $\Delta t$
script: Unique_bots_seen_Deltat.c

1. Random walk
2. Bot emite un mensaje con su ID
3. Bot recibe mensajes con ID,
4. Si es la primera vez que recibe ese ID, guardas ID en vector y sumas 1 a variable de bots vistos.
5. Si recibe un mensaje con el mismo ID, no hace nada.
6. En $\Delta t$, dependiendo de número de bots vistos, enciende led de un color (luces/intervalos dependientes de $\Delta t$) y se detiene.
7. Se toma fotografía del estado final.


### 3. Desincronización temporal entre bots

### 4. Random walk tests 

script: random_motion.hex

Los vídeos pueden ser vistos aquí:

https://mega.nz/folder/3GgwDIAD#aVKxpuHiPXFEw4ke6cx-Jg
