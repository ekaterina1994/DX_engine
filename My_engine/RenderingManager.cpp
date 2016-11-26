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

	if (D3DHelper_CreateSwapChain(g_ApplicationPtr->m_uiManager, m_dxgiFactory, m_commandQueue, m_frameBufferCount, m_swapChain) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createRTVDescriptorHeap(m_device, m_frameBufferCount, m_rtvDescriptorHeap) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	if (D3DHelper_createDSVDescriptorHeap(m_device, m_depthStencilBuffer, g_ApplicationPtr->m_uiManager->getWidth(), g_ApplicationPtr->m_uiManager->getHeight(), m_dsvDescriptorHeap) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createViewportAndScissorRect(g_ApplicationPtr->m_uiManager->getWidth(), g_ApplicationPtr->m_uiManager->getHeight(), m_viewport, m_scissorRect) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	
	if (D3DHelper_createRenderTargets(m_device, m_frameBufferCount, m_rtvDescriptorHeap, m_swapChain, m_renderTargets) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createCommandAllocator(m_device, m_frameBufferCount, m_commandAllocator) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (FAILED(
		m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[m_currentRenderTargetNumber], NULL, IID_PPV_ARGS(&m_commandList))
	))
	{
		return EXIT_FAILURE;
	}

	if (D3DHelper_createFencesAndFenceEvent(m_device,m_frameBufferCount,m_fence, m_fenceEvent, m_fenceValue) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	/*
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
	*/
	return EXIT_SUCCESS;
}

int RenderingManager::HandleD3DError()
{
	// TODO: add d3derrors handling
	// stop rendering, something else
	// output debug window and so on

//	Running = false;
	//assert(false);

	OutputDebugString(L"D3D ERROR!");
	return EXIT_FAILURE;
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

	
	
	/*
	for each (auto model in g_ApplicationPtr->m_resourceManager->getScenePtr()->getModels())
	{
		model.second.m_geometry.m_indexBufferView;
		model.second.m_geometry.m_vertexBufferView;
		model.second.m_material.m_pipelineState;
		model.second.m_material.m_root_signature;
		model.second.m_position.m_position;
		model.second.m_position.m_rotMat;
		model.second.m_position.m_worldMat;
	}
	*/

	static float fakeTime = 0.0f;
	fakeTime += 0.0005f;
	XMMATRIX matRotZ = XMMatrixRotationZ(fakeTime);

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! https://msdn.microsoft.com/en-us/library/ms177202.aspx !
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (auto& model : g_ApplicationPtr->m_resourceManager->getScenePtr()->getModels())
	{
		auto& material = model.second.m_material;
		auto& geometry = model.second.m_geometry;

		m_commandList->SetPipelineState(material.m_pipelineState);
		m_commandList->SetGraphicsRootSignature(material.m_root_signature);

		m_commandList->IASetVertexBuffers(0, 1, &geometry.m_vertexBufferView);
		m_commandList->IASetIndexBuffer(&geometry.m_indexBufferView);

		XMFLOAT3 eyePos{300.0f, 300.0f, -300.0f};
		XMFLOAT3 targetPos{0.0f, 0.0f, 0.0f};
		XMFLOAT3 upVec{0.0f, 0.0f, 1.0f};

		XMMATRIX matView = XMMatrixLookAtRH(XMLoadFloat3(&eyePos), XMLoadFloat3(&targetPos), XMLoadFloat3(&upVec));
		XMMATRIX matProj = XMMatrixPerspectiveFovRH(XM_PIDIV4, 1.0f, 0.001f, 1000000.0f);
		XMMATRIX matViewProj = XMMatrixMultiplyTranspose(XMMatrixMultiply(matRotZ, matView), matProj);

		m_commandList->SetGraphicsRoot32BitConstants(0, 16, &matViewProj, 0); 

		m_commandList->DrawIndexedInstanced(geometry.m_numIndices, 1, 0, 0, 0);
	}

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentRenderTargetNumber], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	HRESULT hr;
	hr = m_commandList->Close();
	if (FAILED(
		hr
	))
	{
		return HandleD3DError();
	}

	return EXIT_SUCCESS;
}

int RenderingManager::RunCommandList()
{
	m_commandList->Close();

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
	return EXIT_SUCCESS;
}

int RenderingManager::RenderFrame()
{
	UpdatePipeline();

	RunCommandList();

	return EXIT_SUCCESS;
}

int RenderingManager::SubmitVertexBufferAndGetView(const Vertex* vertices, size_t numVertices, D3D12_VERTEX_BUFFER_VIEW &VBV)
{

	int vBufferSize = sizeof(Vertex) * numVertices;


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
	return EXIT_SUCCESS;
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
	int status = EXIT_SUCCESS;
	m_currentRenderTargetNumber = m_swapChain->GetCurrentBackBufferIndex();
	if (m_fence[m_currentRenderTargetNumber]->GetCompletedValue() < m_fenceValue[m_currentRenderTargetNumber])
	{
		if (FAILED(
			m_fence[m_currentRenderTargetNumber]->SetEventOnCompletion(m_fenceValue[m_currentRenderTargetNumber], m_fenceEvent)
		))
		{
			g_ApplicationPtr->Stop();
			status = EXIT_FAILURE;
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
	return EXIT_SUCCESS;
}

