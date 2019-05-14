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
Recommendation: Use Makefile to rapidly test out some commands:
```bash
# Run a basic comparison where you expect a match
make run
make run-verbose

# Run several comparisons in series
make test  
make test-verbose
```

### Example: input/ouput
```
$ ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24
asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s
```

### Example: input/verbose output
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

### Example: Several comparisons/accuracy of algorithm
```
$ python ./bin/run.py
asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s
asset: ZTHsrEG5jhA | compilation: ZTHsrEG5jhA => Match found: https://youtu.be/ZTHsrEG5jhA?t=4s
asset: ZTHsrEG5jhA | compilation: j4hnIAqyhGE => Match found: https://youtu.be/j4hnIAqyhGE?t=44s
asset: ZTHsrEG5jhA | compilation: 9RlBXx8Pf-8 => No matches found
asset: ZTHsrEG5jhA | compilation: B9ypGdx5EXA => No matches found
asset: ZTHsrEG5jhA | compilation: WLrbqsXwKz4 => Match found: https://youtu.be/WLrbqsXwKz4?t=163s
asset: ZTHsrEG5jhA | compilation: 8b1lO5NuaEs => No matches found
```
NOTE: For these videos, it correctly identifies 4 matches and 1 video for not having matches. However, it fails to recognize 2 matches (2 false negatives).
* `B9ypGdx5EXA` - This is easy for the human eye to recognize as a match, but due to some movements/distortions, the algorithm fails. It is likely that this was filmed from another screen with a camera.
* `8b1lO5NuaEs` - This has a big black borders, and a huge watermark. These elements combined is likely what makes the algorithm fail.

## Misc
The algorithm uses video only (no audio). It is likely to perform poorly on single-colored backgrounds with text - so try to avoid these in your assets.

### Visualize comparison
This requires that you have Python 3 installed with `matplotlib` and `pandas`.
1. Run the program on any two videos. (Example: `$ make run`) This should generate a csv-file in `./images/`
2. Plot the csv-data using `$ make plot`, and notice the image `./images/out32.png`

### Video Downloads
Any two videos from YouTube can be chosen for comparison, by simply giving their ids to this CLI. If the videos don't exist locally, they will be downloaded and places in `~/.videos/*.mp4`. The filename will be the youtube id, and the video will be kept here in case it is needed later. Note that the `load` timings will be significantly lower the first time.

The `youtube-dl`-binary has been downloaded into `./bin/youtube-dl`, and is used as a dependency of the program for downloading the videos.
