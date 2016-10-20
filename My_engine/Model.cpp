#include "Model.h"


Model::Model()
{

}


int Model::setGeometry(VertexBuffer* vBuf, IndexBuffer* iBuf)
{
	m_vertexBuffer = vBuf;
	m_indexBuffer  = iBuf;
	return EXIT_SUCCESS;
}

int Model::setShaders(ShaderName* vShader, ShaderName* pShader)
{
	m_vertexShaderName = vShader;
	m_pixelShaderName  = pShader;
	return EXIT_SUCCESS;
}

int Model::setPipelineState(PipelineState* pipelineState)
{
	m_pipelineState = pipelineState;
	return EXIT_SUCCESS;
}

int Model::ClearAll()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_vertexShaderName;
	delete m_pixelShaderName;
	delete m_pipelineState;

	return EXIT_SUCCESS;
}