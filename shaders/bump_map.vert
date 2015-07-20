#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 light_vec;
out vec3 eye_vec;
out vec2 tex_coords;

vec3 to_sphere(vec3 P)
{
    float r = sqrt(P.x*P.x + P.y*P.y + P.z*P.z);
    float theta = atan(P.y,(P.x+1E-18));
    float phi= acos(P.z/r); // in [0,pi]
    return vec3(r,theta, phi);
}

vec2 calcUV(vec3 p)
{
    p = normalize(p);

    const float PI = 3.1415926f;

    float u = ((atan(p.x, p.z) / PI) + 1.0f) * 0.5f;
    float v = (asin(p.y) / PI) + 0.5f;

    return vec2(u, v);
}

void main()
{
	vec3 light_position = vec3(-2.0, 2.0, 1.0);
	///////////////////////////////////////// obliczanie wspolrzednych tekstury UV:
	// http://stackoverflow.com/questions/22980246/i-want-to-wrap-an-image-around-a-sphere-in-opengl
	vec3 tex_coords3 = to_sphere(position);
	// tex_coords = vec2(atan(position.y, position.x), atan(position.z, position.x));
	tex_coords = calcUV(position);

	///////////////////////////////////////// obliczanie N, T, B
	vec3 tangent;
	vec3 binormal;

	vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
	vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

	if(length(c1) > length(c2))
	{
		tangent = c1;
	}
	else
	{
		tangent = c2;
	}

	tangent = normalize(tangent);

	binormal = cross(normal, tangent);
	binormal = normalize(binormal);

	///////////////////////////////////////// obliczanie
	vec4 vVertex = view * model * vec4(position, 1.0);
	vec4 temp_vec = vec4(light_position, 1.0) - vVertex;

	light_vec.x = dot(temp_vec, vec4(tangent, 1.0));
	light_vec.y = dot(temp_vec, vec4(binormal, 1.0));
	light_vec.z = dot(temp_vec, vec4(normal, 1.0));

	temp_vec = -vVertex;
	eye_vec.x = dot(temp_vec, vec4(tangent, 1.0));
	eye_vec.y = dot(temp_vec, vec4(binormal, 1.0));
	eye_vec.z = dot(temp_vec, vec4(normal, 1.0));

	gl_Position = projection * view * model * vec4(position, 1.0f);
}