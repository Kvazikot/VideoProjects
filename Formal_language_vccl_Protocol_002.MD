Protocol #002 describes <br>
"vccl - Video content creation language <br>
Baranov Vladimir vsbaranov83@gmail.com <br>
Document creation date: 02 September 2021

* Formal language for constructing video content based on text, taking into account the capabilities of Codex systems (GPT-3)
* The Heisenberg Uncertainty Principle works in our favor when you want to change human history by changing just one byte of your video content description
* Provide the ability to schedule a convenient time for creative work, leaving the entire routine to the machine.

The language vccl is a form of [Embedded Generalized Markup Language](https://ru.wikipedia.org/wiki/Generalized_Markup_Language)

# Special tags for embed vccl into a naration text of the video
Sorces i.e. urls of the videos is defined in the head of the document before the main text and use MarkDown bracket tags <br>

# 3 Levels of security. Sybtatic freedom. Allowed Tokens

allowed_python_tokens:= for range while lambda map list set str

# Hard-coded Rules
```
:vsrc 
[label]: [http://video-hosting/video-id] 
:eol. 
```

Tag src defines urls or file paths. 
```
<00:00:50><src wheeler_1><vfx zoom, fade_out(time_sec=0.2),para_text(color=#00FF0022)> <br>
In John Wheeler's famous double slit thought experiment ...
```

```
<00:00:50><list=particles img1,img2,img3><list(map(trace(lambda x: x*2), range(3)))><stack hstack><vfx slideshow> 
In John Wheeler's famous double slit thought experiment ...
```


```
<00:01:06> <src wheeler_1,terminator_nude,nanobot,big_bang1> No exact date is known 
when Wheeler came up with the idea for the quantum eraser experiment. 
Now I'm not talking about a literal scenario when naked terminators leave the time machine.
```

# Machine Fredom Rules (MFR)
Tag src that defines urls in MFR can be defined more broadly 
as: 
```
[maninblack1]: "Memory eraser from the man in black movie scene" 
```
MFR is used when you dont know exact future context of 
the paragraph or basicaly dont want to define keywords.
It allows to use code-generator like Codex for example trained on vccl rules 
to generate hard-coded version of the script including neccesary security levels 
Let the machine do the boring job. 
keywords are marked with * * is used in ML algorithm 
for more precise defining the context (italic in markdown rendered page)

```
:free
No exact date is known when *Wheeler* came up with the idea 
for the quantum eraser experiment. 
Now I'm not talking about a literal scenario when 
naked terminators leave the time machine.
:eol.
```

# Pauses
You can define pauses beetween chunks of narrated text as:
```
[pause][10-20 sec]
```
