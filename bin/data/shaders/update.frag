#version 330

// ping pong inputs
uniform sampler2DRect particles0;
uniform sampler2DRect particles1;
uniform sampler2DRect particles2;

uniform vec3 mouse;
uniform vec2 resolution;
uniform float radiusSquared;
uniform float elapsed;

in vec2 texCoordVarying;

layout(location = 0) out vec4 posOut;
layout(location = 1) out vec4 velOut;
layout(location = 2) out vec4 colOut;

void main()
{
    vec3 pos = texture(particles0, texCoordVarying.st).xyz;
    vec3 vel = texture(particles1, texCoordVarying.st).xyz;
    vec3 col = texture(particles2, texCoordVarying.st).xyz;

    //  mass
    float mass =  100.0 * col.z * col.z * col.z;
    
    // mouse attraction
    vec3 direction = mouse - pos.xyz;
    float distSquared = dot(direction, direction);
    float magnitude = 2000.0 * (1.0 - distSquared / radiusSquared) / mass;
    vec3 force = step(distSquared, radiusSquared) * magnitude * normalize(direction);
    
    // gravity
    force += vec3(0.0, -0.5, 0.0);
    
    // accelerate
    vel += elapsed * force;
    
    // bounce off the sides
    vel.x *= step(abs(pos.x), 0.5 * resolution.x) * 2.0 - 1.0;
    vel.y *= step(abs(pos.y), 0.5 * resolution.y) * 2.0 - 1.0;
    
    // damping
    vel *= 0.995;
    
    // move
    pos += elapsed * vel;
    
    posOut = vec4(pos, 1.0);
    velOut = vec4(vel, 0.0);
    colOut = vec4(col, 1.0);
}