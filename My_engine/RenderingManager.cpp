#include "RenderingManager.h"
#include "stdafx.h"
#include "D3DHelper.h"

#include "Application.h"


const float RenderingManager::m_clearColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };

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
	if (FAILED(
		m_commandAllocator[m_currentRenderTargetNumber]->Reset()
	))
	{
		/*return*/ HandleD3DError();
	}

	if (FAILED(
		m_commandList->Reset(m_commandAllocator[m_currentRenderTargetNumber], nullptr)
	))
	{
		/*return*/ HandleD3DError();
	}

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


	for each (auto& model in g_ApplicationPtr->m_resourceManager->getScenePtr()->getModels())
	{
		auto& material = model.second.m_material;
		auto& geometry = model.second.m_geometry;
		m_commandList->SetPipelineState(material.m_pipelineState);
		m_commandList->SetGraphicsRootSignature(material.m_root_signature);

		//m_commandList->IASetVertexBuffers(0, 1, &geometry.m_vertexBufferView);
		//m_commandList->IASetIndexBuffer(&geometry.m_indexBufferView);

		// without constans buffer now
		// later I plan to add nocstant buffer
		// TODO: implement constant buffers support
		// maybe it will be more generic step, I mean, we have to bind all resources, described inside rootsignature, not only constant buffer...
		// m_commandList->SetGraphicsRootConstantBufferView(0, constantBufferUploadHeaps[m_currentRenderTargetNumber]->GetGPUVirtualAddress());

		//m_commandList->DrawIndexedInstanced(geometry.m_numIndices, 1, 0, 0, 0);
		m_commandList->DrawInstanced(4, 1, 0, 0);
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
	ID3D12CommandList* ppCommandLists[] = { m_commandList };

	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);


	// TODO: create different functions for logic below
	// Update pipeline (filling command list)
	// Execute command lists
	// signal to next frame
	// swap chain present

	// TODO: we probasbly will have one command list per instance, no?

	if (FAILED(
		m_commandQueue->Signal(m_fence[m_currentRenderTargetNumber], m_fenceValue[m_currentRenderTargetNumber])
	))
	{
		return HandleD3DError();
	}

	if (FAILED(
		m_swapChain->Present(0, 0)
	))
	{
		return HandleD3DError();
	}

	return EXIT_SUCCESS;
}

int RenderingManager::RenderFrame()
{
	UpdatePipeline();
	RunCommandList();
/*
	static bool resources_pushed = false;
	if (!resources_pushed)
	{
		HRESULT hr = m_commandList->Close();
		if (FAILED(
			hr
		))
		{
			return HandleD3DError();
		}

		if (RunCommandList() == EXIT_SUCCESS)
		{
			resources_pushed = true;
		}
	}
*/
	// TODO: UPDATEsubresources!! We have to check here, did we do initial uploading resources!
	if (WaitForPreviousFrame() != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	if (UpdatePipeline() != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	if (RunCommandList() != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int RenderingManager::SubmitVertexBufferAndGetView(Vertex vArray[], D3D12_VERTEX_BUFFER_VIEW &VBV)
{
	ID3D12Resource* vertexBuffer;// TODO:should became member of this manager
	int vBufferSize = sizeof(vArray);

	// create default heap
	// default heap is memory on the GPU. Only the GPU has access to this memory
	// To get data into this heap, we will have to upload the data using
	// an upload heap
	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
										// from the upload heap to this heap
		nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
		IID_PPV_ARGS(&vertexBuffer));

	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	// create upload heap
	// upload heaps are used to upload data to the GPU. CPU can write to it, GPU can read from it
	// We will upload the vertex buffer using this heap to the default heap
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
	vertexData.pData = reinterpret_cast<BYTE*>(vArray); // pointer to our vertex array
	vertexData.RowPitch = vBufferSize; // size of all our triangle vertex data
	vertexData.SlicePitch = vBufferSize; // also the size of our triangle vertex data

										 // we are now creating a command with the command list to copy the data from
										 // the upload heap to the default heap
	UpdateSubresources(m_commandList, vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);

	// transition the vertex buffer data from copy destination state to vertex buffer state
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));


	VBV.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	VBV.StrideInBytes = sizeof(Vertex);
	VBV.SizeInBytes = vBufferSize;
	return EXIT_SUCCESS;
}

int RenderingManager::SubmitIndexBufferAndGetView(DWORD iArray[], int size, D3D12_INDEX_BUFFER_VIEW& IBV, int& numIndicies)
{
	ID3D12Resource* indexBuffer; // TODO: make it mamber of rendering manager class, release it!!

	int iBufferSize = size;

	numIndicies = size / sizeof(DWORD);

	// create default heap to hold index buffer
	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_COPY_DEST, // start in the copy destination state
		nullptr, // optimized clear value must be null for this type of resource
		IID_PPV_ARGS(&indexBuffer));

	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	indexBuffer->SetName(L"Index Buffer Resource Heap");

	// create upload heap to upload index buffer
	ID3D12Resource* iBufferUploadHeap;
	m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
		nullptr,
		IID_PPV_ARGS(&iBufferUploadHeap));
	iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	static D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(iArray); // pointer to our index array
	indexData.RowPitch = iBufferSize; // size of all our index buffer
	indexData.SlicePitch = iBufferSize; // also the size of our index buffer

										// the upload heap to the default heap
	UpdateSubresources(m_commandList, indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);

	// transition the vertex buffer data from copy destination state to vertex buffer state
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	IBV.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	IBV.Format = DXGI_FORMAT_R32_UINT; // 32-bit unsigned integer (this is what a dword is, double word, a word is 2 bytes)
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
	return EXIT_SUCCESS;
}

