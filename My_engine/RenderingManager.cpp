#include "RenderingManager.h"
#include "stdafx.h"
#include "D3DHelper.h"

#include "Application.h"


RenderingManager::RenderingManager()
{
}

int RenderingManager::Init()
{
	if (D3DHelper_CreateDXGIFactory(m_dxgiFactory) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_CreateDevice(m_dxgiFactory, m_device) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_CreateCommandQueue(m_dxgiFactory, m_device, m_commandQueue) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_CreateSwapChain(g_ApplicationPtr->m_uiManager, m_dxgiFactory, m_commandQueue, m_frameBufferCount, m_swapchain) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createRTVDescriptorHeap(m_device, m_frameBufferCount, m_rtvDescriptorHeap) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	
	if (D3DHelper_createRenderTargets(m_device, m_frameBufferCount, m_rtvDescriptorHeap, m_swapchain, m_renderTargets) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createCommandAllocator(m_device, m_frameBufferCount, m_commandAllocator) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createFencesAndFenceEvent(m_device,m_frameBufferCount,m_fence, m_fenceEvent, m_fenceValue) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createRootSignature(m_device, m_rootSignature) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3dHelper_createShaderByteCode(m_vertexShaderBytecode, L"VertexShader.hlsl", "vs_5_0") == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3dHelper_createShaderByteCode(m_vertexShaderBytecode, L"PixelShader.hlsl", "ps_5_0") == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int RenderingManager::ClearAll()
{
	return EXIT_SUCCESS;
}

