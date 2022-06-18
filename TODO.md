
### 2. Optimizar la extracción de imágenes

- Cambiar el archivo extract_images.sh para incluir en línea de comando una vez el vídeo / nombre de carpeta donde guardar las imágenes.

- Checar si el comando de ffmpeg puede ir más rápido:

```bash
  ffmpeg -ss $ss -i $1 -vf "select=not(mod(n\, $mod))" -vsync vfr -q:v 2 $2/output%04d.jpg
```

-vf es necesario para seleccionar una imagen cada x fotograma.

-vsync Video sync method.   
**vfr** (variable frame rate):  Frames are passed through with their timestamp or dropped so as to prevent 2 frames from having the same timestamp.

Check this explanation:
https://superuser.com/questions/1274661/selecting-one-every-n-frames-from-a-video-using-ffmpeg


-q:v controls quality of the output image. Normal range for JPEG is 2-31 with 31 being the worst quality.
