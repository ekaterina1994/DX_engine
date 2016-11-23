#include "ResourceManager.h"
#include "stdafx.h"

ResourceManager::ResourceManager()
{

}

int ResourceManager::Init()
{
	return EXIT_SUCCESS;
}

int ResourceManager::Update() 
{
	m_scene->Update();
	return EXIT_SUCCESS;
}

int ResourceManager::InitScene()
{
	Scene* scene = new Scene();
	/*
	Model::Material material;

	if (getMaterial(material))
	{
		return EXIT_FAILURE;
	}

	Model::Geometry geometry;

	if (getGeometry(geometry))
	{
		return EXIT_FAILURE;
	}

	Model::Position position;

	if (getPosition(position))
	{
		return EXIT_FAILURE;
	}

	scene->AddModel("cube", { material, geometry, position });
	*/

	scene->AddModel("cube", createModelFromFile("cube.obj"));

	m_scene = scene;
	return EXIT_SUCCESS;
}

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model && ResourceManager::createModelFromFile(string name)
{
	Model::Material material;
	Model::Geometry geometry;
	Model::Position position;
	/*
	std::string inputfile = "cornell_box.obj";
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

std::string err;
bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

if (!err.empty()) { // `err` may contain warning message.
  std::cerr << err << std::endl;
}

if (!ret) {
  exit(1);
}

// Loop over shapes
for (size_t s = 0; s < shapes.size(); s++) {
  // Loop over faces(polygon)
  size_t index_offset = 0;
  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
    int fv = shapes[s].mesh.num_face_vertices[f];

    // Loop over vertices in the face.
    for (size_t v = 0; v < fv; v++) {
      // access to vertex
      tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
      float vx = attrib.vertices[3*idx.vertex_index+0];
      float vy = attrib.vertices[3*idx.vertex_index+1];
      float vz = attrib.vertices[3*idx.vertex_index+2];
      float nx = attrib.normals[3*idx.normal_index+0];
      float ny = attrib.normals[3*idx.normal_index+1];
      float nz = attrib.normals[3*idx.normal_index+2];
      float tx = attrib.texcoords[2*idx.texcoord_index+0];
      float ty = attrib.texcoords[2*idx.texcoord_index+1];
    }
    index_offset += fv;

    // per-face material
    shapes[s].mesh.material_ids[f];
  }
}
	*/


	return { material, geometry, position };
}

int ResourceManager::getMaterial(Model::Material & material)
{
	HRESULT hr;
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineStateObject;

	ID3D12Device* device = g_ApplicationPtr->m_renderingManager->getDevice();

	CD3DX12_ROOT_PARAMETER rootParameters[1];
	rootParameters[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters,//no root parameters
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);

	ID3DBlob* signature;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(hr))
	{
		return false;
	}
	D3D12_INPUT_LAYOUT_DESC	inputLayoutDesc			= {};
	D3D12_SHADER_BYTECODE	vertexShaderBytecode	= {};
	D3D12_SHADER_BYTECODE	pixelShaderBytecode		= {};
	if (getShaderEnv(inputLayoutDesc, vertexShaderBytecode, pixelShaderBytecode) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	CD3DX12_RASTERIZER_DESC rastDesc(D3D12_DEFAULT);
	rastDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	//rastDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; // a structure to define a pso
	psoDesc.InputLayout = inputLayoutDesc; // the structure describing our input layout
	psoDesc.pRootSignature = rootSignature; // the root signature that describes the input data this pso needs
	psoDesc.VS = vertexShaderBytecode; // structure describing where to find the vertex shader bytecode and how large it is
	psoDesc.PS = pixelShaderBytecode; // same as VS but for pixel shader
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
	psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
	psoDesc.RasterizerState = rastDesc; // CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
	psoDesc.NumRenderTargets = 1; // we are only binding one render target
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state
																		   // create the pso
	hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateObject));

	if (!FAILED(hr) && pipelineStateObject != nullptr && rootSignature != nullptr)
	{
		material.m_pipelineState = pipelineStateObject;
		material.m_root_signature = rootSignature;
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

int ResourceManager::getGeometry(Model::Geometry & geometry)
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView; 
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	int numIndicies;

	Vertex vArray[] = {
		// front face
		{ -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// right side face
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// left side face
		{ -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// back face
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// top face
		{ -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },

		// bottom face
		{ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
	};

	DWORD iArray[] = {
		// ffront face
		0, 1, 2, // first triangle
		0, 3, 1, // second triangle

		// left face
		4, 5, 6, // first triangle
		4, 7, 5, // second triangle

		// right face
		8, 9, 10, // first triangle
		8, 11, 9, // second triangle

		// back face
		12, 13, 14, // first triangle
		12, 15, 13, // second triangle

		// top face
		16, 17, 18, // first triangle
		16, 19, 17, // second triangle

		// bottom face
		20, 21, 22, // first triangle
		20, 23, 21, // second triangle
	};

	
	if (g_ApplicationPtr->m_renderingManager->SubmitVertexBufferAndGetView(vArray, _countof(vArray), vertexBufferView))
	{
		return EXIT_FAILURE;
	}

	if (g_ApplicationPtr->m_renderingManager->SubmitIndexBufferAndGetView(iArray, sizeof(iArray),indexBufferView, numIndicies))
	{
		return EXIT_FAILURE;
	}

	geometry.m_indexBufferView = indexBufferView;
	geometry.m_vertexBufferView = vertexBufferView;
	geometry.m_numIndices = numIndicies;

	return EXIT_SUCCESS;
}

int ResourceManager::getPosition(Model::Position & position)
{
	return EXIT_SUCCESS;
}

int ResourceManager::getShaderEnv(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, D3D12_SHADER_BYTECODE& vertexShaderBytecode, D3D12_SHADER_BYTECODE& pixelShaderBytecode)
{
	HRESULT hr;
	ID3DBlob* vertexShader; // d3d blob for holding vertex shader bytecode
	ID3DBlob* errorBuff; // a buffer holding the error data if any
	hr = D3DCompileFromFile(L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShader,
		&errorBuff);

	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return EXIT_FAILURE;
	}

	// fill out a shader bytecode structure, which is basically just a pointer
	// to the shader bytecode and the size of the shader bytecode
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	// compile pixel shader
	ID3DBlob* pixelShader;
	hr = D3DCompileFromFile(L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShader,
		&errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return EXIT_FAILURE;
	}

	// fill out shader bytecode structure for pixel shader
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	// create input layout

	// The input layout is used by the Input Assembler so that it knows
	// how to read the vertex data bound to it.

	static D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// fill out an input layout description structure

	// we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
	inputLayoutDesc.NumElements = _countof(inputLayout);
	inputLayoutDesc.pInputElementDescs = inputLayout;
	return EXIT_SUCCESS;
}

int ResourceManager::ClearAll()
{
	if (m_scene != nullptr)
	{
		delete m_scene;
	}

	return EXIT_SUCCESS;
}
