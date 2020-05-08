uniform sampler2D texture;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = vec4(vec3(color.r * 0.299 + color.g * 0.587 + color.b * 0.114), color.a);
}