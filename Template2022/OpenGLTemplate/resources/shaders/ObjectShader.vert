#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat4 inverseViewMatrix; 
	mat3 normalMatrix;
} matrices;
uniform vec3 WorldCameraPosition;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inColor;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate

//Reflection Direction to Fragment shader
out vec3 ReflectDirection;

struct PositionLightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // Light intensity
};
uniform PositionLightInfo Positionlight[2];

out vec4 p;
out vec3 n;
out vec3 reflected;

vec3 ADS(PositionLightInfo light, vec3 position, vec3 norm)
{
	vec3 s= normalize(light.Position.xyz - position);
	vec3 v = normalize(-position);
	vec3 r = reflect( -s, norm );
	vec3 I = light.Intensity;
	return
        I * ( .5 +
        .5 * max( dot(s, norm), 0.0 ) +
        .5 * pow( max( dot(r,v), 0.0 ), 15 ) );
		
}

// This is the entry point into the vertex shader
void main()
{	 

		
	// Transform the vertex spatial position using the projection and modelview matrices
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0);
	
	// Get the vertex normal and vertex position in eye coordinates
	n = normalize(matrices.normalMatrix * inNormal);
	p = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	// Evaluate the lighting equation for each Postionlight
	vec3 plColor = vec3(0.0);
	for( int i = 0; i < 2; i++ )
		plColor += ADS( Positionlight[i], p.xyz, n );
	vColour = plColor;

	ReflectDirection = (matrices.inverseViewMatrix * vec4(reflect(p.xyz, n), 1)).xyz;
	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
