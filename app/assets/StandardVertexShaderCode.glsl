
attribute vec3 position;

attribute vec2 vertexUV;

varying vec2 UV;

uniform mat4 ModelViewProject;

void main()
{

gl_Position =  ModelViewProject * vec4(position, 1.0);
UV = vertexUV;

}
