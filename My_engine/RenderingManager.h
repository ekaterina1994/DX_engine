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

public:
	RenderingManager();
	int Init();
	int ClearAll();

	int RenderFrame();

	ID3D12Device* getDevice()
	{
		return m_device;
	}

	int SubmitVertexBufferAndGetView(Vertex[], D3D12_VERTEX_BUFFER_VIEW&);

	int SubmitIndexBufferAndGetView(DWORD[], int, D3D12_INDEX_BUFFER_VIEW&, int&);
	//setupResourcelistener();


//	int getVertexBufferView();


private:
	//NotificateListeners();
	int RunCommandList();

	int UpdatePipeline();
	int WaitForPreviousFrame();
	int HandleD3DError();


	static const int 		m_frameBufferCount = 2;
	int						m_currentRenderTargetNumber;
	static const float 		m_clearColor[4];

	IDXGIFactory4*			m_dxgiFactory		= nullptr;
	ID3D12Device*			m_device			= nullptr;
	ID3D12CommandQueue*		m_commandQueue		= nullptr;
	IDXGISwapChain3*		m_swapChain			= nullptr;
	ID3D12DescriptorHeap*	m_rtvDescriptorHeap = nullptr;
	int						m_rtvDescriptorSize;
	ID3D12Resource*			m_depthStencilBuffer;
	ID3D12DescriptorHeap*	m_dsvDescriptorHeap	= nullptr; 
	ID3D12Resource*			m_renderTargets[m_frameBufferCount];
	ID3D12CommandAllocator* m_commandAllocator[m_frameBufferCount];

	ID3D12Fence*			m_fence[m_frameBufferCount];
	HANDLE					m_fenceEvent;
	UINT64					m_fenceValue[m_frameBufferCount];
	ID3D12GraphicsCommandList* m_commandList;
	D3D12_VIEWPORT			m_viewport;
	D3D12_RECT				m_scissorRect;
};


