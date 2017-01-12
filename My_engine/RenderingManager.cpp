#include "RenderingManager.h"
#include "stdafx.h"
#include "D3DHelper.h"

#include "Application.h"


const float RenderingManager::m_clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

RenderingManager::RenderingManager()
{

}

int RenderingManager::Init()
{
	// enabling Debug layer
	{
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}

	if (D3DHelper_CreateDXGIFactory(m_dxgiFactory) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_CreateDevice(m_dxgiFactory, m_device) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_CreateCommandQueue(m_dxgiFactory, m_device, m_commandQueue) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_CreateSwapChain(g_ApplicationPtr->m_uiManager, m_dxgiFactory, m_commandQueue, m_frameBufferCount, m_swapChain) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_createRTVDescriptorHeap(m_device, m_frameBufferCount, m_rtvDescriptorHeap) == FAIL)
	{
		return FAIL;
	}

	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	if (D3DHelper_createDSVDescriptorHeap(m_device, m_depthStencilBuffer, g_ApplicationPtr->m_uiManager->getWidth(), g_ApplicationPtr->m_uiManager->getHeight(), m_dsvDescriptorHeap) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_createViewportAndScissorRect(g_ApplicationPtr->m_uiManager->getWidth(), g_ApplicationPtr->m_uiManager->getHeight(), m_viewport, m_scissorRect) == FAIL)
	{
		return FAIL;
	}
	
	if (D3DHelper_createRenderTargets(m_device, m_frameBufferCount, m_rtvDescriptorHeap, m_swapChain, m_renderTargets) == FAIL)
	{
		return FAIL;
	}

	if (D3DHelper_createCommandAllocator(m_device, m_frameBufferCount, m_commandAllocator) == FAIL)
	{
		return FAIL;
	}

	if (FAILED(
		m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[m_currentRenderTargetNumber], NULL, IID_PPV_ARGS(&m_commandList))
	))
	{
		return FAIL;
	}

	if (D3DHelper_createFencesAndFenceEvent(m_device,m_frameBufferCount,m_fence, m_fenceEvent, m_fenceValue) == FAIL)
	{
		return FAIL;
	}

	/*
	if (D3DHelper_createRootSignature(m_device, m_rootSignature) == FAIL)
	{
		return FAIL;
	}

	if (D3dHelper_createShaderByteCode(m_vertexShaderBytecode, L"VertexShader.hlsl", "vs_5_0") == FAIL)
	{
		return FAIL;
	}

	if (D3dHelper_createShaderByteCode(m_vertexShaderBytecode, L"PixelShader.hlsl", "ps_5_0") == FAIL)
	{
		return FAIL;
	}
	*/
	return OK;
}

int RenderingManager::HandleD3DError()
{
	// TODO: add d3derrors handling
	// stop rendering, something else
	// output debug window and so on

//	Running = false;
	//assert(false);

	OutputDebugString(L"D3D ERROR!");
	return FAIL;
}

int RenderingManager::UpdatePipeline()
{
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentRenderTargetNumber], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_currentRenderTargetNumber, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);	
	m_commandList->ClearRenderTargetView(rtvHandle, m_clearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//unPSO setup
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX* matViewProj = g_ApplicationPtr->m_resourceManager->getMatViewProj();

	for (auto& model : g_ApplicationPtr->m_resourceManager->getScenePtr()->getModels())
	{
		auto& material = model.second.m_material;
		auto& geometry = model.second.m_geometry;

		m_commandList->SetPipelineState(material.m_pipelineState);
		m_commandList->SetGraphicsRootSignature(material.m_root_signature);

		m_commandList->IASetVertexBuffers(0, 1, &geometry.m_vertexBufferView);
		m_commandList->IASetIndexBuffer(&geometry.m_indexBufferView);

		m_commandList->SetGraphicsRoot32BitConstants(0, 16, matViewProj, 0); 

		m_commandList->DrawIndexedInstanced(geometry.m_numIndices, 1, 0, 0, 0);
	}

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentRenderTargetNumber], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	return OK;
}

