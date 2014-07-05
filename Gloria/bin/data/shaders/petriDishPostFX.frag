#version 120

// this is how we receive the texture
uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

void main()
{
	vec4 col = texture2DRect(tex0, texCoordVarying);
    gl_FragColor = col;

    float lightness = 1.2;

    float luminance = (col.r + col.g + col.b) / 3 * lightness;

    vec3 bw = vec3(luminance);

    // Apply contrast.
    float Contrast = 9;
  	bw.rgb = ((bw.rgb - 0.5f) * max(Contrast, 0)) + 0.5f;

    vec3 darkColor = vec3(0,0,0);
    vec3 lightColor = vec3(0.94, 1, 0.9);
    vec3 toned = mix(darkColor, lightColor, bw);

    gl_FragColor.rgb = toned;
    gl_FragColor.a = col.a;
}