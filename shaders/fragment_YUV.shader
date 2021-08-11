#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main(){
    float y = texture(texture1, TexCoord).r;
    float u = texture(texture2, TexCoord).r;
    float v = texture(texture3, TexCoord).r;

    y = 1.1643 * (y - 0.0625);
    u = u - 0.5;
    v = v - 0.5;
    
    float r = y               + 1.5958 * v;
    float g = y - 0.39173 * u - 0.81290 * v;
    float b = y + 2.017 * u;

    FragColor = vec4(r, g, b, 1.0);
}
//end