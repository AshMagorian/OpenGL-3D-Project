uniform vec3 in_Emissive;
uniform vec3 in_Ambient;
uniform sampler2D in_Texture;
uniform vec3 in_LightPos;

varying vec4 ex_Color;
varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;


void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);

	vec3 norm = normalize(ex_Normal);
	vec3 lightDir = normalize(in_LightPos - ex_FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1,1,1);

	vec3 light = in_Emissive + in_Ambient + diffuse;
	gl_FragColor = tex * vec4(light, 1);
}