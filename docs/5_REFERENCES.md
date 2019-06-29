[![](logo.png)](../README.md)

# References

### Content ID (by Google)
A similar system to VideoID already exists for YouTube, and is owned by Google. This is called Content ID. Why then, create another system to do the same thing?

There are several problems with Content ID. 
 - It is proprietary and closed source. 
 - Its use is very limited (it can only be used automatically, and no manual control is given).
 - Its use will have side effects/it is proactive. If a match is detected, a claim will automatically be placed on the video.
 
#### Why is it bad that it will automatically place claims?
If a video has less than 100 000 views, the uploader is allowed to edit the video, and then refute the claim. If the video has more than 100 000 views, this cannot be done.

This means the optimal strategy would be to only place claims on videos with more than 100 000 views (or wait until they have this many views), since then the claim can no longer be refuted. This is not a choice you are given when using ContentID.

### pHash (https://www.phash.org/)
The image hashing algorithm used in this project is inspired by the algorithm used in the "pHash" project for perceptual image hashing.

### youtube-dl (https://ytdl-org.github.io/youtube-dl/index.html)
youtube-dl is a command-line program to download videos from YouTube (although other sites are also supported). The binary of this CLI is put in the bin folder of the project. It is used to download a video when it does not already exist locally.

You can see [HERE](https://github.com/bergkvist/videoid/blob/17d453f21f20e6f7aa9a5ed244f6fd1c8671db3e/src/video.cpp#L18) where it is used in the code.

### Hamming weight/popcount
[THIS CODE](https://github.com/bergkvist/videoid/blob/17d453f21f20e6f7aa9a5ed244f6fd1c8671db3e/src/hashed_frame.cpp#L6-L11) for counting the number of set bits in a 32 bit integer was copied from [THIS ANSWER](https://stackoverflow.com/a/109025) on StackOverflow

