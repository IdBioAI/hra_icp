#pragma once

#include <vector>
#include <string>

#include <glm/fwd.hpp>

class mesh
{
public:
	std::vector< glm::vec3 > vertices;
	std::vector< GLuint >    indices;
	std::vector< glm::vec4 > colors;
	std::vector< glm::vec2 > texcoords;
	std::vector< glm::vec3 > normals;
	std::string				 name;
	GLuint tex_id = 0;

    bool indirect = false; 
	bool texture_used = false;
	bool normals_used = false;
	bool colors_used = false;

	GLenum primitive_type = GL_POINTS;

	void clear(void)
	{
		delete_texture();
		vertices.clear();
		indices.clear();
		texcoords.clear();
		colors.clear();
		normals.clear();
		indirect = false;
        texture_used = false;
		normals_used = false;
		colors_used = false;
		primitive_type = GL_POINTS;
		tex_id = 0;
	};

	void delete_texture(void) { if (texture_used) glDeleteTextures(1, &tex_id); texture_used = false; };
};

bool loadOBJ(mesh & loaded_mesh, std::string obj_path);
bool loadTexture(mesh & out_mesh, std::string texture_path);
bool mesh_init(mesh & out_mesh, std::string obj_path, std::string texture_path);

// simple
void mesh_draw_vertex(mesh & mesh);
void mesh_draw_arrays(mesh & mesh);
void mesh_draw_arrayelement(mesh & mesh);
void mesh_draw_elements(mesh & mesh);

// complex
void mesh_draw(mesh & mesh);