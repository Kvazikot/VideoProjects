  + - - - + - + - -
  + - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
  + - + - + email: vsbaranov83@gmail.com  <br>
  + - + - + github: http://github.com/Kvazikot/VideoProjects  <br>
```
                            )            (
                           /(   (\___/)  )\
                          ( #)  \ ('')| ( #
                           ||___c\  > '__||
                           ||**** ),_/ **'|
                     .__   |'* ___| |___*'|
                      \_\  |' (    ~   ,)'|
                       ((  |' /(.  '  .)\ |
                        \\_|_/ <_ _____> \______________
                         /   '-, \   / ,-'      ______  \
                b'ger   /      (//   \\)     __/     /   \
                                            './_____/
```              
## Musk's Reward Function  
| Activity | Reward |
|---|---|
|Punishment for not inovating  |-80 |
|Punishment for trying inovating but failing | -5 |
| Reward for successful inovation | 100000 |

Baranov Vladimir vsbaranov83@gmail.com <br>
Document creation date: 02 September 2021


# It is desirable to implement:

* All software should be developed taking into account its use in realtime streams.
* All software should be reused in different contexts and different episodes of a futuristic science podcast.
* Occam's principle. The programmer works with the text or description of the desired content, and not with the timeline GUI of video editing software and its oddities
* Completely standalone software script for editing different video options is possible for different categories of people or contexts.
* High density of information packing typical for Discovery, PBS Nova and other reference scientific channels.
* Content is similar to what the futurist Issac Arthur does, but with the addition of an alternate history
* Inserts on 3D scenes from different sf films are welcomed. You can bypass the automatic copyright checker using the Rubik's cube effect or other geometric distortions. See [QuasiCode] for video translation into asci codes (https://github.com/Kvazikot/QuasiCode/blob/master/QuasiCode_EncoderDoc.MD)
* In the future, of course, you need to create your own 3D scenes, but because of their high cost, you can use YouTube content to promote the channel
* Change voice. My voice sounds closer to nerd than Herodotus.
* Some kind of talking 3D head.
* Formal language for constructing video content based on text, taking into account the capabilities of Codex systems (GPT-3)
* The Heisenberg Uncertainty Principle works in our favor when you want to change human history by changing just one byte of your video content description
* Provide the ability to schedule a convenient time for creative work, leaving the entire routine to the computer.

# List of 3d scenes
* VideoCube
* [Barth Sextic](https://github.com/Kvazikot/VideoProjects/blob/master/docs/Video_content_creation_Protocol_001_eng.MD#barth-sextic-as-a-scene)
* [Worm hole tunel](https://github.com/Kvazikot/Tunel/blob/master/docs/Tunel_Ideas.MD)


# General algorithm for creating video content for youtube
1. First, the text of the issue is being prepared.
2. It is recorded by voice using bandicam from full screen (f11 in chrome) text screen capture. <br/>
   When dictating text, you can highlight the place in the text where I read. <br/>
   This is necessary in order to be able to read the time code and synchronize the video sequence with the text. <br/>
3. Timecode markup. Upload subtitle file from youtube.
4. Audio with attached subtitles will be used with arbitrary pauses between phrases / paragraphs. Video grep technology.
5. An alternative option is to use the Google speech synthesizer. See section questions.
6. According to the text, the script finds the necessary video content in YouTube
7. Preparing a list of videos with such a sign

Room | URL | Name | Duration | Transition Effect |
| --- | --- | --- | --- | --- |


4. I mark in the text the places where which video goes
5. I add directly to the text a list of videos and effects [source1, source2, source3] <3d cube, fade_in>
6. Run python script using [MoviePy] (https://zulko.github.io/moviepy/install.html),
which ideally goes over the text or plate and compiles the finished video.
For example, right in the plate, you can use a keyword to indicate the transition between videos.
7. The sound is taken from the video prepared at stage 1.
8. Further in the script you need to provide standard labels with inscriptions.
The fonts are available from the web.
[Photoshop Automation via Python] (https://martechwithme.com/photoshop-scripting-with-python-on-windows/)
9. All this is seasoned with the imposition of fog, cellular automata

An example of text for with processing rules and content description is [here] (https://github.com/Kvazikot/VideoProjects/edit/master/Prob_Terminator_scenario/text_with_clip_insertion.MD)

# VideoCube program
It is implemented on Unity and is able to receive via Berkeley Waves i.e. Berkeley Sockets commands from the Python script master.
VdeoCube is a collection of programs. Historically, the first 3D effect using video frames transmitted over sockets from a Python program.
The idea of ​​a Rubik's cube where instead of colored sub-cubes, TVs with video frames are presented.
Bercley sockets eventially was replaced by shared memory

# Barth Sextic as a scene
This surface is a good way to build in Unity. 
I knew about this wonderful structure from [Tibees blog](https://www.youtube.com/channel/UC52kszkc08-acFOuogFl5jw)
"A sextic surface is one defined by a polynomial equation of degree 6. The Barth sextic, drawn above by Craig Kaplan, is the sextic surface with the maximum possible number of ordinary double points: that is, points where it looks like the origin of the cone in 3-dimensional space defined by"

After the VideoCube program be ready. Video Textures 
Description of algebraic geometry can be found on [American Mathematical Sosiety site.](https://blogs.ams.org/visualinsight/2016/04/15/barth-sextic/) 


# Background as worm hole tunel
Bla bla bla

# Communication protocol between a Unity application and a Python script
Package Type 1 Instruction
id (128 bits) = c3e87452-0f42-11ec-a198-38b1dbc8b668
size (int64) = 20 kb
string (max 1 MB) json_object

Package Type 2 Picture
id (128 bits) = 01701412-0f44-11ec-a466-38b1dbc8b668
size (int64) = 20 kb
string (max 100 MB) json_object image description
size (int64) = 20 kb
pixel_data = bytes array



# General diagram of the pipeline for preparing the final video
The pipeline looks something like this
1. Python master script reads this description, subtitle script and video with my voice.
2. Finds tags with sources described in the text.
3. Downloads YouTube videos from the desired position
4. Divides the text into paragraphs.
5. Calculates how long it takes to fill in video content for a given paragraph.
6. Creates 6 frames from 6 different video sources in numpy.hstack
7. Adds text to the frame rendered by the script text_realtime_vfx.py and photoshop_render.py
8. Joins frames from different sources writes everything to the paragraph1.mp4 file
9. Launches the application on Unity (eg VideoCube).
10. Sends it a frame image via sockets or this application itself reads
