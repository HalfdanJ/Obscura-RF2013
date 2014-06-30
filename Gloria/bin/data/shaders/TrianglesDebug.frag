uniform sampler2D tex;

varying vec3 normal;

void main()
{
	gl_FragColor = vec4(normal, 1.0);// vec4(1.0) * length(vec3(0.0, 0.0, 1.0) * normal); // vec4(normal.x, 0., 0.0, 1.0);//gl_Color; //* texture2D(tex, gl_TexCoord[0].xy);
}
