#include "Scene.h"


int Scene::AddModel(string name, Model&& inModel)
{
	m_models.insert({ name, inModel });
	return EXIT_SUCCESS;
}

int Scene::AddModel(string name, Model& inModel) 
{
	m_models.insert({name, inModel});
	return EXIT_SUCCESS;
}

int Scene::ClearAll()
{
	return EXIT_SUCCESS;
}