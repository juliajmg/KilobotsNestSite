# Recording and Image Processing Protocol

## Recording

## Before starting
- Remmember to always check voltage and replace batteries of low-battery/dead kilobots.

- Check camera battery and settings


- Once a week run the code "IDdebug" to confirm all kilobots have an ID assigned.


1. **Asegurarse que todos tengan ID**:
   Iniciar el script *id_debug.hex*. Los kilobots encenderán la luz si tienen un ID $\mod 10$, i.e. el bot con ID = 1 encenderá del mismo color que el ID 11, 21, 31 y 41. Si no enciende luz, es que no tiene ID.
2. **Patas de kilobots rectas**:
   Utilizar el aparato 3D printed.


3. **Comprobar que los kilobots estén calibrados**
   Iniciar el script *go_straight.hex*. Se considera que un bot está calibrado si pueden ir recto 15 cm, en una anchura de 7.5 cm.
   Calibrar a los kilobots que se necesite con base en los valores de la tabla:
https://docs.google.com/spreadsheets/d/1OJTKlNPMPNzJElsxZkRB5j2uYAmKU1vKFCtaC1egaqg/edit?usp=sharing 
   Si estos valores no aseguran ir recto, modificarlos y documentarlo en la tabla.



## Procedure
1. Order kilobots with beacons below the glass and distribute the rest randomly over the circle.
2. Run the program, and set a timer to start recording right before the 30th cycle starts (beacon will turn green in cycle=29).
3. Record the rest of the cycles up until cycle=210 or the camera dies haha.
4. For $\Delta t > 800$ two or more videos will be recorded to include all cycles.


## Image Extraction

1. Once we copied the MP4, check at which second the experiment starts ($\Delta t$ cycle = 30) and ends (depending on $\Delta t$), and cut the video with *ffmpeg* as follows:
```bash
ffmpeg -ss 00:01:00 -i video.MP4 -to 00:02:00 -c copy video.mp4
```   
The format for time is hh:mm:ss.

1.1. If we are working with $\Delta t > 800$ first we need to cut and concatenate the videos into one:

**Concat demuxer**

This demuxer reads a list of files and other directives from a text file and demuxes them one after the other, as if all their packets had been muxed together. All files must have the same streams (same codecs, same time base, etc.) but can be wrapped in different container formats.
Instructions

Create a file mylist.txt with all the files you want to have concatenated in the following form (lines starting with a # are ignored):

```txt
# this is a comment
file '/path/to/file1.mp4'
file '/path/to/file2.mp4'
file '/path/to/file3.mp4'
```

Note that these can be either relative or absolute paths. Then you can stream copy or re-encode your files:

```bash
ffmpeg -f concat -safe 0 -i mylist.txt -c copy output.wav
```
The -safe 0 above is not required if the paths are relative.
Automatically generating the input file

It is possible to generate this list file with a bash for loop. Either of the following would generate a list file containing every *.wav in the working directory:
```bash
# with a bash for loop
for f in *.mp4; do echo "file '$f'" >> mylist.txt; done

ffmpeg -f concat -safe 0 -i mylist.txt -c copy -an output.wav
```
-an removes sound of video

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

Go to dir/kiloColors/RGBKiloCounter

enter python command:
```bash
python kilocounter.py -i dir_input_images -o output_file n_kilobots

```
