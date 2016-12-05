#pragma once
#include "stdafx.h"
#include "Model.h"

#include "tiny_obj_loader.h"

int ConvertGeomentry(	string objFile,
						Model::Vertex*& vertexBuffer,
						uint64_t& indexBuffer)
{/*
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFile.c_str(), "Models/", true);

	if (shapes.size() != 0 || attrib.vertices.size() != 0)
	{
		vertexBuffer->assign(attrib.vertices.begin(), attrib.vertices.end());
		for each (auto indices in shapes[0].mesh.indices)
		{
			indexBuffer->push_back(indices.vertex_index);
		}
	}*/
	return OK;
}