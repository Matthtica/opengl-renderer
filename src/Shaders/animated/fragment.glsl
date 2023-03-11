#version 450 core

in vec2 TexCoord;
out vec4 fragColor;

uniform float t;
uniform vec3 start;
uniform vec3 end;

float frac(float v) {
    return v - floor(v);
}

vec3 frac(vec3 v) {
    vec3 res;
    res.x = frac(v.x);
    res.y = frac(v.y);
    res.z = frac(v.z);
    return res;
}

float lerp(float a, float b, float t) {
    return a + ((b - a) * t);
}

vec3 lerp(vec3 a, vec3 b, float t) {
    vec3 res;
    res.x = lerp(a.x, b.x, t);
    res.y = lerp(a.y, b.y, t);
    res.z = lerp(a.z, b.z, t);
    return res;
}

void main() {
    fragColor = vec4(lerp(start, end, (TexCoord.x + 1) / 2.0), 1.0);
    //fragColor = vec4(95.0f/255, 55.0f/255, 196.0f/255, 1.0);
}
