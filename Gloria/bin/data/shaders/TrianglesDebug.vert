varying vec3 normal;

void main()
{
    normal = gl_Normal;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
