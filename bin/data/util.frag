#define PI 3.14159265358979

float random(vec2 n)
{
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

mat4 makeLookAt(vec3 eye, vec3 center, vec3 up)
{
    mat4 M;

    vec3 zaxis = normalize(eye - center);
    vec3 xaxis = normalize( cross(up, zaxis) );
    vec3 yaxis = cross(zaxis,xaxis);

    M[0] = vec4(xaxis,0);
    M[1] = vec4(yaxis,0);
    M[2] = vec4(zaxis,0);
    M[3] = vec4(eye,1);

    return M;
}
