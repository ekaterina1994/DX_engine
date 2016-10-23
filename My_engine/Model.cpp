#include "Model.h"
#include "GeomertyConverter.h"

Model::Model()
{
	m_vertexBuffer		= nullptr;
	m_indexBuffer		= nullptr;
	m_position			= nullptr;
	m_worldMatrix		= nullptr;
	m_rotationMatrix	= nullptr;
	m_vertexShaderName	= nullptr;
	m_pixelShaderName	= nullptr;
	m_pipelineState		= nullptr;
}


int Model::setGeometry(string objFile)
{
	return ConvertGeomentry(objFile, m_vertexBuffer, m_indexBuffer);
}

int Model::setShaders(string vShader, string pShader)
{
	//m_vertexShaderName = vShader;
	//m_pixelShaderName  = pShader;
	return EXIT_SUCCESS;
}

int Model::setPipelineState(string pipelineState)
{
	m_pipelineState = nullptr;// pipelineState;
	return EXIT_SUCCESS;
}

int Model::ClearAll()
{
	if (m_vertexBuffer != nullptr)		delete m_vertexBuffer;
	if (m_indexBuffer != nullptr)		delete m_indexBuffer;
	if (m_position != nullptr)			delete m_position;
	if (m_worldMatrix != nullptr)		delete m_worldMatrix;
	if (m_rotationMatrix != nullptr)	delete m_rotationMatrix;
	if (m_vertexShaderName != nullptr)	delete m_vertexShaderName;
	if (m_pixelShaderName != nullptr)	delete m_pixelShaderName;
	if (m_pipelineState != nullptr)		delete m_pipelineState;

	return EXIT_SUCCESS;
}