# Recording and Image Processing Protocol


## Recording

### Before starting
- Remmember to always check voltage and replace batteries of low-battery/dead kilobots.

- Check camera battery

### Procedure
1. Order kilobots with beacons below the glass. Distribute the rest randomly over the circle.
2. Run the program, and set a timer to start recording right before the 30th cycle starts (beacon will turn green in cycle=29).
3. Record the rest of the cycles up until cycle=255 or the camera dies haha.
4.

## Image Extraction

1. Once we copied the MP4, check at which second the experiment starts ($\Delta t$ cycle = 30) and ends (depending on $\Delta t$), and cut the vídeo with ffmpeg as follows:

ffmpeg -ss 00:01:00 -i video.MP4 -to 00:02:00 -c copy video.mp4

The format for time is hh:mm:ss.

2. We will now extract one image per cycle. For this, we will start at the approximate middle of the first cycle, and extract every x frame, where x is the mean duration of a cycle for each $\Delta t$.

For example, for $\Delta t = 400$ this would be the command:

```sh
ffmpeg -ss 00:00:02 -i video.mp4 -vf "select=not(mod(n\,106))" -vsync vfr -q:v 2 output%04d.jpg
```
the bash code **extract_images.sh** will do this automatically:

```sh
bash extract_images.sh video_name dir_to_save_images deltat
```

## Color Counting
