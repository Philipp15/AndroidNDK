
precision mediump float;

varying vec2 UV;

uniform sampler2D myTexture;

uniform sampler2D myAnotherTexture;


void main()
{
	gl_FragColor = texture2D(myTexture, UV).bgra;
}