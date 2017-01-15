#pragma once

#include "Scene.h"
#include "stdafx.h"

class TestScene01 : public Scene
{
public:

	int LoadModels() override
	{/*
		{
			Model model = Model();
			std::string modelName = "teste";
			if (createModelFromFile(std::string(modelName + ".obj").c_str(), model)) return FAIL;
			AddModel(modelName.c_str(), std::move(model));
		}
		*/
		{
			Model model = Model();
			std::string modelName = "dodecahedron";//"cube"
			if (createModelFromFile(std::string(modelName + ".obj").c_str(), model)) return FAIL;
			AddModel(modelName.c_str(), std::move(model));
		}

		/*
		{
			Model model = Model();
			if (createSceneBoundingBox(model)) return FAIL;
			AddModel("SceneBoundingBox", std::move(model));
		}
		*/
		
		return OK;
	}
};