int RenderingManager::RunCommandList()
{
	HRESULT hr = m_commandList->Close();
	if (FAILED(
		hr
	))
	{
		return HandleD3DError();
	}

	ID3D12CommandList* ppCommandLists[] = { m_commandList };

	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowIfFailed(m_commandQueue->Signal(m_fence[m_currentRenderTargetNumber], 
		++m_fenceValue[m_currentRenderTargetNumber]));

	while (m_fence[m_currentRenderTargetNumber]->GetCompletedValue() != m_fenceValue[m_currentRenderTargetNumber])
	{
		// active waiting
	}

	ThrowIfFailed(m_commandAllocator[m_currentRenderTargetNumber]->Reset());

	ThrowIfFailed(m_commandList->Reset(m_commandAllocator[m_currentRenderTargetNumber], nullptr));

	ThrowIfFailed(m_swapChain->Present(0, 0));

	m_currentRenderTargetNumber = m_swapChain->GetCurrentBackBufferIndex();
	return OK;
}

int RenderingManager::RenderFrame()
{
	UpdatePipeline();

	RunCommandList();

	return OK;
}

int RenderingManager::SubmitVertexBufferAndGetView(const Vertex* vertices, size_t numVertices, D3D12_VERTEX_BUFFER_VIEW &VBV)
{
	int vBufferSize = static_cast<int>(sizeof(Vertex) * numVertices);

	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));
	
	m_vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	ID3D12Resource* vBufferUploadHeap;
	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
		nullptr,
		IID_PPV_ARGS(&vBufferUploadHeap));
	vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	static D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<const BYTE*>(vertices); // pointer to our vertex array
	vertexData.RowPitch = vBufferSize; // size of all our triangle vertex data
	vertexData.SlicePitch = vBufferSize; // also the size of our triangle vertex data

										 // we are now creating a command with the command list to copy the data from
										 // the upload heap to the default heap
	UpdateSubresources(m_commandList, m_vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);

	// transition the vertex buffer data from copy destination state to vertex buffer state
	ID3D12Fence* syncFence = nullptr;
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&syncFence));

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	RunCommandList();

	VBV.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	VBV.StrideInBytes = sizeof(Vertex);
	VBV.SizeInBytes = vBufferSize;
	return OK;
}

int RenderingManager::SubmitIndexBufferAndGetView(uint32_t* iArray, int size, D3D12_INDEX_BUFFER_VIEW& IBV, int& numIndicies)
{
	int iBufferSize = size;

//	numIndicies = size / sizeof(DWORD);

	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));

	m_indexBuffer->SetName(L"Index Buffer Resource Heap");

	ID3D12Resource* iBufferUploadHeap;
	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&iBufferUploadHeap));
	iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	static D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(iArray);
	indexData.RowPitch = iBufferSize;
	indexData.SlicePitch = iBufferSize;
	UpdateSubresources(m_commandList, m_indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);

	ID3D12Fence* syncFence = nullptr;
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&syncFence));

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	RunCommandList();

	IBV.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	IBV.Format = DXGI_FORMAT_R32_UINT;
	IBV.SizeInBytes = iBufferSize;
	return 0;
}

int RenderingManager::WaitForPreviousFrame()
{
	int status = OK;
	m_currentRenderTargetNumber = m_swapChain->GetCurrentBackBufferIndex();
	if (m_fence[m_currentRenderTargetNumber]->GetCompletedValue() < m_fenceValue[m_currentRenderTargetNumber])
	{
		if (FAILED(
			m_fence[m_currentRenderTargetNumber]->SetEventOnCompletion(m_fenceValue[m_currentRenderTargetNumber], m_fenceEvent)
		))
		{
			g_ApplicationPtr->Stop();
			status = FAIL;
		}
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
	m_fenceValue[m_currentRenderTargetNumber]++;
	return status;
}

int RenderingManager::ClearAll()
{
	// TODO: clear me m_depthStencilBuffer
	// todo release m_vertexBuffer and m_indexBuffer
	return OK;
}

