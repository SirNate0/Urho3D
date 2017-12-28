#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

#line 6

varying vec2 vTexCoord;
varying vec2 vScreenPos;

#ifdef COMPILEPS
uniform float cRotation;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vTexCoord = GetQuadTexCoord(gl_Position);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

// From https://gist.github.com/yiwenl/3f804e80d0930e34a0b33359259b556c
vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

void PS()
{
    vec3 rgb = texture2D(sDiffMap, vScreenPos).rgb;
    
    mat3 rgb2yiq = mat3(0.299, 0.596, 0.211,
                        0.587, -0.274, -0.523,
                        0.114, -0.322, 0.312);
    mat3 yiq2rgb = mat3(1, 1, 1,
                        0.956, -0.272, -1.106,
                        0.621, -0.647, 1.703);
    
    vec3 org = rgb2yiq * rgb;
    vec3 rot = org;
    rot.yz = rotate(org.yz, cRotation);
    
    vec3 final = yiq2rgb * rot;
    
    gl_FragColor = vec4(final, 1.0);
}

