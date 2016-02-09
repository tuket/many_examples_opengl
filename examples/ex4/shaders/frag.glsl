#version 120

varying vec3 var_color;

void main()
{
    if(var_color.r > var_color.g)
    {
        if(var_color.r > var_color.b)
        {
            gl_FragColor = vec4(1, 0, 0, 1);
        }
        else
        {
            gl_FragColor = vec4(0, 0, 1, 0);
        }
    }
    else
    {
        if(var_color.g > var_color.b)
        {
            gl_FragColor = vec4(0, 1, 0, 0);
        }
        else
        {
            gl_FragColor = vec4(0, 0, 1, 0);
        }
    }
}
