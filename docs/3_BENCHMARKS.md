![](logo.png)
[Back to frontpage](../README.md)

# Benchmarks
NOTE: You might wonder what the `load`-action times are. This is the time spent checking if the video exists on disk, and downloading it if it doesn't. The reason it is practically 0 below, is that the videos already exist on disk.
### Sequential (without OpenMP)
```
$ VERBOSE=1 ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24

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

### Parallelized (with OpenMP)
```
./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24

OpenCV version   : 4.1.0
OpenMP version   : 201511
HASH_SIZE        : 32	(This means the images/frames will be resized to 32x32) 
WINDOW_SIZE      : 120	(This is used for computing rolling average and rolling std)
MIN_MATCH_LENGTH : 1	(in seconds. A match must last longer than this)

asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s

Elapsed time:
+-------------+---------+-----------------
| Resource    | Action  | Time
+-------------+---------+-----------------
| asset       | load    | 0.00272165 s
| asset       | hash    | 0.119081 s
| compilation | load    | 0.0011063 s
| compilation | hash    | 0.893859 s
| both        | compare | 0.0615238 s
+-------------+---------+-----------------
```

[Click here to see where the parallelization with OpenMP was added](https://github.com/bergkvist/videoid/blob/366589bdf0c099e8725ec96734a3f7bfa1b2ec02/src/video_comparison.cpp#L116-L130)

NOTE: Using OpenMP, with 4 cores (Intel Core i7-6600U) - we can see that the video comparison is indeed around 4 times faster. Why do we care about improving the comparison speed when it is already a lot faster than the hashing?

The answer is one word: **Scalability**:

| # of compilations   | # of assets | # of hashes | # of comparisons | comparisons per hash |
|---------------------|-------------|-------------|------------------|----------------------|
| 1                   | 1           | 2           | 1                | 0.5                  |
| 10                  | 1           | 11          | 10               | 0.91                 |
| 10                  | 10          | 20          | 100              | 5                    |
| 100                 | 100         | 200         | 10 000           | 50                   |
| 1 000               | 1 000       | 2 000       | 1 000 000        | 500                  |

Notice that as we get more assets, and more compilations, the number of comparisons is what increases the most quickly (assumning we only hash each video once).

#### But still, it would be useful if we could improve the speed of the hashing, right?
The problem here, is that around 80% of the time spent on hashing a single frame, is actually spent on reading it from disk. An attempt at parallelizing this using `#pragma omp parallel for ordered` had no noticable impact on the hashing time. The sequential version is already so optimized that there is little room for improvement, due to memory bandwidth limitations.
