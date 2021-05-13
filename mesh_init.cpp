#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh_init.h"
#include "texture.h"

mesh gen_mesh_brick(const float brick_size)
{
	mesh mesh;
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));
	
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));

	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));

	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));

	
	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.tex_id = textureInit("resources/wall2.bmp", false, false);

	return mesh;
}

mesh gen_mesh_grass(const float brick_size)
{
	mesh mesh;



	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));

	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));

	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(0.0f, brick_size, 0.0f));

	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, 0.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, 0.0f, -brick_size));
	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.tex_id = textureInit("resources/grass.png", false, true);

	return mesh;
}

mesh gen_mesh_ceiling(const float brick_size)
{
	mesh mesh;

	mesh.texcoords.push_back(glm::vec2(0.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, -brick_size));
	mesh.texcoords.push_back(glm::vec2(1.0f, 1.0f));
	mesh.vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	mesh.texcoords.push_back(glm::vec2(1.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(1.0f, brick_size, 0.0f));
	mesh.texcoords.push_back(glm::vec2(0.0f, 0.0f));
	mesh.vertices.push_back(glm::vec3(brick_size, brick_size, 0.0f));


	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.tex_id = textureInit("resources/str.bmp", false, false);

	return mesh;
}

mesh gen_mesh_gun(int gun)
{
	mesh mesh;

	// top
	mesh.texcoords.push_back(glm::vec2(0.93, 0.78));
	mesh.vertices.push_back(glm::vec3(-0.3, -1, 0));
	mesh.texcoords.push_back(glm::vec2(1.0, 0.78));
	mesh.vertices.push_back(glm::vec3(0.3, -1, 0));
	mesh.texcoords.push_back(glm::vec2(1.0, 0.0));
	mesh.vertices.push_back(glm::vec3(0.3, -1, -4));
	mesh.texcoords.push_back(glm::vec2(0.92, 0.0));
	mesh.vertices.push_back(glm::vec3(-0.3, -1, -4));

	//back
	mesh.texcoords.push_back(glm::vec2(0.0, 0.39));
	mesh.vertices.push_back(glm::vec3(-0.3, -1.4, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.07, 0.39));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.07, 0.27));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, 0.0));
	mesh.texcoords.push_back(glm::vec2(0.0, 0.27));
	mesh.vertices.push_back(glm::vec3(-0.3, -1.0, 0.0));

	//right
	mesh.texcoords.push_back(glm::vec2(0.0, 0.1));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, 0));
	mesh.texcoords.push_back(glm::vec2(0.75, 0.1));
	mesh.vertices.push_back(glm::vec3(0.3, -1.0, -4));
	mesh.texcoords.push_back(glm::vec2(0.75, 0.2));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, -4));
	mesh.texcoords.push_back(glm::vec2(0.0, 0.2));
	mesh.vertices.push_back(glm::vec3(0.3, -1.4, 0));

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;

	if(gun == 1)
		mesh.tex_id = textureInit("resources/gun.bmp", false, false);
	else if(gun == 2)
		mesh.tex_id = textureInit("resources/gun2.bmp", false, false);
	

	return mesh;
}

mesh gen_mesh_floor(const cv::Mat & map, const float brick_size)
{
	mesh mesh;

	for (int j = 0; j < map.cols; j++) {
		for (int i = 0; i < map.rows; i++) {
			mesh.texcoords.push_back(glm::vec2(0.0, 1.0));
			mesh.vertices.push_back(glm::vec3(brick_size * j, brick_size + brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(1.0, 1.0));
			mesh.vertices.push_back(glm::vec3(brick_size + brick_size * j, brick_size + brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(1.0, 0.0));
			mesh.vertices.push_back(glm::vec3(brick_size + brick_size * j, brick_size * i, 0.0f));

			mesh.texcoords.push_back(glm::vec2(0.0, 0.0));
			mesh.vertices.push_back(glm::vec3(brick_size * j, brick_size * i, 0.0f));
		}
	}

	mesh.primitive_type = GL_QUADS;
	mesh.texture_used = true;
	mesh.tex_id = textureInit("resources/ground.bmp", false, false);

	return mesh;
}

void gen_mesh_magazines(std::vector<mesh> & magazines)
{
	magazines.clear();

	for (int i = 0; i < 6; i++)
	{
		float texture_coord_w0, texture_coord_w1;
		mesh mesh;

		switch (i)
		{
		case 0:
			texture_coord_w0 = 0.27f; texture_coord_w1 = 0.06f;
			break;
		case 1:
			texture_coord_w0 = 0.00f; texture_coord_w1 = 0.32f;
			break;
		case 2:
			texture_coord_w0 = 0.09f; texture_coord_w1 = 0.49f;
			break;
		case 3:
			texture_coord_w0 = 0.27f; texture_coord_w1 = 0.66f;
			break;
		case 4:
			texture_coord_w0 = 0.44f; texture_coord_w1 = 0.83f;
			break;
		case 5:
			texture_coord_w0 = 0.62f; texture_coord_w1 = 1.00f;
			break;
		default:
			std::cerr << "Da fuq?" << std::endl;
			exit(1);
			break;
		}

		mesh.texcoords.push_back(glm::vec2(0.0f, texture_coord_w1));
		mesh.vertices.push_back(glm::vec3(1.7f, -1.0f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.45f, texture_coord_w1));
		mesh.vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.45f, texture_coord_w0));
		mesh.vertices.push_back(glm::vec3(1.0f, -1.5f, 0.0f));
		mesh.texcoords.push_back(glm::vec2(0.0f, texture_coord_w0));
		mesh.vertices.push_back(glm::vec3(1.7f, -1.5f, 0.0f));

		mesh.primitive_type = GL_QUADS;
		mesh.texture_used = true;

		magazines.push_back(mesh);
	}
}


/**********************************************************************************************************************/

mesh gen_mesh_circle(const float radius, const unsigned int num_segments)
{
	mesh mesh;
	float theta;
	glm::vec3 vertex;

	mesh.primitive_type = GL_LINE_LOOP;

	for (unsigned int u = 0; u < num_segments; u++)
	{
		theta = 2.0f * 3.1415926f * float(u) / float(num_segments);

		vertex.x = radius * cosf(theta);
		vertex.y = radius * sinf(theta);
		vertex.z = 0.0;

		mesh.vertices.push_back(vertex);
		mesh.indices.push_back(u);
	}

	return mesh;
}

