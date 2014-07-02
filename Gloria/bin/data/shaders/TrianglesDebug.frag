uniform sampler2DRect tex0;

uniform float lightAmount;
uniform float textureAmount;

varying vec4 ambientGlobal, eyeSpaceVertexPos;
varying vec4 vertexPos;
varying vec3 vertexNormal;


float PI = 3.14159265359;
int lightsNumber = 1;

vec4 directional_light(in int lightIndex, in vec3 normal) {
    vec3 lightDir;
    vec4 dirLightColor, diffuse, specular, ambient = vec4(0.0);;
    float intensity;
    
    lightDir = normalize(gl_LightSource[lightIndex].position.xyz); // that's already in eye space
    ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
    /* The ambient term of a directional light will always be present */
    dirLightColor = ambient;
    /* compute light intensity
     * (the dot product between normal and light dir)
     */
    intensity = max(dot(normal, lightDir), 0.0);
    if (intensity > 0.0) {
        vec3 reflection;
        vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
        vec3 eyeVector = normalize(-eyeSpaceVertexPos_n); // in eye space, eye is at (0,0,0)
        
        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
        dirLightColor += diffuse * intensity;
        // compute Phong specular component
        reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
        specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
        gl_FrontMaterial.specular * gl_LightSource[lightIndex].specular;
        dirLightColor += specular;
    }
    return dirLightColor;
}

vec4 point_light(in int lightIndex, in vec3 normal) {
    vec3 lightDir;
    vec4 pointLightColor, diffuse, specular, ambient = vec4(0.0);
    float intensity, dist;
    
    pointLightColor = vec4(0.0);
    // Compute the light direction
    lightDir = vec3(gl_LightSource[lightIndex].position - eyeSpaceVertexPos);
    /* compute the distance to the light source */
    dist = length(lightDir);
    intensity = max(dot(normal, normalize(lightDir)), 0.0);
    if (intensity > 0.0) {
        vec3 reflection;
        vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
        vec3 eyeVector = normalize(-eyeSpaceVertexPos_n);
        float att, dist;
        
        dist = length(lightDir);
        att = 1.0 / (gl_LightSource[lightIndex].constantAttenuation +
                     gl_LightSource[lightIndex].linearAttenuation * dist +
                     gl_LightSource[lightIndex].quadraticAttenuation * dist * dist);
        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
        ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
        pointLightColor += att * (diffuse * intensity + ambient);
        // compute Phong specular component
        reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
        specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
        gl_FrontMaterial.specular *
        gl_LightSource[lightIndex].specular;
        pointLightColor += att * specular;
    }
    return pointLightColor;
}

vec4 spot_light(in int lightIndex, in vec3 normal) {
    vec3 lightDir;
    vec4 spotLightColor, diffuse, specular, ambient = vec4(0.0);
    float intensity, dist;
    
    spotLightColor = vec4(0.0);
    // Compute the light direction
    lightDir = vec3(gl_LightSource[lightIndex].position - eyeSpaceVertexPos);
    /* compute the distance to the light source */
    dist = length(lightDir);
    intensity = max(dot(normal, normalize(lightDir)), 0.0);
    if (intensity > 0.0) {
        float spotEffect;
        
        spotEffect = dot(normalize(gl_LightSource[lightIndex].spotDirection), normalize(-lightDir));
        if (spotEffect > gl_LightSource[lightIndex].spotCosCutoff) {
            vec3 reflection;
            vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
            vec3 eyeVector = normalize(-eyeSpaceVertexPos_n);
            float att, dist;
            
            dist = length(lightDir);
            att = spotEffect / (gl_LightSource[lightIndex].constantAttenuation +
                                gl_LightSource[lightIndex].linearAttenuation * dist +
                                gl_LightSource[lightIndex].quadraticAttenuation * dist * dist);
            diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
            ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
            spotLightColor += att * (diffuse * intensity + ambient);
            // compute Phong specular component
            reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
            specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
            gl_FrontMaterial.specular * gl_LightSource[lightIndex].specular;
            spotLightColor += att * specular;
        }
    }
    return spotLightColor;
}

vec4 calc_lighting_color(in vec3 normal) {
    vec4 lightingColor = vec4(0.0);
    
    for (int i = 0; i < lightsNumber; i++) {
        if (gl_LightSource[i].position.w == 0.0) {
            lightingColor += directional_light(i, normal);
        }
        else {
            if (gl_LightSource[i].spotCutoff <= 90.0) {
                lightingColor +=spot_light(i, normal);
            }
            else {
                lightingColor += point_light(i, normal);
            }
        }
    }
    return lightingColor;
}



void main()
{

    vec3 color = texture2DRect(tex0, gl_TexCoord[0].xy).rgb;
    color =color*textureAmount + (1.-textureAmount) * vec3(1.);
 //   vec3 bump = texture2DRect(depthTex, gl_TexCoord[0].xy).rgb;
    
    vec3 n = (vertexNormal);
    n = normalize(n);
    
    color *= 2.*calc_lighting_color(n).rgb * lightAmount + (1. - lightAmount) ;

    gl_FragColor.rgb = color;
	gl_FragColor.a = 1.0;
    
	//gl_FragColor = gl_Color;//vec4(normal, 1.0);// vec4(1.0) * length(vec3(0.0, 0.0, 1.0) * normal); // vec4(normal.x, 0., 0.0, 1.0);//gl_Color; //* texture2D(tex, gl_TexCoord[0].xy);
}
