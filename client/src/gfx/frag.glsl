#version 330 core

#define MAX_POINT_LIGHTS 	8
#define MAX_SPOT_LIGHTS 	8

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec2 tex_coords;
in vec3 frag_pos;

uniform sampler2D shadow_map;
uniform vec3 view_pos;
uniform Material material;
uniform DirLight dir_light;

out vec4 FragColor;

vec3 dir_light_contribution(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	vec3 reflect_dir = reflect(-light_dir, normal);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));

	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = vec3(texture(material.diffuse, tex_coords))
		* diff
		* light.diffuse;

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	vec3 specular =
		vec3(texture(material.specular, tex_coords))
		* spec
		* light.specular;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 output_color = vec3(0.0);
	output_color += dir_light_contribution(dir_light, norm, view_dir);
	FragColor = vec4(output_color, 1.0);
}
