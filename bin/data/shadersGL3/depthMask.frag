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
    //color = vec4(0, 0, 10, 1);
    
    float dist = 255 * (maskColor.r + maskColor.g * 256);
    
    if (dist < threshold) {
        outputColor = color;
    }
    else if (dist < threshold + fadingDistance) {
        float k = (dist - threshold) / fadingDistance;
        outputColor = vec4(
                           k + color.r * (1 - k),
                           k + color.g * (1 - k),
                           k + color.b * (1 - k),
                           1
        );
    }
    else
        outputColor = vec4(1, 1, 1, 1);
}
