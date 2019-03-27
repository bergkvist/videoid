---
title: "Proposal: An Efficient Video Fingerprinting System Using OpenCV and Perceptual Hashes"
geometry: margin=4cm
---

Assume you are the rightful owner of a video clip on YouTube. You might then have a right to ad revenue from other YouTube videos that are using your clip. Discovering videos that might have used your clip can be a manual and tedious process. But what if this process could be automated? What if your computer could recognize whether your clip has been used in an arbitrary video?

For the sake of scalability: Imagine that you own 100 clips. You have a list of 100 youtube videos you suspect might contain some of these clips. You want to find out which videos contain which clips.

Clips should still be detected in videos, even if the video changes color tones, adds watermarks, or changes the video quality.

# Method
### Procedure
1. Download a set of videos from YouTube.
2. Hash each frame of every video using a manually implemented perceptual hash. *(In contrast to a cryptographic hash, a perceptual hash changes only a small amount for a small change in input)*
3. Compare the frame hashes using hamming distance *(XOR the hashes together, and count the number of set bits in the output)*
4. Run the output signal (of the smallest hamming distances) through a low pass filter, to remove potential spikes/false positive matches.
5. Discover where the filtered output signal crosses a certain threshold, and list these as matches. (Some graphing output might also be desirable)

### Tools
* Language: C++
* For downloading videos from YouTube: `youtube-dl` (cli)
* For working with video files in C++: `OpenCV` (library)
* (Not yet decided) Some tool for graphing the output signal
