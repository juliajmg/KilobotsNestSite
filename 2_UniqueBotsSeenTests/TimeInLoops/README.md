
### Bots vistos en $\Delta t$
script: bees_model=2_time=lop_botsSeen.c

1. Random walk
2. Bot emite un mensaje con su ID
3. Bot recibe mensajes con ID,
4. Si es la primera vez que recibe ese ID, guardas ID en vector y sumas 1 a variable de bots vistos.
5. Si recibe un mensaje con el mismo ID, no hace nada.
6. En $\Delta t$, dependiendo de número de bots vistos, enciende led de un color (luces/intervalos dependientes de $\Delta t$) y se detiene.
7. Se toma fotografía del estado final.
