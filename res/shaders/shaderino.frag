uniform sampler2D texture;
uniform vec2 pos;
uniform float time;
uniform bool istouch;
uniform float fade;

float nsin(float x)
{
    return (sin(x)+1.0f)/2.0f;
}

void main() 
{
        // lookup the pixel in the texture
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
        vec4 color = vec4(0.0,0.0,0.0,1.0);//gl_Color;

        float d = distance(gl_TexCoord[0].st, vec2(0.5f,0.5f));
        if (d < 0.4f)
        {
            float cc = 0.5f + (0.5f) * nsin(time*8.0f);
            if (istouch)
            {
                color[0] = 1.0f;
                color[1] = 0.5f;
                color[2] = 0.5f;
            }
            else
            {
                color[0] = 0.5f;
                color[1] = 0.5f;
                color[2] = 1.0f;
            }
            color[3] = fade;
        }

       // multiply it by the color
       gl_FragColor = color * pixel;
}
