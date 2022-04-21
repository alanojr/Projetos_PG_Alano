
#version 450

// uniform vec4 inputColor;
in vec4 vertexColor;
out vec4 color;

void main()
{
	color = vertexColor;
}