# -*- coding: utf-8 -*-
"""

    Simple coloring shader example.

    Press any number key (not on the numpad) to change the triangle color

"""

try:
    from pyshaders import from_files_names, ShaderCompilationError 
    import pyglet
    import time
    import random
    import numpy as np
    import ctypes
    from pyglet import image
    from pyglet.window import key
    import pyglet.gl as gl
    from pyglet.gl import *
    import pyrr
    from pyrr import Matrix44 as matrix, Vector3 as vector, Quaternion as quaternion

except ImportError:
    print("Pyglet not found.")
    exit()
    
try:
    import pyshaders
except ImportError:
    import sys
    from os import getcwd
    from os.path import dirname
    sys.path.append(dirname(getcwd()))
    
    import pyshaders
    
    
f = open("base_frag.glsl.c")
frag = f.read()


f = open("base_vert.glsl.c")
vert = f.read()



def test_matrix1():    
    angles = pyrr.euler.create(roll=0.0, pitch=0.0, yaw=50.0, dtype=None)
    model = pyrr.matrix44.create_from_eulers(angles)
    #model = pyrr.matrix44.create_from_matrix33(model)
    #print(f"model=\n{model}")
    model = pyrr.Matrix44.identity()
    model[0,2] = 0.5 # translate x
    model[1,2] = 0 # translate z
    model[2,2] = 0
    model = model.transpose()
    #print(f"model=\n{model}")
    proj = pyrr.matrix44.create_perspective_projection(45, 1, 1, 1000) #create_orthogonal_projection(0,1,1,0,0,1000)
    view = pyrr.matrix44.create_look_at(np.array([0,15,0]), 
                                        np.array([0,0,0]), 
                                        np.array([1,0,0]))
     
    # set modelview matrix 2 variants
    #1. https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.transform.Rotation.html
    #2. https://pyrr.readthedocs.io/en/latest/api_matrix.html
    mvp =  model * proj *  view

    A = np.array([0,0,0,1])
    #print(f"A={A}")
    A = A @ mvp
    #print(f"A'' = {A}")
    
    B = np.array([1,0,1,1]) * 0.5
    #print(f"B={B}")
    B = B @ mvp
    B = (B[0],B[2],B[1])
    #print(f"B'' = {B}")    
    return mvp    
# Window creation
window = pyglet.window.Window(visible=True, width=1280, height=720, resizable=True)



#Shader creation
try:
    shader = pyshaders.from_string(vert, frag)
except ShaderCompilationError as e:
    print(e.logs) 
    window.close()

shader.use()
#Triangle creation
quads = pyglet.graphics.vertex_list(4,
    ('v2f', (-1, -1, 1, -1, 1, 1, -1, 1)),
)

pngfile = f"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\text.png"
text = image.load(pngfile)
width, height = text.width, text.height
print(f"{pngfile} width={width}  height={height}")

pngfile = f"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\background.png"
background = image.load(pngfile)
background_tex = background.get_texture()
width, height = background.width, background.height
print(f"{pngfile} width={width}  height={height}")

#Uniform colors
color_map = {key._0: (1.0, 1.0, 1.0), key._1: (1.0, 0.0, 0.0),
                key._2: (0.0, 1.0, 0.0), key._3: (0.0, 0.0, 1.0),
                key._4:(1.0, 1.0, 0.0), key._5:(1.0, 0.0, 1.0),
                key._6:(0.0, 1.0, 1.0), key._7:(0.5, 0.5, 0.5),
                key._8:(1.0, 0.7, 0.0), key._9:(0.1, 0.7, 0.1)}

def get_data(modelmtx):
    c_float_p = ctypes.POINTER(ctypes.c_float)
    data = modelmtx #numpy.array([[0.1, 0.1], [0.2, 0.2], [0.3, 0.3]])
    data = data.astype(np.float32)
    data_p = data.ctypes.data_as(c_float_p)
    return data_p

@window.event
def on_draw():
    window.clear()
    glEnable(GL_TEXTURE_2D)        # typically target is GL_TEXTURE_2D
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(background_tex.target, background_tex.id)
    # set texel size
    TexelSize = (1.0 / background.width, 1.0 / background.height)
    loc =glGetUniformLocation(shader.pid, "TexelSize".encode('utf-8'))
    glUniform2f(loc, TexelSize[0], TexelSize[1])
    
   
    # модельная матрица
    loc2 = glGetUniformLocation(shader.pid, "mvp".encode('utf-8'))
    mvp = test_matrix1()
    glUniformMatrix4fv(loc2, 1, GL_FALSE, get_data(mvp) )
    loc3 = glGetUniformLocation(shader.pid, "params".encode('utf-8'))
    params = test_matrix1()
    params[0,0] = random.randint(1,1000)
    glUniformMatrix4fv(loc3, 1, GL_FALSE, get_data(params) )
    #print(shader.uniforms)
    quads.draw(GL_QUADS)

@window.event
def on_resize(width, height):
    glViewport(0, 0, (GLsizei) (width), (GLsizei) (height) )
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW)
    
@window.event
def on_key_press(symbol, modifiers):
    if symbol==113:        
        window.close()
        pyglet.app.exit()
    if symbol in color_map.keys():
        shader.uniforms.color = color_map[symbol]

def update(dt):
    # ...
    on_draw()
    
test_matrix1()
pyglet.clock.schedule_interval(update, 0.1)
pyglet.app.run()

