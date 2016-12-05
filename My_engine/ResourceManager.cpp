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
	m_scene->Update();
	return OK;
}

int ResourceManager::InitScene()
{
	m_scene = new TestScene01();
	m_scene->Init();
	return OK;
}


int ResourceManager::ClearAll()
{
	if (m_scene != nullptr)
	{
		delete m_scene;
	}

	return OK;
}
