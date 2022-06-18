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



## Medidas temporales y de mensajes

### Cuánto tiempo pasa en cada loop

Evolución de los kiloticks con las iteraciones del loop: la evolución es lineal con un pendiente de aprox 0.58


![](counter_vs_kiloticks_kilobots_10.png)


| kiloticks | iterations |
| --------- | ---------- |
| 100       | 172.41     |
| 500       | 862        |
| 1000      | 1724.1     |
| 5000      | 8620.6     |

### Iteraciones transcurridas entre mensajes

$ReceivedMessage =  0.3to0.5*iteration^{-1}$


#### INFORMACIÓN DE MENSAJES


Both the transmitter and receiver have an isotropic emission or reception pattern, which allow the robot to receive messages equally from all directions.

**message_t *tx_message()**
Callback for message transmission.

This callback is triggered every time a message is scheduled for transmission (roughly twice every second).

Kilobots can communicate at rates up to 30 kb/s with all robots using the same infrared channel for communication, there is the possibility that two or more robots may try to transmit at the same time. To mitigate this problem a standard carrier sense multiple access with collision avoidance (CSMA/CA) method is used.

In an experiment with 25 robots, configured as shown in Fig. 2, the channel could support on average 240 five-byte packets/second, a 32% channel usage.

Infrarred:  up to 7cm, up to 32kb/s  and 1kbyte/s with  25 robots, serial (256000 baud)

1024 bytes = 1 KB (kilobytes).


**Extract image every x frames**
g -i test.mp4 -vf "select=not(mod(n\,97))" -vsync vfr -q:v 2 images/RGB/out%3d.jpg


ffmpeg -i test.mp4 -vf "select=not(mod(n\,(96 + round(random(0,2)))))" -vsync vfr -q:v 2 images/RGB/out%3d.jpg


ffmpeg -i inVideo.mp4
  -vf "select='between(t,30,30.5)+between(t,45,45.5)+between(t,73,73.5)'"
  -s 320x240 -vsync 0 out%d.png

select='eq(n\,100)+eq(n\,184)+eq(n\,213)'

**Count number of frames in video**
  ffprobe -v error -select_streams v:0 -count_packets -show_entries stream=nb_read_packets -of csv=p=0 test.mp4


This actually counts packets instead of frames but it is much faster. Result should be the same. If you want to verify by counting frames change -count_packets to -count_frames and nb_read_packets to nb_read_frames.

*What the ffprobe options mean*

  *-v error This hides "info" output (version info, etc) which makes parsing easier (but makes it harder if you ask for help since it hides important info).*

    *-count_frames Count the number of packets per stream and report it in the corresponding stream section.*

    *-select_streams v:0 Select only the first video stream.*

    *-show_entries stream=nb_read_packets Show only the entry for nb_read_frames.*

    *-of csv=p=0 sets the output formatting. In this case it hides the descriptions and only shows the value. See FFprobe Writers for info on other formats including JSON.*



| Delta_t | Mean Period (s.f) | frames (25f/s) |
| ------- | ----------------- | -------------- |
| 400     | 3.22              | 97-100         |
| 200     | 1.19              | 46-50          |



- Establecer un tiempo de medición en cada periodo donde se cuenta sincronización. Por ejemplo, a la mitad del periodo.
- Obtener imágenes con fps correspondientes para el tiempo de medición elegido.
- Contabilizar colores.
-

# Procedimiento de extracción de fotogramas y m conteo de blobs
Fotogramas a medida, archivo: ImageExtract_byLeaderCycleDuration.sh
Fotogramas por módulo aleatorio en rango de lider, archivo: ImageExtract_byRandMod.sh

10 fotogramas por ciclo, y sacar la derivada



### Método marzo 2022 para extracción de imágenes

**Idea general:** Cortar los primeros 30 cyclos, y cortar el final hasta el ciclo 255.

1. Ver vídeo y anotar en qué minuto:segundo el líder apaga su luz después de encender verde, y en qué minuto:segundo enciende luz verde al final del vídeo.
2. Cortar el vídeo:

ffmpeg -ss 00:02:33 -i BeesModel_deltat=0400_p1=10_p2=10_q1=2_q2=3_lambda=20_rep=02.MP4 -to 00:18:15 -c copy cut.mp4


3. Extraer imágenes cada x frame (moda de duración de ciclos para cada deltat), ya sea para el vídeo previamente cortado:

ffmpeg -ss 00:00:02 -i  video.mp4 -vf "select=not(mod(n\,106))" -vsync vfr -q:v 2 output%04d.jpg

O cortarlo y extraer imágenes al mismo tiempo

ffmpeg -ss 00:02:33 -i BeesModel_deltat=0400_p1=10_p2=10_q1=2_q2=3_lambda=20_rep=02.MP4 -to 00:18:15 -vf "select=not(mod(n\,106))" -vsync vfr -q:v 2 BeesModel_deltat=0400_p1=10_p2=10_q1=2_q2=3_lambda=20_rep=02_Images/out%04d.jpg



#### FFMPEG DOCUMENTATION
Cutting small sections

To extract only a small segment in the middle of a movie, it can be used in combination with -t which specifies the duration, like -ss 60 -t 10 to capture from second 60 to 70. Or you can use the -to option to specify an out point, like -ss 60 -to 70 to capture from second 60 to 70. -t and -to are mutually exclusive. If you use both, -t will be used.

Note that if you specify -ss before -i only, the timestamps will be reset to zero, so -t and -to will have the same effect. If you want to keep the original timestamps, add the -copyts option.

The first command will cut from 00:01:00 to 00:03:00 (in the original), using the faster seek.
The second command will cut from 00:01:00 to 00:02:00, as intended, using the slower seek.
The third command will cut from 00:01:00 to 00:02:00, as intended, using the faster seek.

ffmpeg -ss 00:01:00 -i video.mp4 -to 00:02:00 -c copy cut.mp4
ffmpeg -i video.mp4 -ss 00:01:00 -to 00:02:00 -c copy cut.mp4
ffmpeg -ss 00:01:00 -i video.mp4 -to 00:02:00 -c copy -copyts cut.mp4
