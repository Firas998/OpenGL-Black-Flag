#version 330 core

layout (location = 0) in vec3 position;  // vertex position in local space (x,y,z) 
layout (location = 3) in vec2 uv;        // vertex uv-texture (u,v)

out vec2 TexCoords;
out vec4 ParticleColor;



uniform vec3 textureindex;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	float scale = 10.0f;
	TexCoords = 0.2f*(uv+textureindex.xy);
	//vec3 CameraRight_worldspace = {view[0][0], view[1][0], view[2][0]};
	//vec3 CameraUp_worldspace = {view[0][1], view[1][1], view[2][1]};
	//vec3 vertexPosition_worldspace = offset3 
	//	+ CameraRight_worldspace * vertex.x * scale
	//	+ CameraUp_worldspace * vertex.y * scale;
	gl_Position = projection * view * model * vec4(position,1.0f);
}
