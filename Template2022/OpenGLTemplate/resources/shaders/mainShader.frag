#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform sampler2D sampler1;  // The texture sampler
uniform sampler2D sampler2;  // The texture sampler
uniform sampler2D sampler3;  // The texture sampler

uniform samplerCube CubeMapTex;
uniform bool renderSkybox;
uniform bool renderterrain;
uniform float fMinHeight, fMaxHeight;

in vec3 worldPosition;


void main()
{


	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition);

	} else {

		// Get the texel colour from the texture sampler
		if(renderterrain)
		{
			vec4 vTexColour0 = texture(sampler0, vTexCoord);	
			vec4 vTexColour1 = texture(sampler1, vTexCoord);
			vec4 vTexColour2 = texture(sampler2, vTexCoord);
			vec4 vTexColour3 = texture(sampler3, vTexCoord);
			float f = clamp(3*(worldPosition.y - fMinHeight) / (fMaxHeight - fMinHeight), 0, 3);
			vec4 vTexColour;
			if (f < 1)
				vTexColour = mix(vTexColour0, vTexColour1, f);
			else if (f < 2)
				vTexColour = mix(vTexColour1, vTexColour2, f - 1.0);
			else
				vTexColour = mix(vTexColour2, vTexColour3, f - 2.0);
			vOutputColour = vTexColour*vec4(vColour, 1.0f);
		}
		else
		{	
			vec4 vTexColour = texture(sampler0, vTexCoord);
			vOutputColour = vTexColour*vec4(vColour, 1.0f);
		}
	}
	
	
}
