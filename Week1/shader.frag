#if __VERSION__ >= 140
out vec4 fColor;
#endif

void main()
{
#if __VERSION__ >= 140
    fColor = vec4(0.0, 0.0, 0.0, 1.0);
#else
    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
#endif
}

