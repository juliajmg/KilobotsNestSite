# Próxima reunión:
1. Probar con el material de allá.
1. Explicar cómo estoy midiendo los Delta t.
2. Ver el efecto de un kilobot que resetee el reloj para todos.
3. Ver el efecto que cuatro kilobots en cuadrantes de medio.
4. Cuántos bots ven en Delta_t, para varios Delta_t.
5. Arena pequeña: 20 cm de radio. Hacer vídeos con esta arena y condición PRW-angulos discretos.
6. Modelo de inmóviles con kilombo: mismos vecinos.


## Pruebas 25/11/2021

### Subir el vidrio para ver si aumenta el área de comunicación


#### CONDICIONES

- Oscuridad
- Dos alturas del vidrio
- 21 kilobots por altura.
- Infrarrojo hacia arriba


#### PROTOCOLO
- Poner beacon en el centro, debajo del cristal
- Medir la distancia máxima a la que llega la señal de sincronización
- Anotar la distancia en una tabla.

### Tiempo en loops en lugar de en kilo ticks

Elegir la altura de vidrio.
Hacer pruebas de sincronización a partir de loops para delta_t ~ 100, 1000, 5000 ticks

### Bots vistos en delta t ~ 100, 1000, 5000 ticks




## PRUEBAS 02/DICIEMBRE/2021


3. Modelo ya en marcha, pero que la luz marque la sincronización. Contando el tiempo de 256 ciclos. 200 iteraciones.
   3.1. Contabilizar cuántos se pierden uno o dos ciclos:
   - Contabilizar el periodo en segundos, del beacon para 200 iteraciones
Por alguna razón, de 0.24 pasa a 0. Ej, 1.23, 1.24, 2.00. Por esto hay que restar .76 a todos los cálculos.



## Reunión 21 diciembre de 2021
- Hacerlo a la carta o con un módulo fijo, tomar fotografías.
- Restringir N máximo a ver en bots vistos,y volver a sacar cuartiles con ese límite (por ejemplo, 20 es el máximo).



# ENERO 2022


1. Obtener fotogramas medios en el tiempo para delta_t = 800 y 1600 (hablar con Fabian para comprar servidores).
2. Comparar resultados de bots vistos en delta_t= 400, 800, 1600 para fotogramas a la carta vs módulo fijo
3. Grabar bots vistos con cuartiles normalizados a máx bots vistos para cada delta_t.


# 31 de marzo 2022

1. Difusividad: coeficiente de difusión en el desplazaiento cuadrático medio.
2. difusión = velocidad^2 / tau
3. Si no se mueven (régimen de velocidad 0), cómo queda el consenso.
4. Estimar el desplazamiento cuadrático medio en el kilombo.

5. Hacer un vídeo de los robots moviéndose en el espacio, con luz, y pasárselo a Carmen para que calcule el desplazamiento cuadrático medio.

6. Terminar de grabar delta t = 1600, con las condiciones que ya tengo.
7. Ver con David zonas en el histograma de densidad que tienen.

8. Bots vistos de uno y de otro,
