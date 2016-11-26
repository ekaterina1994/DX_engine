#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <D3D12SDKLayers.h>
#include "d3dx12.h"
#include <string>
#include "UIManager.h"
using namespace DirectX;

struct Vertex {

	Vertex(float px, float py, float pz, float pa, float nx, float ny, float nz, float na, float u, float v) :
		position(px, py, pz, pa), normal(nx, ny, nz, na), texCoord(u,v) {}

	XMFLOAT4 position;
	XMFLOAT4 normal;
	XMFLOAT2 texCoord;
};

// Helper functions
int D3DHelper_CreateDXGIFactory(IDXGIFactory4*&);
int D3DHelper_CreateDevice(IDXGIFactory4*&, ID3D12Device*&);
int D3DHelper_CreateCommandQueue(IDXGIFactory4*&, ID3D12Device*&, ID3D12CommandQueue*&);
int D3DHelper_CreateSwapChain(UIManager*&, IDXGIFactory4*&, ID3D12CommandQueue*&, int, IDXGISwapChain3*&);
int D3DHelper_createRTVDescriptorHeap(ID3D12Device*&, int, ID3D12DescriptorHeap*&);
int D3DHelper_createRenderTargets(ID3D12Device*&, int, ID3D12DescriptorHeap*&, IDXGISwapChain3*&, ID3D12Resource*[]);
int D3DHelper_createDSVDescriptorHeap(ID3D12Device*&, ID3D12Resource*&, int, int, ID3D12DescriptorHeap*&);
int D3DHelper_createViewportAndScissorRect(int, int, D3D12_VIEWPORT&, D3D12_RECT&);
int D3DHelper_createCommandAllocator(ID3D12Device*& , int , ID3D12CommandAllocator*[]);
int D3DHelper_createFencesAndFenceEvent(ID3D12Device*&, int, ID3D12Fence*[], HANDLE, UINT64[]);
int D3DHelper_createRootSignature(ID3D12Device*&, ID3D12RootSignature*&);
int D3dHelper_createShaderByteCode(D3D12_SHADER_BYTECODE, LPCWSTR, LPCSTR);

