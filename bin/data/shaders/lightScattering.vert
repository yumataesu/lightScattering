#version 410

in vec3 position;
in vec2 texcoord;

out vec2 v_texcoord;
out vec2 vUv;

void main() {
    gl_Position = vec4(position, 1.0);
    v_texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
}
