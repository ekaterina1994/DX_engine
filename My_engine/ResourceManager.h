#pragma once
#include "Application.h"
#include <string>

class ResourceManager
{
public:
	ResourceManager();
	int Init();
	int ClearAll();
	int InitSceneFromConfigFile(std::string configFile);
private:
};

