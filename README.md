![](docs/logo.png)

# bergkvist/videoid
Let's say you own the exclusive license to a video clip (an *asset*) on YouTube.

Instead of manually looking through videos (*compilations*) to find out if it contains your *asset*, you wish this process could be automated.

If that is the case, you are in luck. videoid is written in C++, utilizing OpenCV for video processing, and OpenMP for multithreaded performance.



## Installation
```sh
$ git clone https://github.com/bergkvist/videoid
$ cd videoid
$ make

# Check that it is working:
$ ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24
```

## Usage
```
Usage: videoid <asset-id> <compilation-id>
  <asset-id>:       The id of a YouTube video that you own
  <compilation-id>: The id of a YouTube video that you want to check

NOTE: by setting the environment variable VERBOSE=1, you will get more detailed output when running the program.
```

## Documentation
### [Usage and examples](docs/1_EXAMPLES.md)
### [Theory (How it works)](docs/2_THEORY.md)
### [Benchmarks](docs/3_BENCHMARKS.md)
### [Analysis of Results](docs/4_RESULTS.md)
### [References](docs/5_REFERENCES.md)
### [Troubleshooting](docs/0_TROUBLESHOOTING.md)
