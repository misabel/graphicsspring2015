varying vec3 N;
varying vec3 v;

void main()
{
	N = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();
	v = vec3(gl_ModelViewMatrix * gl_Vertex);;
}