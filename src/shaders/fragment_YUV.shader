#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main(){
    vec3 yuv = vec3(1.1643 *(texture(texture1, TexCoord).r - 0.0625), (texture(texture2, TexCoord).r - 0.5), (texture(texture3, TexCoord).r - 0.5));
    
    vec3 rgb = mat3(
        1,       1,       1,
        0.0,    -0.39173, 2.017,
        1.5958, -0.81290, 0     ) * yuv;

    FragColor = vec4(rgb, 1.0);
}
