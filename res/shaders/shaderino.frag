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
        if (d < 0.9f)
        {
            float cc = 0.5f + (0.5f) * nsin(time*8.0f);
            if (istouch)
            {
                color[0] = 0.7f;
                color[1] = 0.7f;
                color[2] = 0.7f;
            }
            else
            {
                color[0] = 0.5f;
                color[1] = 0.5f;
                color[2] = 0.5f;
            }
            color[3] = fade;
        }

       // multiply it by the color

       float size = 1.f/600.f;
       vec4 originalColor = vec4(0,0,0,0);
       float total = 0.f;
       for(float i =-8.f; i <= 8.f; ++i) {
           for(float j =-8.f; j <= 8.f; ++j) {
               float radio = abs(i)+abs(j);
               if (radio <= 8.f) originalColor += 8.f*(16.f-radio)*texture2D(texture,vec2(gl_TexCoord[0].s+(size*i), gl_TexCoord[0].t+(size*j)));
               total += 8.f*(16.f-radio);
           }
       }

       vec4 final = originalColor / total;
       final = final+ 0.3f*texture2D(texture,vec2(gl_TexCoord[0].st));
       gl_FragColor = final*1.2f*color;
}
