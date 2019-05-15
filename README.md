# bergkvist/content-id
Video comparison using perceptual hashing in C++

## Getting started
### Requirements
 - `opencv4` (or alternatively `opencv3`, but then you will need to change the `OPENCV`-variable in the `Makefile` from `opencv4` to `opencv`)

### Terminology
Some basic concepts/"big words" will be used as terminology in this project:
 - ***asset***: A video that you own. Your motivation for using this project is that you wish to find your *asset* within other videos (*compilations*)
 - ***compilation***: A video often containing several different clips, where one of these might be yours. In practice, this can be any video you want to check if your asset is inside of.

## Usage
```
Usage: ./bin/main.exe <asset-id> <compilation-id>
  <asset-id>:       The id of a YouTube video that you own
  <compilation-id>: The id of a YouTube video that you want to check
 
NOTE: by setting the environment variable VERBOSE=true, you will get more detailed output when running the program.
```
### Example ouput
```
$ ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24
asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s
```

### Example verbose output
```
$ VERBOSE=true ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24

OpenCV version   : 4.1.0
HASH_SIZE        : 32   (This means the images/frames will be resized to 32x32) 
WINDOW_SIZE      : 120  (This is used for computing rolling average and rolling std)
MIN_MATCH_LENGTH : 1    (in seconds. A match must last longer than this)

asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s

Elapsed time:
+-------------+---------+-----------------
| Resource    | Action  | Time
+-------------+---------+-----------------
| asset       | load    | 0.00301644 s
| asset       | hash    | 0.133421 s
| compilation | load    | 0.00125059 s
| compilation | hash    | 0.892378 s
| both        | compare | 0.230802 s
+-------------+---------+-----------------
```

## Misc
The algorithm uses video only (no audio). It is likely to perform poorly on single-colored backgrounds with text - so try to avoid these in your assets.

### Visualize comparison
This requires that you have Python 3 installed with `matplotlib` and `pandas`.
1. Run the program on any two videos. (Example: `make run`)
2. Now a csv-file has been generated in `./images/`. Plot this by running `make plot`)
3. A png-file with the same name should now appear in the `./images/`-folder.

### Video Downloads
Any two videos from YouTube can be chosen for comparison, by simply giving their ids to this CLI. If the videos don't exist locally, they will be downloaded and places in `~/.videos/*.mp4`. The filename will be the youtube id, and the video will be kept here in case it is needed later. Note that the `load` timings will be significantly lower the first time.

A `youtube-dl`-binary has been downloaded into `./bin`, and is useful for
