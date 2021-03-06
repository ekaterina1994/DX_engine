#include "ResourceManager.h"
#include "stdafx.h"
#include "TestScene01.h"

ResourceManager::ResourceManager()
{

}

int ResourceManager::Init()
{
	return OK;
}

int ResourceManager::Update() 
{
	return m_scene->Update();
}

int ResourceManager::InitScene()
{
	m_scene = new TestScene01();
	m_scene->Init();
	m_scene->LoadModels();
	return OK;
}

XMMATRIX * ResourceManager::getMatViewProj()
{
	return m_scene->getMatViewProj();
}

int ResourceManager::ClearAll()
{
	if (m_scene != nullptr)
	{
		delete m_scene;
	}
	return OK;
}
