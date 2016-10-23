#pragma once
#include "stdafx.h"
#include <map>
#include <string>

#include "Model.h"

// It's not "clear" interface... but I want to keep name IScene for understandability
class IScene
{
public:
	virtual int Update() = 0;

	int SetUpGlobalPipelineState() { return EXIT_SUCCESS; }

	int AddModel(string name, Model* inModel);

	int ClearAll();

private:
	map<string, Model*>		m_models;

};