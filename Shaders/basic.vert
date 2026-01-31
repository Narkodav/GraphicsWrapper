#version 450
layout(location = 0) in vec4 inPosition;
layout(location = 1) in uint inTextureId;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) flat out uint textureId;

layout(set = 0, binding = 0) uniform UniformTransforms {
    mat4 view;
    mat4 proj;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * inPosition;
    fragTexCoord = inPosition.xy * 0.5 + 0.5;  // Convert [-1,1] to [0,1]
    textureId = inTextureId;
}