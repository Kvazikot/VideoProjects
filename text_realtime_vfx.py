# -*- coding: utf-8 -*-
"""

    Simple coloring shader example.

    Press any number key (not on the numpad) to change the triangle color

"""

try:
    from pyshaders import from_files_names, ShaderCompilationError 
    import pyglet
    import time
    import numpy as np
    import ctypes
    from pyglet import image
    from pyglet.window import key
    import pyglet.gl as gl
    from pyglet.gl import *
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
    
    
frag = """
#version 400 core

out vec4 color_frag;

uniform sampler2D Background;
varying vec2 v_texcoord;
varying vec2 uv;
uniform vec3 color = vec3(1.0, 1.0, 1.0);

void main()
{
  float y = v_texcoord.y;
  float x = clamp(v_texcoord.x, 0.2, 0.3);
  vec2 c = vec2(1, 1);
  float d = 1.0 - distance(c, vec2(x, y));
  float thick = 0.1;
  if( d > 0.3 && d < 0.3 + thick)
     color_frag = vec4(0, 0, 0, 1.0);
  else
     color_frag = vec4(1, 1, 1, 1.0);
  color_frag = texture2D(Background, uv);
  //d = clamp(d, 0.0, 0.2);
  //color_frag = vec4(d, d, d, 1.0);
}
"""

vert = """
#version 400 core
uniform vec2 TexelSize = vec2(1.0, 1.0);
uniform mat4 uMVMatrix = mat4( 1, 5, 9, 2,
                               2, 6, 10, 2,
                               2, 6, 10, 2,
                               3, 7, 11, 2);
layout(location = 0)in vec2 vert;
varying vec2 v_texcoord;
varying vec2 uv;

void main()
{
  gl_Position = vec4(vec3(vert, 0) , 1) * uMVMatrix;
  // Pass texture coordinate to fragment shader
  // Value will be automatically interpolated to fragments inside polygon faces
  v_texcoord = vert.xy + vec2(1, 1); 
  float aspect = TexelSize.y / TexelSize.x;
  uv = vert.xy;
 //vec3(gl_MultiTexCoord0 - 0.5) * 1.0;

}
"""
    
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
    ('v2f', (0, 0, 1, 0, 1, 1, 0, 1)),
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
    
    # set modelview matrix
    modelmtx = matrix.from_scale([2, 2, 2]) * matrix.from_translation([0.5, 0.5, 0])
    loc2 = glGetUniformLocation(shader.pid, "uMVMatrix".encode('utf-8'))
    #app = lambda y: [np.float(i) for i in modelmtx]
    app = np.array([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1])
    print(len(app))
    # модельная матрица
    glUniformMatrix4fv(loc2, 1, GL_FALSE, get_data(modelmtx) )
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

pyglet.app.run()

