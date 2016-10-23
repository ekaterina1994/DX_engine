#include "IScene.h"


int IScene::AddModel(string name, Model* inModel)
{
	m_models.insert({ name, inModel });
	return EXIT_SUCCESS;
}

int IScene::ClearAll()
{
	for each (auto model in m_models)
	{
		model.second->ClearAll();
		delete model.second;
	}

	return EXIT_SUCCESS;
}