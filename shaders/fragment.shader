#version 450 core

out vec4 color;

in vec2 fragTextureCord;

uniform sampler2D textureSampler;
uniform vec3 ourColor;

void main()
{
    color = texture(textureSampler, fragTextureCord) * vec4(ourColor, 1.0f);
}
