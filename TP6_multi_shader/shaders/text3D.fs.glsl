#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;


uniform sampler2D uTexture;


void main()
{
	vec2 fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y);
	// fFragColor = vec4(vec3(normalize(vNormal_vs)), 1.);

	fFragColor = texture(uTexture, fCorrectCoords);

}
