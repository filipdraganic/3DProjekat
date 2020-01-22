#version 330 core
in vec4 FragPos;
out vec4 FragColor;
uniform vec3 lightPos;
uniform float far_plane;

in vec2 aTexCoord;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);   
    
    // mapiramo izmedju 0 i 1 tako sto delimo sa far_plane
    lightDistance = lightDistance / far_plane;
    
    // upisemo ovo kao modifikovanu daljinu
    gl_FragDepth = lightDistance;
    
}