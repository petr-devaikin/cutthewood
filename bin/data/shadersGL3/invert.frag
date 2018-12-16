#version 150

// special transparency based on the mask. see expansion

// these are our textures
uniform sampler2DRect tex0;
uniform sampler2DRect img;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;


void main()
{
    vec4 color = texture(tex0, texCoordVarying).rgba;
    vec4 imgColor = texture(img, texCoordVarying).rgba;
    
    if (color.r == 1)
        outputColor = vec4(1 - imgColor.r, 1 - imgColor.g, 1 - imgColor.b, 1);
    else
        outputColor = imgColor;
}
