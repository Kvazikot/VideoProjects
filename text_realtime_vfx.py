# -*- coding: utf-8 -*-
"""

    Simple coloring shader example.

    Press any number key (not on the numpad) to change the triangle color

"""

try:
    from pyshaders import from_files_names, ShaderCompilationError 
    import pyglet
    import time
    from pyglet import image
    from pyglet.gl import GL_QUADS
    from pyglet.window import key

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
varying vec2 v_texcoord;
uniform vec3 color = vec3(1.0, 1.0, 1.0);

void main()
{
  float x = v_texcoord.x;
  vec2 c = vec2(0.0, 0.0);
  float d = distance(c, v_texcoord);
  color_frag = vec4(d, color.g, color.b, 1.0);
}
"""

vert = """
#version 400 core

layout(location = 0)in vec2 vert;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
void main()
{
  gl_Position = vec4(vert, 1, 1);
  // Pass texture coordinate to fragment shader
  // Value will be automatically interpolated to fragments inside polygon faces
  v_texcoord = vert.xy; 
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
    ('v2f', (-0.95, -0.95, 0.95, -0.95, 0.95, 0.95, -0.95, 0.95)),
)

#Uniform colors
color_map = {key._0: (1.0, 1.0, 1.0), key._1: (1.0, 0.0, 0.0),
                key._2: (0.0, 1.0, 0.0), key._3: (0.0, 0.0, 1.0),
                key._4:(1.0, 1.0, 0.0), key._5:(1.0, 0.0, 1.0),
                key._6:(0.0, 1.0, 1.0), key._7:(0.5, 0.5, 0.5),
                key._8:(1.0, 0.7, 0.0), key._9:(0.1, 0.7, 0.1)}

@window.event
def on_draw():
    window.clear()
    quads.draw(GL_QUADS)
    
@window.event
def on_key_press(symbol, modifiers):
    if symbol==113:        
        window.close()
        pyglet.app.exit()
    if symbol in color_map.keys():
        shader.uniforms.color = color_map[symbol]

pyglet.app.run()

