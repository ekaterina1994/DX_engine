#pragma once
#include "Application.h"

#include "Model.h"
#include "Scene.h"
#include "D3DHelper.h"

#include <string>

class ResourceManager
{
public:

	ResourceManager();
	int Init();
	int Update();
	int ClearAll();
	int InitScene();
	Scene* getScenePtr() { return m_scene; }
private:
	Scene*	m_scene;
	int getMaterial(Model::Material& material); 
	int getGeometry(Model::Geometry & geometry, Vertex* vArray, int vSize, uint32_t* iArray, int iSize, int numIndicies);
	int getPosition(Model::Position& position);
	int getShaderEnv(D3D12_INPUT_LAYOUT_DESC&, D3D12_SHADER_BYTECODE&, D3D12_SHADER_BYTECODE&);
	Model & createModelFromFile(string name);
};
