
uniform float waveHeight;

uniform float waveTime;

attribute vec3 position;

attribute vec2 vertexUV;

varying vec2 UV;

uniform mat4 ModelViewProject;

void main()
{

	vec4 v = vec4(position, 1.0);

	v.y = cos(sin(v.x + waveTime * 3.14) * cos(v.z + waveTime * 3.14) * waveHeight);

	gl_Position = ModelViewProject * v ;
	UV = vertexUV;

}