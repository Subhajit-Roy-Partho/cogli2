#version 330 core

out vec4 frag_color;

uniform vec3 ambient_light;
uniform vec3 camera_position;
uniform bool disable_lighting;

in vec4 vec_color;
in vec3 normal;
in vec3 world_pos;

void main() {
	if(disable_lighting) {
		frag_color = vec_color;
		return;
	}

    vec3 norm = normalize(normal);
    vec3 camera_dir = normalize(camera_position - world_pos);

	frag_color = vec_color * vec4(ambient_light, 1.f);
}
