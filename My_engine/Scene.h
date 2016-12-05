#pragma once
#include "stdafx.h"
#include <map>
#include <string>
#include <math.h>

#include "tiny_obj_loader.h"
#include "Model.h"
#include "Application.h"
// TODO: define here camera, view matricies and virtual methiods to update motion
// It's not "clear" interface... but I want to keep name IScene for understandability
class Scene
{
public:
	int Update() { return OK; }

	int SetUpGlobalPipelineState() { return OK; }

	int AddModel(string name, Model&& inModel);

	int AddModel(string name, Model& inModel);

	virtual int Init();

	map<string, Model>& getModels() { return m_models; }

	int ClearAll();

protected:
	int createModelFromFile(string name, Model& model);
	std::vector<Vertex> scaleVBuffer(std::vector<Vertex>& vBuffer, float xMax, float yMax, float zMax);
	int Scene::getGeometry(Model::Geometry & geometry, Vertex* vArray, int vNumVerticies, uint32_t* iArray, int iSize, int numIndicies);
	int Scene::getMaterial(Model::Material & material);
	int Scene::getPosition(Model::Position & position);
	int Scene::getShaderEnv(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, D3D12_SHADER_BYTECODE& vertexShaderBytecode, D3D12_SHADER_BYTECODE& pixelShaderBytecode);

private:
	map<string, Model>				m_models;

};