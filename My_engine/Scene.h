#pragma once
#include "stdafx.h"
#include <map>
#include <string>

#include "Model.h"

// It's not "clear" interface... but I want to keep name IScene for understandability
class Scene
{
public:
	int Update() { return EXIT_SUCCESS; }

	int SetUpGlobalPipelineState() { return EXIT_SUCCESS; }

	int AddModel(string name, Model&& inModel);

	map<string, Model>& getModels() { return m_models; }

	int ClearAll();

private:
	map<string, Model>		m_models;

};