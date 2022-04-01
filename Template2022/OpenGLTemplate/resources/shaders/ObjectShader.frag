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


void main()
{
	vec4 cubeMapColor = texture(CubeMapTex, normalize(ReflectDirection));
	vec4 vTexColour = texture(sampler0, vTexCoord);	
	vec4 color = mix(vTexColour, cubeMapColor, ReflectFactor);
	if(color.a < 0.1f)
	discard;

	vOutputColour = vec4(color.xyz,0.f);

}
