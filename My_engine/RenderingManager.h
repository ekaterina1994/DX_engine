#pragma once
#include "stdafx.h"
#include "D3DHelper.h"

class RenderingManager
{
public:
	RenderingManager();
	int Init();
	int ClearAll();
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

