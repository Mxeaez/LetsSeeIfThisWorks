#version 460 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 color;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}