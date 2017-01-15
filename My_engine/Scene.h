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
	int Update();

	int SetUpGlobalPipelineState() { return OK; }

	int AddModel(string name, Model&& inModel);

	int AddModel(string name, Model& inModel);

	int Init();

	virtual int LoadModels() { return OK; };
	
	map<string, Model>& getModels() { return m_models; }
	
	XMMATRIX* getMatViewProj() { return &m_matViewProj; }

	int ClearAll();

	struct BoundingBox
	{
		float x0;
		float x1;

		float y0;
		float y1;

		float z0;
		float z1;
	};

protected:
	int createSceneBoundingBox(Model &);
	int createModelFromFile(string name, Model& model);
	std::vector<Vertex> scaleVBuffer(std::vector<Vertex>& vBuffer, float xMax, float yMax, float zMax);
	int Scene::getGeometry(Model::Geometry & geometry, Vertex* vArray, int vNumVerticies, uint32_t* iArray, int iSize, int numIndicies);
	int Scene::getMaterial(Model::Material & material, D3D12_FILL_MODE);
	int Scene::getPosition(Model::Position & position);
	int Scene::getShaderEnv(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, D3D12_SHADER_BYTECODE& vertexShaderBytecode, D3D12_SHADER_BYTECODE& pixelShaderBytecode);

private:
	map<string, Model>				m_models;
	XMMATRIX						m_matViewProj;
	float							m_fakeTime = 0;
	BoundingBox						m_boundingBox;
	int								m_maxEdge;
	//TODO: Camera							m_camera;

};