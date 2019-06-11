![](logo.png)
[Back to frontpage](../README.md)

# References

### YouTube ContentID
A similar system to VideoID already exists for, and is owned by YouTube. This is called ContentID. Why then, create another system to do the same thing?

There are several problems with ContentID. 
 - It is proprietary and closed source. 
 - Its use is very limited (it can only be used automatically, and no manual control is given).
 - Its use will have side effects/it is proactive. If a match is detected, a claim will automatically be placed on the video.
 
#### Why is it bad that it will automatically place claims?
If a video has less than 100 000 views, the uploader is allowed to edit the video, and then refute the claim. If the video has more than 100 000 views, this cannot be done.

This means the optimal strategy would be to only place claims on videos with more than 100 000 views (or wait until they have this many views), since then the claim can no longer be refuted. This is not a choice you are given when using ContentID.

### pHash (https://www.phash.org/)
The image hashing algorithm used in this project is inspired by the algorithm used in the "pHash" project for perceptual image hashing.