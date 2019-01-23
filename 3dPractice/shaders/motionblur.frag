uniform sampler2D in_Texture;


varying vec4 ex_Color;
varying vec2 ex_TexCoord;


void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);
	tex = tex * vec4(0.7, 0.7, 0.7, 0.7);
	gl_FragColor = tex;
}