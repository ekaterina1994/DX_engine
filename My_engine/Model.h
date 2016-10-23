#pragma once
#include "stdafx.h"

#include <string>
#include <vector>

class Model 
{
	// Model is rather just a set of data than some worker instance
public:
	// data structures
	typedef std::vector<float> VertexBuffer;
	typedef std::vector<int> IndexBuffer;
	struct PipelineState {};
	struct Vector4X4{};
	struct Matrix4X4{};

	struct CompiledShaderObject {};
	
public:
	Model();
	int setGeometry(string objFile);
	int setShaders(string vShader, string pShader);
	int setPipelineState(string pipelineState);

	int ClearAll();

private:
	// Skeletone of the model
		// Vertex buffer
		// Index buffer
	VertexBuffer* m_vertexBuffer;
	IndexBuffer*  m_indexBuffer;

	// положение в пространстве
	Vector4X4* m_position;
	Matrix4X4* m_worldMatrix;
	Matrix4X4* m_rotationMatrix;


	// Rasterization
		// Shader names
		// Rasterizator state
	CompiledShaderObject* m_vertexShaderName;
	CompiledShaderObject* m_pixelShaderName;
	PipelineState* m_pipelineState;

	// Texture sampling
		// Textures data
		// Texture coordinates
		// Sampler state

	// textures support still isn't implemented :(
};