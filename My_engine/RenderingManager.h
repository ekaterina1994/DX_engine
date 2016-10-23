#pragma once
#include "stdafx.h"
#include "Application.h"
#include "D3DHelper.h"

class RenderingManager
{
	class Object
	{
	public:
		Object() {};
		int Init() {};
	private:

	};

	struct Vertex {
		Vertex(float x, float y, float z, float r, float g, float b, float a) : pos(x, y, z), color(r, g, b, a) {}
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

public:
	RenderingManager();
	int Init();
	int ClearAll();
	int Update();
	int RenderFrame();

private:
	const int static m_frameBufferCount = 3;

	IDXGIFactory4*			m_dxgiFactory		= nullptr;
	ID3D12Device*			m_device			= nullptr;
	ID3D12CommandQueue*		m_commandQueue		= nullptr;
	IDXGISwapChain3*		m_swapchain			= nullptr;
	ID3D12DescriptorHeap*	m_rtvDescriptorHeap = nullptr;
	ID3D12Resource*			m_renderTargets[m_frameBufferCount];
	ID3D12CommandAllocator* m_commandAllocator[m_frameBufferCount];
	ID3D12Fence*			m_fence[m_frameBufferCount];
	HANDLE					m_fenceEvent;
	UINT64					m_fenceValue[m_frameBufferCount];
	ID3D12RootSignature*	m_rootSignature = nullptr;
	D3D12_SHADER_BYTECODE	m_vertexShaderBytecode;
	D3D12_SHADER_BYTECODE	m_pixelShaderBytecode;
};

