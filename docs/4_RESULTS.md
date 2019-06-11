![](logo.png)
[Back to frontpage](../README.md)

# Results and accuracy

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
* `B9ypGdx5EXA` - This is easy for the human eye to recognize as a match, but due to some movements/distortions, the algorithm fails. It is likely that the video was being played at a screen, and then filmed with a camera with slight movements.
* `8b1lO5NuaEs` - This has a big black borders, and a huge watermark. These elements combined is likely what makes the algorithm fail.