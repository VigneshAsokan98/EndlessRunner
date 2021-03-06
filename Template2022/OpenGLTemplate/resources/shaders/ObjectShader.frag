#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec3 ReflectDirection;

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

layout(binding=10) uniform samplerCube CubeMapTex;

uniform bool DrawSkyBox;    // Are we drawing the sky box?
uniform float ReflectFactor;// Amount of reflection

layout( location = 0 ) out vec4 FragColor;


struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};


struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

uniform LightInfo Spotlight[2]; 
uniform MaterialInfo material1; 

in vec4 p;
in vec3 n;

uniform bool useLight;


// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 3, pp. 106 - 107, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	if (angle < cutoff) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
			specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
		return ambient + spotFactor * (diffuse + specular);
	} else 
		return ambient;
}

void main()
{
	vec4 cubeMapColor = texture(CubeMapTex, normalize(ReflectDirection));
	vec4 vTexColour = texture(sampler0, vTexCoord);	
	vec4 color = mix(vTexColour, cubeMapColor, ReflectFactor);

	if(color.a < 0.1f)
	discard;

	vec3 Colour = BlinnPhongSpotlightModel (Spotlight[0], p, normalize(n));
	if(useLight)
	{
		for( int idx = 0; idx < 2; idx++ )
			Colour *= BlinnPhongSpotlightModel (Spotlight[idx], p, normalize(n));
		
		//Colour = BlinnPhongSpotlightModel (Spotlight[0], p, normalize(n)) * BlinnPhongSpotlightModel (Spotlight[1], p, normalize(n));
		vOutputColour = 4 * color * vec4(Colour, 1.0);	
		return;
	}
	
	//FragColor = vec4(vColour, 1.0);
	vOutputColour = color * vec4(Colour, 1.0) * vec4(vColour, 1.0);

}
