#version 150

// special transparency based on the mask. see expansion

// these are our textures
uniform sampler2DRect tex0;
uniform sampler2DRect mask;
uniform float threshold;
uniform float fadingDistance;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;


void main()
{
    vec4 maskColor = texture(mask, texCoordVarying).rgba;
    vec4 color = texture(tex0, texCoordVarying).rgba;
    
    int distance = maskColor.r << 8 + maskColor.g;
    if (distance < threshold)
        outputColor = color;
    else
        outputColor = vec4(1, 1, 1, 1);
}
