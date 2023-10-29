#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

// NOTE: *** default uniforms ***
//
// vertexTexCoord2
// vertexNormal
// vertexTangent
// vertexColor
// matView
// matProjection
// matModel
// matNormal
// colDiffuse
// texture0
// texture1
// texture2

uniform sampler2D texture0;
uniform vec2 resolution;

// uniform vec4 coldiffuse;

out vec4 finalColor;

vec3 pix(vec2 pos) {
    pos.y = 1 - pos.y;
    // pos = clamp(pos, 0., 1.);
    return texture2D(texture0, pos).rgb;
}

vec3 w(vec2 pos) {
    pos.y = 1 - pos.y;
    // pos = clamp(pos, 0., 1.);
    return .5 - texture2D(texture0, pos).rgb;
}

float R = 0.04;
float r = 0.028;
float dt = 0.88;
float pi = 3.14159265359;

float alpha = 0.8;
float os = 0.0;
float scale = 2;

vec3 sigmoid(vec3 x) {
    return scale / (1 + exp(-alpha*x-os)) - 1;
}

// i big -> more blackness
vec3 s(vec3 i, vec3 o) {
    return sigmoid(sin(4*o - 5*i));
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 compute(vec2 pos) {
    float ic = 0;
    float oc = 0;
    vec3 inner = vec3(0);
    vec3 outer = vec3(0);
    for (float dy = -R; dy <= R; dy += 1./resolution.y) {
        for (float dx = -R; dx <= R; dx += 1./resolution.x) {
            if (dx*dx + dy*dy < r*r) {
                inner += w(vec2(pos.x + dx, pos.y + dy));
                ic += .5;
            } else if (dx*dx + dy*dy < R*R) {
                outer += w(vec2(pos.x + dx, pos.y + dy));
                oc += .5;
            }
        }
    }
    inner /= ic;
    outer /= oc;
    // return clamp(dt * s(inner, outer), 0., 1.);
    return clamp(pix(pos) + dt * s(inner, outer), 0., 1.);
    // return clamp(pix(pos), 0., 1.);
}


void main() {
    vec4 col = vec4(compute(fragTexCoord), 1.);
    finalColor = clamp(col, 0., 1.);
    // finalColor = vec4(fragTexCoord, 0., 1.);
    // finalColor = vec4(1., 0., 0., 1.);
}
