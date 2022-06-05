#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;


uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;
uniform vec2 textureindex;
uniform mat4 view;


void main()
{
	float scale = 10.0f;
	TexCoords = 0.2f*vertex.zw+textureindex;
	ParticleColor = color;
	vec3 CameraRight_worldspace = {view[0][0], view[1][0], view[2][0]};
	vec3 CameraUp_worldspace = {view[0][1], view[1][1], view[2][1]};
	vec3 offset3 = {offset.x, offset.y, 0};
	vec3 vertexPosition_worldspace = offset3 
		+ CameraRight_worldspace * vertex.x * scale
		+ CameraUp_worldspace * vertex.y * scale;
	gl_Position = projection * vec4(vertexPosition_worldspace,1.0f);
}
