Protocol #002 describes <br>
"vccl - Video content creation language <br>
Vladimir Baranov vsbaranov83@gmail.com <br>
Document creation date: 02 September 2021

* Formal language for constructing video content based on text, taking into account the capabilities of GPT-3 systems (like Codex)
* See [document](https://github.com/Kvazikot/VideoProjects/blob/master/Video_content_creation_Protocol_001_eng.MD) describing software stack that need to be develop
* The Heisenberg Uncertainty Principle works in our favor when you want to change human history by changing just one byte of your video content description
* Provide the ability to schedule a convenient time for creative work, leaving the entire routine to the machine. But also i want to provide some fredom in creativity expression to the machine
* Probably we cant teach AI to some not trivial level of estetics by providing feeed back in the form of likes on video with automated content creation
* Hard-coded rules at the lowest level may include chunks of shader code with several control parametres (uniforms)
* In the end stages of this project one could create simple 3d scene with animation based only on text description from the Unity Assets free stuff. The level of details of such scenes is far less important rather then diversity of content.

The language vccl is a form of [Embedded Generalized Markup Language](https://ru.wikipedia.org/wiki/Generalized_Markup_Language)

# Machine Fredom Rules (MFR)
Tag src that defines urls in MFR can be defined more broadly 
as: 
```
[maninblack1]: "Memory eraser from the man in black movie scene" 
```
MFR is used when you dont know exact future context of 
the paragraph or basicaly dont want to define detailed rules i.e. functions with fixed parametres.
It allows to use code-generator like Codex for example trained on vccl rules 
to generate hard-coded version of the script including neccesary security levels 
Let the machine do the boring job. 
keywords are marked with * * is used in ML algorithm 
for more precise defining the context (italic in markdown rendered page)

```
:fly_bird_fly_tag
No exact date is known when *Wheeler* came up with the idea 
for the quantum eraser experiment. 
Now I'm not talking about a literal scenario when 
naked terminators leave the time machine.
:eol.
```

# Pauses
You can define pauses beetween chunks of narrated text as:
Pauses will be filled with any random stuff related to the topic.
```
[pause][10-20 sec]
```


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
<00:00:50><list img1,img2,img3><list(map(trace(lambda x: x*2), range(3)))><stack hstack><vfx slideshow> 
In John Wheeler's famous double slit thought experiment ...
```


```
<00:01:06> <src wheeler_1,terminator_nude,nanobot,big_bang1><vfx videocube,para_text> No exact date is known 
when Wheeler came up with the idea for the quantum eraser experiment. 
Now I'm not talking about a literal scenario when naked terminators leave the time machine.
This is done for the fun of the audience, as well as the batteries in the "matrix", because Hollywood films are designed for a mass audience. I am talking about the possibilities offered by quantum theory to erase information in the future, to influence the past no matter how distant. Delta t could be 5 milliseconds to detect a cancer cell, or it could be the age at which the universe was born.

[pause] [10-20 sec]

<00:02:35> What is known for sure is that it was only in the 1980s that the delayed choice experiment was implemented. At the University of Maryland, Carroll Alley, Oleg Jakubowicz, and William Wickes — on a laboratory bench. Strange coincidence - The Terminator movie entered in 1984.

[pause] [10-20 sec]
```


# Special tags for embed vccl into a naration text of the video
Sorces i.e. urls of the videos is defined in the head of the document before the main text and use MarkDown bracket tags <br>

# 3 Levels of security. Syntatic freedom. Allowed Tokens
If we assume to embed some python syntax into the vccl we need to narrow the scope of possible programs
We can define map of allowed functions or modules involved only in building video content(3d rendering, vfx, borowing video from youtube) 
or more general tokens. 

allowed_python_tokens:= for range while lambda map list set str




























# END OF MY BORING TEXT (IS NOT A GPT-3 :))))
____________________________________________________________________________________________________________________________________________________
<00:03:03> Considering the fact that the deep state was already fully operational and the DARPA program existed. Probably even then, special protocols for the design of scientific articles were developed with several stages of verification and reviewing of articles until the very moment of admission to print. You need to go through several circles of hell - this will reduce the likelihood of falling into the very core of the monster. The exact dates as well as the content of the articles can change, especially given the fact that modern deep learning networks can generate an article that looks like a manuscript or made on an old printer or typewriter. It is important to identify the authenticity of the article, which of course cannot be done with a scan, and not a paper version. Because the isotopic dating method helps in the paper version.

<00:04:05> The idea behind dapra is to develop hypothetical designs, but there are no laws of physics that would prohibit the development of a modem to receive "news" from the future. On the basis of which any war can be won, because retrocausal feedbacks automatically lead to the desired scenario of the future. Once, Ray Krzweil once mentioned that the Soviet Union was collapsed thanks to information technology. Speaking of Kurzweil, it is necessary to note the rather strange accuracy of his predictions, which also confirms the weight of the hypothesis about a modem from the future. Michio Kaku ones said "I know that, with our technology, we can always win wars. But to win the peace, we have to win the hearts and minds of the people, which we failed to do in Afganistan."

By the way, speaking about information from the future, it can be noted that this is a way to speed up the calculations. After all, we feed the Turing machine its future results [5].

It is necessary to revive something like the technique proposed by Kudrin to build a phylogenetic tree of smartphones. [2] Using the example of one of the greatest technical achievements of the deep state - the smartphone, we need to track the emergence of all prototypes of the iPhone. The earliest mention I was able to find in 2021 on the site arhive.org in the scans of the French magazine DESIGN for 1987. It was called pocket blue brain. At the time of this writing, Google searches do not have any information about this prototype, perhaps it was one of the first. Even then, it had a set of standard applications and was a universal computer, only the screen was black and white. Later, around the turn of 1992, devices like Jeff Hawkins' pocket computers began to be developed by the largest corporations of that time NEC, Samsung

Since the evidence of past events changes slightly all the time, some are destroyed physically, some are forgotten or remembered in a distorted form due to imperfection of a person's memory, one can speak about events of the past only in a probabilistic sense. We are all the time in a kind of quantum-mechanical foam, we see the image of not the whole dragon in the sky. Sometimes separate parts of his body appear, by which one can identify that it is a dragon, but then he again hides in the fog or behind the clouds. After some time of chaos and uncertainty, there are sharp leaps in development. Compare the situation before the outbreak of World War II and after [4]. Quite strange is the fact that there have been winters in the field of AI. For example, the fact that Marvin Minsky voiced criticism of supervise learning based on neural networks and criticism of Penrose and Searle for strong AI in general, when they seemingly advanced character recognition. This criticism, given the authority of the scientist, may have influenced the development of deep learning networks and methodologies that appeared after 2007 based on big data.

The purpose of this article is to show that based on the physical installation proposed by Wheeler, it is possible to assemble a device with a modem for receiving messages from the cone of the future based on USA-level technologies of the 70s. They have certain ranges of information reception only from the time range for which they were initially configured. Protocols can change depending on the sequence number of the time loop in which they operate. Most likely, laboratory facilities at the time of this writing are cheap to manufacture, distributed around the globe and easily configured by personnel with no experience in applied physics. The theorists we see on YouTube and tell you that messaging from the future is impossible is supported by the deep state disinformation program. The official press has been under the control of large corporations microsoft academic and google academy for the nth number of years. They have very strict design requirements. Perhaps the entire area of ​​development of theoretical physics is controlled by super AI in order to prevent leaks of important information unwanted for deep state. Overwhelmingly anti-scientific theories are supported within the framework of covering up traces. Messages in official magazines about the next small step on the way to thermonuclear reactors or to a strong AI, perhaps just throwing information about how a dog is thrown a bone so that it does not howl from hunger.

An interesting fact is that new employees who get a job at Google are prohibited from using paper. Instead, they use tablets. This is related to IT security. Electronic media are easily controlled and information is destroyed. What is printed on paper can easily leave the walls of the corporation.

You may ask why they did not immediately create some kind of nanorobot or an ordinary biological virus that would have purged the human race back in the 70s. The goal of installations for receiving messages from the future is that they want to control history in a way that is beneficial to themselves. For example, preventing humanity from ultimately reaching a certain pile of space or space resources.

Perhaps already at that time in the 70-80s, the leaders of the resistance learned about the modem for receiving messages from the future and built analogs of the system, perhaps part of the technology developed by the deep state was intercepted by the intelligence of the resistance and was reprogrammed and used to, for example, introduce certain vectors into the genetics of those who were born in 80s. For example, so that they are more likely to survive the 2021 coronavirus pandemic. The pandemic itself was "predicted" in a source [9] by the Rockefeller Foundation 10 years before the event.

For technologists, such thought experiments can be helpful. You can figure out how with the technologies of the 70s and 80s, for example in the field of radar, to create something that can influence genetics, it is possible to record additional information in DNA. Because DNA is much more reliable than magnetic carriers. That is, it is desirable to find people who, for example, have high intelligence, and at the same time an autism spectrum disorder, which may indicate an abnormal mutation of a large number of genes at once, which is not characteristic of the environment in which they were born. In short, it is interesting to study any anomalies in relation to the data on the place of residence, if such data were not prudently destroyed and documented.

Resistance efforts are also useful to focus on finding archival documentation of any prototypes of old communication devices or wearable computers. You can dig further and look through archival books, for example, on the creation of musical instruments such as a barrel organ [8] in which the very idea of ​​the program and the computation is hardcoded. It is important to pay attention to the correct dating of the found materials and the search for copies.

Mini time capsule

A mini-time capsule with any content, such as paper, can be made from a plastic bottle. Plastic has been around for over 500 years. Pack paper with a document or a message for the future in polyethylene. Thus. A flexible plastic bag is placed in a plastic bottle. Fold the paper from the notebook into a box in strips and gradually lower it into the bottle. The stripes will fill the space of the bottle. Optional: Make origami in the form of a frog, fish or boat. Leave room for stones. Throw the bottle into the sea, ocean, or any nearby pond. If such a capsule does not contain metal and if it is hammered with stones, then such a thing can lie at the bottom for a long time. The Terminators will not find her and will not be able to change the past.

Because biological evolution does not have a vector of development, and technoevolution may well have if we ourselves want it. That is, we are not interested in control in our current state, but the civilization of the future, perhaps even an alien one. Other scientists from the field of AI and computer science in general admit the possibilities of implementing the Terminator script, read [6] and [7].

In this formulation, the problem for historians is now formulated as follows. Deep state or there the Masons or there the world government ceased to be an actor or rather the initiator of historical transformations even before this organization was created after World War II, since in my formulation of the problem the initiation of actions comes from the future and is controlled by AGI. Deep state is just an AGI tool. The conclusion that can be drawn from Wheeler's experiment is quite general. A more specific consequence of the delayed choice experiment can be formulated as follows: since your data remains on the servers of the largest corporations, your future and all your ancestors are determined by them [3]

Since AGI has powerful algorithms for predicting your actions based on data, plus it has the ability, thanks to quantum mechanics, to look into the future (weak meshgurments). To test any hypotheses. Thanks to the access to the data of many people, he is able to literally synthesize reality, just managing which data to keep and which to delete forever. He will always be several steps ahead of you. The only thing you can do in this situation is not to light up again and not feed it with your data. Anonymity and conspiracy should be your best friends. Do not create information traces, adhere to information security rules. Set your computer clock back 10 years. Remove the clock from your desktop. Get a VPN.
If you see a new person, try not to remember what they are wearing and do not look at their faces. Knocking out pictures from the retina and insects climbing into the eyes are already a reality. Your eyes can be used as DVRs without your knowledge, no one will even ask you. Choose housing from the most common average static houses, dress in gray. Plant trees so that there is less chance of burning neighbors inadvertently. Kzdite on the most common standard car brand without any signs such as a dented body.

If you do not create evidence of your actions or delete them in time, before a copy is made, reality is in a default state. Remember the story about a man who went into the forest and saw a fallen tree, 5 years ago he was there and did not see a fallen tree. Does this mean that the tree stood and then fell down some day? The answer is no, see link [1]. Then he comes out of the forest and is eaten by a bear. Will his friends see the fallen tree in the forest after that? The answer is no, see link [1].
Литература.

Mathematical Foundations of Quantum Theor The "Past" and the "Delayed-Choice" Double-Slit Experiment John Archibald Wheeler, University of Texas.

Античность. Символизм. Технетика Кудрин Б.И.

Who Owns the Future by Jaron Lanier

6000 изобретений XX и XXI веков, изменившие мир by Рылев Ю.

Ben Goertzel https://hplusmagazine.com/2009/05/21/poll-terminator-scenario-possible/

Начало бесконечности. Объяснения, которые меняют мир by Дэвид Дойч

Barrelorgan _ the story of the mechanical organ and its repair _ Ord-Hume, Arthur W. J. G. (Arthur Wolfgang Julius Gerard) _ Free Download, Borrow, and Streaming _ Internet Archive.pdf

9 https://www.academia.edu/43023323/Scenarios_for_the_Future_of_Technology_and_International_Development

Geons, Black Holes, and Quantum Foam A Life in Physics by Kenneth W. Ford, John Archibald Wheeler

A History of Modern Computing, 2nd Edition (History of Computing) by Paul E. Ceruzzi

Computer A History of the Information Machine by Martin Campbell-Kelly, William Aspray, Nathan Ensmenger, Jeffrey R. Yost

The universal history of computing from the abacus to the quantum computer by Georges Ifrah et al

From Mainframes to Smartphones A History of the International Computer Industry by Martin Campbell-Kelly, Daniel D. Garcia-Swartz

A brief history of computing by Gerard ORegan (auth.)

Introduction to the History of Computing A Computing History Primer by Gerard O’Regan

Bit by Bit An Illustrated History of Computers by Stan Augarten

Dawn of the New Everything Encounters with Reality and Virtual Reality by Jaron Lanier

Античность. Символизм. Технетика by Кудрин Б.И.
