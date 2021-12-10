TEMPLATE = subdirs

CONFIG += ordered

# All the projects in your application are sub-projects of your solution
SUBDIRS = app7CoreLib \
          app7Dyn \
          app7 \
          app7ML
#          muApiLib
#          Samples\HMProcessing
#          Samples\cube \
    #Samples/TableParser
#          Samples\Terrain_tool



# Use .depends to specify that a project depends on another.
#app7.depends = app7CoreLib app7Dyn hmprocessing
cube.depends = app7CoreLib
#Planets.depends = app6CoreLib

