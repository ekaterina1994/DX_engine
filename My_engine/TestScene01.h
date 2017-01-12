#pragma once

#include "Scene.h"
#include "stdafx.h"

class TestScene01 : public Scene
{
public:
	int Init() override
	{/*
		{
			Model model = Model();
			std::string modelName = "teste";//"teapot";
			if (createModelFromFile(std::string(modelName + ".obj").c_str(), model)) return FAIL;
			AddModel(modelName.c_str(), std::move(model));
		}
		*/
		{
			Model model = Model();
			if (createSceneBoundingBox(model)) return FAIL;
			AddModel("SceneBoundingBox", std::move(model));
		}

		return OK;
	}
};