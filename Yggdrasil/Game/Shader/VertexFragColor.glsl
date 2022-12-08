#version 440

out vec4 FragColor;
in vec2 uv0;
//layout(binding=0) uniform sampler2D diffuse;
uniform sampler2D diffuse;

void main()
{
    //FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    FragColor = texture2D(diffuse, uv0);
}