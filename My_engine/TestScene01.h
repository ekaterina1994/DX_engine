#pragma once

#include "Scene.h"
#include "stdafx.h"

class TestScene01 : public Scene
{
public:
	int Init() override
	{
		Model model = Model();
		if (createModelFromFile("teapot.obj", model)) return FAIL;
		AddModel("teapot", std::move(model));
		return OK;
	}
};