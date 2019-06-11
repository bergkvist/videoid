![](logo.png)
[Back to frontpage](../README.md)

# Benchmarks

### Example: input/verbose output
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

With OpenMP
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
NOTE: Using OpenMP, with 4 cores - we can see that the video comparison is indeed around 4 times faster. Why do we care about improving the comparison speed when it is already faster than the hashing?

The answer is one word: **Scalability**:

| # of compilations   | # of assets | # of hashes | # of comparisons |
|---------------------|-------------|-------------|------------------|
| 1                   | 1           | 2           | 1                |
| 10                  | 1           | 11          | 10               |
| 10                  | 10          | 20          | 100              |
| 100                 | 100         | 200         | 10 000           |
| 1000                | 1000        | 2000        | 1 000 000          |

Notice that as we get more assets, and more compilations, the number of comparisons is what increases the most quickly (assumning we only hash each video once).