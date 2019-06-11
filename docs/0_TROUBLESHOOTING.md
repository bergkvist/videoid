![](logo.png)
[Back to frontpage](../README.md)

# Troubleshooting
If you are having trouble compiling, make sure you have installed all required dependencies.

### Check the version of OpenCV you are using
If you are using `opencv3`, you will need to change the OPENCV-variable in the Makefile from `opencv4` to `opencv`.

### Visualize comparison
This requires that you have Python 3 installed with `matplotlib`, `pandas` and `configparser`.
1. Run the program on any two videos. (Example: `$ make run`) This should generate a csv-file in `./images/`
2. Plot the csv-data using `$ make plot`, and notice the image `./images/out32.png`

### Video Downloads
Any two videos from YouTube can be chosen for comparison, by simply giving their ids to this CLI. If the videos don't exist locally, they will be downloaded and places in `~/.videos/*.mp4`. The filename will be the youtube id, and the video will be kept here in case it is needed later. Note that the `load` timings will be significantly lower the first time.

The `youtube-dl`-binary has been downloaded into `./bin/youtube-dl`, and is used as a dependency of the program for downloading the videos.
