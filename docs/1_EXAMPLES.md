## Getting started

### Terminology
Some basic concepts/"big words" will be used as terminology in this project:
 - ***asset***: A video that you own. Your motivation for using this project is that you wish to find your *asset* within other videos (*compilations*)
 - ***compilation***: A video often containing several different clips, where one of these might be yours. In practice, this can be any video you want to check if your asset is inside of.

### Requirements
 - `opencv4` (or alternatively `opencv3`, but then you will need to change the `OPENCV`-variable in the `Makefile` from `opencv4` to `opencv`)

## Usage
```
Usage: videoid <asset-id> <compilation-id>
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