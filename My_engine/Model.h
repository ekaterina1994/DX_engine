#pragma once
#include "stdafx.h"

#include "D3DHelper.h"

#include <string>
#include <vector>

struct Model 
{
	struct Material
	{
		ID3D12PipelineState*	m_pipelineState;
		ID3D12RootSignature*	m_root_signature;
	};

	struct Geometry
	{
		D3D12_VERTEX_BUFFER_VIEW	m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW		m_indexBufferView;
		int							m_numIndices; // TODO: init numIndices for models
	};

	struct Position
	{
		XMFLOAT4X4	m_worldMat;
		XMFLOAT4X4	m_rotMat;
		XMFLOAT4	m_position;
	};

	Model(Material& m, Geometry& g, Position& p)
	{
		m_material = m;
		m_geometry = g;
		m_position = p;
	}

	Model(Model& in)
	{
		m_material = in.m_material;
		m_position = in.m_position;
		m_geometry = in.m_geometry;
	}
	
	Material	m_material;
	Geometry	m_geometry;
	Position	m_position;

};