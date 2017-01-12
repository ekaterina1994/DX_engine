#pragma once
#include "Application.h"

#include "Model.h"
#include "Scene.h"
#include "D3DHelper.h"

#include <string>

class Scene;

class ResourceManager
{
public:

	ResourceManager();
	int Init();
	int Update();
	int ClearAll();
	int InitScene();
	Scene* getScenePtr() { return m_scene; }
	XMMATRIX* getMatViewProj();

private:
	Scene*	m_scene;
};
