[![](logo.png)](../README.md)

# Usage and Examples
### Requirements
 - `opencv4` or `opencv3` (both are supported)
 - `openmp`

### Using make
```bash
# Compile the project, expecting a binary at bin/main.exe
make

# Run a basic comparison where you expect a match
make run
make run-verbose

# Run several comparisons in series
make test
make test-verbose

# Plot data from latest run to debug why a match was or wasn't found.
# NOTE: Requires Python (matplotlib + pandas + configparser)
make plot
```


### Example: input/ouput
```
$ ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24
asset: ZTHsrEG5jhA | compilation: M_KWGJw6R24 => Match found: https://youtu.be/M_KWGJw6R24?t=106s
```

### Example: verbose input/output


```
VERBOSE=1 ./bin/main.exe ZTHsrEG5jhA M_KWGJw6R24

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

### Visualization of input/output
![plot](out32.png)