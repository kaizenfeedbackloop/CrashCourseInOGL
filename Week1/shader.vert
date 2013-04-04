#if __VERSION__ >= 140
in vec4  vPosition;
#else
attribute vec4 vPosition;
#endif

void main()
{
    gl_Position = vPosition;
}

