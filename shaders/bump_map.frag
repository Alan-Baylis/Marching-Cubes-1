#version 330 core

in vec3 light_vec;
in vec3 eye_vec;
in vec2 tex_coords;
uniform sampler2D texturemap;
uniform sampler2D normalmap;

out vec4 color;

void main()
{
	float inv_radius = 0.01;

	float distSqr = dot(light_vec, light_vec);
	float att = clamp(1.0 - inv_radius * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = light_vec * inversesqrt(distSqr);

	vec3 vVec = normalize(eye_vec);

	vec4 base = texture2D(texturemap, tex_coords);

	vec3 bump = normalize( texture2D(normalmap, tex_coords).xyz * 2.0 - 1.0);

	//vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	float diffuse = max( dot(lVec, bump), 0.0 );

	// vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse;
	vec4 vDiffuse = vec4(diffuse);

	//float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), gl_FrontMaterial.shininess );

	// vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

	// color = ( vAmbient*base + vDiffuse*base + vSpecular) * att;
	color = (vDiffuse*base) * att;
}