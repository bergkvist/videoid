# compare-videos
Video comparison using perceptual hashing in C++

### Dependencies
(Manjaro Linux)
 - `extra/opencv 4.0.1-5` (installed to `/usr/include/opencv4`)
 - `community/youtube-dl 2019.03.01-1` (only used for downloading videos)

### Downloading videos
```zsh
$ youtube-dl --id -f 'bestvideo[height<=144][ext=mp4]' <YOUTUBE_ID>
```
