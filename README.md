# OpenGL Learning

My repo for learning opengl.

## Referencess

OpenGl learning with The Cherno: 
https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

My first expermentation with ogl 1.1: 
https://twitter.com/OnikenX_/status/1398109002433695746

Using uniforms and changing values with 2 static triangles: 
https://twitter.com/OnikenX_/status/1446779099511508992

## Terms and meanings

### Shader

A shader is a program that runs in the GPU. An exemple of a shader can be seen in the 
file [basic.shader](./res/shaders/basic.shader). 

### Vertex Buffer

It's an simple buffer that holds data. This buffer is saved in the GPU memory.
This is then referenced when using draw commands to draw something in the screen.

### Uniforms

They are somewhat a global variable in a shader that can be changed programatically, 
for exemple the u_color found in the [basic_shader](./res/shaders/basic.shader).

it can be modified by using the function glGetUniformLocation to get the uniform 
and glUniform4f to set the uniform values.





