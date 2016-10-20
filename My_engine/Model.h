#pragma once
#include "stdafx.h"
#include <string>

class Model 
{
	// Model is rather just a set of data than some worker instance
public:
	// data structures
	struct VertexBuffer{};
	struct IndexBuffer {};
	struct PipelineState {};
	struct Vector4X4{};
	struct Matrix4X4{};


	typedef string ShaderName;

	
public:
	Model();
	int setGeometry(VertexBuffer* vBuf, IndexBuffer* iBuf);
	int setShaders(ShaderName* vShader, ShaderName* pShader);
	int setPipelineState(PipelineState* pipelineState);

	int ClearAll();

private:
	// Skeletone of the model
		// Vertex buffer
		// Index buffer
	VertexBuffer* m_vertexBuffer;
	IndexBuffer*  m_indexBuffer;

	// положение в пространстве
	Vector4X4*    m_position;
	Matrix4X4*    m_worldMatrix;
	Matrix4X4*    m_rotationMatrix;


	// Rasterization
		// Shader names
		// Rasterizator state
	ShaderName*    m_vertexShaderName;
	ShaderName*    m_pixelShaderName;
	PipelineState* m_pipelineState;

	// Texture sampling
		// Textures data
		// Texture coordinates
		// Sampler state

	// textures support still isn't implemented :(
};