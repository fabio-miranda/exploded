

void main(void)
{
    gl_FrontColor = vec4(gl_Vertex.xyz, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}