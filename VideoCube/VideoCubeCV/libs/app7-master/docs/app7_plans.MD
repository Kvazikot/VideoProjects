  + - - - + - + - -
  + - + - + copyright by Vladimir Baranov (Kvazikot)  <br>
  + - + - + email: vsbaranov83@gmail.com  <br>
  + - + - + github: http://github.com/Kvazikot/app7  <br>
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
Document creation date: 03/03/2021

# Criticism of the app7 project

● Undefined purpose of the project.
○ a) development of a 3d engine without a scope,
○ b) a specific game,
○ c) virtual environment for autopilot.
○ d) separate demos
● Deadline for completion in case 1.a is not clear
● There is no clear vision of the genre of the game in case 2.b: traffic simulator, race, exit the maze
● If the goal is 1.a, then it is not clear how to improve it, add new features or refactor existing modules.
● Some modules are highly dependent on others, many interdependencies
● To implement the game, it is more rational to use a ready-made 3d engine. by embedding the already developed logic there.
● Why reinvent the wheel?
● Easier to find level-modelers, texturers who know how to work with popular engines (for example, Unity).


# What is positive about the project
1. Platform independence
2. Simple hierarchy of base classes
3. An easy way to text serialize



## Algorithmic task: teach an agent to bypass collisions and go a route from the starting point, through intermediate points and to the final


Calculating the distance from the sensor on the car to the nearest obstacle
It is possible to simulate the distance to the closest object from the 2D view by taking the suppression of the bounding shapes. The second option is to write a shader that will calculate the distance from the nearest object to a given point on the car. These points (sensor coordinates) are passed to the shader via Uniform variables. At the output, it has an instantaneous distance measurement with added noise. The second option is, of course, more computationally intensive, but in terms of project development, it is better.
Plugin loading system.
Which loads the dll embeds the dialog in the plugins panel. Inserting TerrainDlg into DialogBox as a plugin. Each plugin can have its own dialog.

Level editor plugin on QGraphScene (movement towards the scene graph)


The ability to draw a level, create a level from a 2D picture, but not a maze, but an arbitrary one. The editor is a simple 2D window you can use QGraphScene to create walls. Alternatively, you can export and import scenes from SVG. You can move objects in a 2D map and see a 3D display at the same time. To begin with, you can use extrusion from MTerrain to see how it looks in 3D, but then there will be no way to cut off the invisible walls of the level. Therefore, on subsequent iterations, it is better to make each wall in 3D as a separate geometric object.

Advantages: on hosts that do not have Opengl 4.0 support, you can create levels, and run the simulation on another machine

In general, the application should not eat up too many resources, more precisely, it should focus on several assembly options depending on the available resources.


Restore scripts in lua from old versions
An alternative is to inline a lua interpreter. It has already been tested in practice and is lighter than Python.
Embed python, but you need to decide how to translate * .lib files into * .a
The third option is to connect the old Python interpreter working through shared memory, but then the scripts will be executed in another process, which will not be accessed from the main process, so this option is not very suitable.
1. Linux now has an updated CObjRegistry code, functions for calling methods and setting attributes have been added. We need to transfer this code.
2. It is also necessary to bind object classes


# Plan A.0 AI players logic
1. Move the code for passing the maze from the mingw branch to the veh_branch branch
2. Make a separate assembly / branch veh_branch in which there will only be a car and a code for passing through the maze. In order not to be distracted by other modules
● Remove all unnecessary modules.
3. Set a destination somewhere inside the maze and instruct the autopilot to get to it.
4. Add enemies to the labyrinth, they will be controlled with the knowledge of the map, because otherwise there will be a heavy load on the processor. And the agent passing the maze can build a map as it passes.

Time estimate: 1-3 points from two weeks to 1.5 months
Plan A.1 Play
Idea: There is a labyrinth inside the castle, you need to exit the labyrinth. Enemies can attack the player in the maze.
Success rate score 60%
1. Restore the castle scene and add a maze there.
2. Add enemies to the labyrinth in the form of other cars, they will be controlled with the knowledge of the map, because otherwise there will be a heavy load on the processor. There should be recognition of people and cars, cars can be destroyed.
3.Copy constructor for Biped
Modify the Biped class to implement people so they can be copied.
The copy constructor does not work right now.

3. Make a class of a bird flying freely in a certain space. Make a copy constructor.

Estimated time: 6-12 months




# Plan A.3 OctTree
Success rate score 10%
5. Rendering the world. Develop an OctTree class for rendering large spaces and calculating object collisions


# Plan A.3 Refactoring app7 CObjRegistry refactoring
This class does serialization and at the same time represents a scene graph. It is possible to single out a department
