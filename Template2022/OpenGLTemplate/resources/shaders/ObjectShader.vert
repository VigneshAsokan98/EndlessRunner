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
layout (location = 4) in mat4 inModelMat;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate

//Reflection Direction to Fragment shader
out vec3 ReflectDirection;

// This is the entry point into the vertex shader
void main()
{	 

		gl_Position = matrices.projMatrix * matrices.modelViewMatrix *
	vec4(inPosition, 1.0f);
	// Get the vertex normal and vertex position in eye coordinates
	vec3 n = normalize(matrices.normalMatrix * inNormal);
	vec3 p = (matrices.modelViewMatrix * vec4(inPosition, 1.0f)).xyz;
	ReflectDirection = (matrices.inverseViewMatrix * vec4(reflect(p, n), 1)).xyz;

	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	vColour = inColor + vec3(0.5f, 0.5f, 0.5f);
	
	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
