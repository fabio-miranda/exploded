varying vec2 texCoord;
varying vec4 pos;
varying vec4 posInterpolated;

void main()
{
  gl_Position = ftransform();
  texCoord = vec2(gl_MultiTexCoord0);
  pos =  gl_Position;
  posInterpolated = vec4(gl_Vertex.xyz, 1.0);
}
