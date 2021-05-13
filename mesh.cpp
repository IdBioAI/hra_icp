// mesh.cpp 
// Author: JJ
#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "texture.h"
#include "OBJloader.h"

bool loadOBJ(mesh & out_mesh, std::string obj_path)
{
	bool res;

	if (obj_path.size() == 0)
		return false;
	
	out_mesh.clear();
	out_mesh.primitive_type = GL_TRIANGLES;
	
	res = loadOBJ(obj_path.c_str(), out_mesh.vertices, out_mesh.texcoords, out_mesh.normals);
	if (!res)
		return false;

	if (out_mesh.normals.size() > 0)
	{
		out_mesh.normals_used = true;
		if (out_mesh.vertices.size() != out_mesh.normals.size())
			return false;
	}

	if (out_mesh.texcoords.size() > 0)
	{
		out_mesh.texture_used = true;
		if (out_mesh.vertices.size() != out_mesh.texcoords.size())
			return false;
	}

	return true;
}

bool loadTexture(mesh & out_mesh, std::string texture_path)
{
	if (texture_path.size() == 0)
	{
		out_mesh.texture_used = 0;
		return true;
	}

	out_mesh.tex_id = textureInit(texture_path.c_str(), false, false);
	out_mesh.texture_used = true;
	
	return true;
}

bool mesh_init(mesh & out_mesh, std::string obj_path, std::string texture_path)
{
	if (!loadOBJ(out_mesh, obj_path))
		return false;
	if (!loadTexture(out_mesh, texture_path))
		return false;

	return true;
}

// simple methods - example only (no colors, normals, etc...)
void mesh_draw_vertex(mesh& mesh)
{
	if (mesh.vertices.size() == 0)
		return;

	if (mesh.texture_used) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,mesh.tex_id);
	}

	// NOTICE: no pointer needed!
	glBegin(mesh.primitive_type);

	for (int i = 0; i < mesh.vertices.size(); ++i)
	{
		if (mesh.texture_used)
			glTexCoord2fv(glm::value_ptr(mesh.texcoords[i]));
			
		glVertex3fv(glm::value_ptr(mesh.vertices[i]));
	}

	glEnd();
}

void mesh_draw_vertex_both(mesh & mesh)
{
	if (mesh.vertices.size() == 0)
		return;

    if (mesh.indirect && mesh.vertices.size() == 0)
        return;
    
	glBegin(mesh.primitive_type);

	if (mesh.indirect)
    {
        for (auto i : mesh.indices )
    	{
    		glVertex3fv(glm::value_ptr(mesh.vertices[i]));
    	}
    }
    else 
    {
        for (auto i : mesh.vertices )
    	{
    		glVertex3fv(glm::value_ptr(i));
    	}
	}
    
	glEnd();
}

void mesh_draw_arrays(mesh & mesh)
{
	if (mesh.vertices.size() == 0)
		return;

	glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(mesh.primitive_type, 0, mesh.vertices.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void mesh_draw_arrayelement(mesh & mesh)
{
	if (mesh.indices.size() == 0)
		return;

	glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBegin(mesh.primitive_type);
	for (auto i : mesh.indices)
	{
		glArrayElement(i);
	}
	glEnd();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void mesh_draw_elements(mesh & mesh)
{
	if (mesh.indices.size() == 0)
		return;

	glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(mesh.primitive_type, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
}

// complex method

void mesh_draw(mesh & mesh)
{
	if (mesh.vertices.size() == 0)
		return;

	glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	if (mesh.normals_used)
	{
		glNormalPointer(GL_FLOAT, 0, mesh.normals.data());
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	if (mesh.texture_used)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, mesh.texcoords.data());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, mesh.tex_id);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	if (mesh.colors_used)
	{
		glColorPointer(4, GL_FLOAT, 0, mesh.colors.data());
		glEnableClientState(GL_COLOR_ARRAY);
	}

	// draw batch 
	glDrawArrays(mesh.primitive_type, 0, mesh.vertices.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	if (mesh.normals_used)
		glDisableClientState(GL_NORMAL_ARRAY);
	if (mesh.texture_used)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (mesh.colors_used)
		glDisableClientState(GL_COLOR_ARRAY);
}
