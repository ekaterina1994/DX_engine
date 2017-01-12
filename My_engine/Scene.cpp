#include "Scene.h"


int Scene::Update()
{
	m_fakeTime += 0.1f;
	XMMATRIX matRotZ = XMMatrixRotationZ(m_fakeTime);

	//static float y_pos = .0f;
	//		y_pos += float(0.001);
	XMFLOAT3 eyePos{ 20.0f, 20.0f, 20.0f };
	XMFLOAT3 targetPos{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 upVec{ 0.0f, 0.0f, 1.0f };

	XMMATRIX matView = XMMatrixLookAtRH(XMLoadFloat3(&eyePos), XMLoadFloat3(&targetPos), XMLoadFloat3(&upVec));
	XMMATRIX matProj = XMMatrixPerspectiveFovRH(XM_PIDIV4, 1.0f, 0.1f, 1000000.0f);
	m_matViewProj = XMMatrixMultiplyTranspose(XMMatrixMultiply(matRotZ, matView), matProj);

	return OK;
}

int Scene::AddModel(string name, Model&& inModel)
{
	m_models.insert({ name, inModel });
	return OK;
}

int Scene::AddModel(string name, Model& inModel) 
{
	m_models.insert({name, inModel});
	return OK;
}

int Scene::Init()
{
	return OK;
}

int Scene::createSceneBoundingBox(Model & model)
{
	/*
		m_camera->getBoundingBox();
	*/
	static Model::Material material;
	Model::Geometry geometry;
	Model::Position position;

	static std::vector<Vertex> vBuffer = 
	{
		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z0, 0,0,0,-1,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z0, 0,0,0,-1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z0, 0,0,0,-1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z0, 0,0,0,-1,0,0,0),
		
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z0, 0,0,1,0,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z0, 0,0,1,0,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z1, 0,0,1,0,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z1, 0,0,1,0,0,0,0),

		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z0, 0,0,-1,0,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z0, 0,0,-1,0,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z1, 0,0,-1,0,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z1, 0,0,-1,0,0,0,0),

		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z1, 0,0,0,1,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z1, 0,0,0,1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z1, 0,0,0,1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z1, 0,0,0,1,0,0,0),

		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z0, 0,-1,0,0,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y0, m_boundingBox.z1, 0,-1,0,0,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z1, 0,-1,0,0,0,0,0),
		Vertex(m_boundingBox.x0, m_boundingBox.y1, m_boundingBox.z0, 0,-1,0,0,0,0,0),

		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z0, 0,1,0,1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y0, m_boundingBox.z1, 0,1,0,1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z1, 0,1,0,1,0,0,0),
		Vertex(m_boundingBox.x1, m_boundingBox.y1, m_boundingBox.z0, 0,1,0,1,0,0,0),

	};
	static std::vector<uint32_t> iBuffer = 
	{
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,
		
		8, 9, 10,
		9, 10, 11,

		12, 13, 14,
		13, 14, 15,

		16, 17, 18,
		17, 18, 19,

		20, 21, 22,
		21, 22, 23

	};

	getMaterial(material, D3D12_FILL_MODE_SOLID);// D3D12_FILL_MODE_WIREFRAME);

	int vNumVerticies = static_cast<int>(vBuffer.size());
	int iNumIndexes = static_cast<int>(iBuffer.size());
	int iWholeSize = static_cast<int>(iBuffer.size() * sizeof(uint32_t));

	getGeometry(geometry, &vBuffer[0], vNumVerticies, &iBuffer[0], iWholeSize, iNumIndexes);

	getPosition(position);

	model = Model(material, geometry, position);


	return OK;
}

int Scene::createModelFromFile(string name, Model & model)
{
	static Model::Material material;
	Model::Geometry geometry;
	Model::Position position;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, name.c_str());
	
	if (!ret) return FAIL;

	static std::vector<Vertex> vBuffer;
	static std::vector<uint32_t> iBuffer;
	uint32_t current_index = 0;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = 0, ny = 0, nz = 0;
				if (idx.normal_index != -1)
				{
					nx = attrib.normals[3 * idx.normal_index + 0];
					ny = attrib.normals[3 * idx.normal_index + 1];
					nz = attrib.normals[3 * idx.normal_index + 2];
				}
				float tx = 0, ty = 0;
				if (idx.texcoord_index != -1)
				{
					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}
				vBuffer.push_back({ vx,vy,vz,1.0,nx,ny,nz,1.0,tx,ty });
				iBuffer.push_back(current_index++);
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	// TODO: that should depend on the frustrum properties
	vBuffer = scaleVBuffer(vBuffer, 
		m_boundingBox.x1- m_boundingBox.x0, 
		m_boundingBox.y1 - m_boundingBox.y0, 
		m_boundingBox.z1 - m_boundingBox.z0);
	getMaterial(material, D3D12_FILL_MODE_SOLID);

	int vNumVerticies = static_cast<int>(vBuffer.size());
	int iNumIndexes = static_cast<int>(iBuffer.size());
	int iWholeSize = static_cast<int>(iBuffer.size() * sizeof(uint32_t));

	getGeometry(geometry, &vBuffer[0], vNumVerticies, &iBuffer[0], iWholeSize, iNumIndexes);

	getPosition(position);

	model = Model(material, geometry, position);

	return OK;
}

std::vector<Vertex> Scene::scaleVBuffer(std::vector<Vertex>& vBuffer, float xMax, float yMax, float zMax)
{
	float wantedMaxes[3] = { xMax,yMax,zMax };
	float localMaxes[] = { -1, -1, -1 };

	for each (auto& vertex in vBuffer)
	{
		float x = vertex.position.x;
		float y = vertex.position.y;
		float z = vertex.position.z;
		if (abs(x) > localMaxes[0]) localMaxes[0] = abs(x);
		if (abs(y) > localMaxes[1]) localMaxes[1] = abs(y);
		if (abs(z) > localMaxes[2]) localMaxes[2] = abs(z);
	}
	int biggestAxis = -1;
	float realMax = -1;
	for (int i = 0; i < 3; i++)
	{
		if (localMaxes[i] > realMax)
		{
			realMax = localMaxes[i];
			biggestAxis = i;
		}
	}

	bool isOutOfTheBox = false;

	for (int i = 0; i < 3; i++)
	{
		if (wantedMaxes[i] < realMax)
		{
			isOutOfTheBox = true;
			break;
		}
	}
	// nothing to scale
	if (!isOutOfTheBox) return vBuffer;

	float divider = wantedMaxes[biggestAxis] / localMaxes[biggestAxis];
	std::vector<Vertex> out;
	for each (auto& vertex in vBuffer)
	{
		out.push_back({
			vertex.position.x * divider,
			vertex.position.y * divider,
			vertex.position.z * divider,
			1.0,
			vertex.normal.x,
			vertex.normal.y,
			vertex.normal.z,
			1.0,
			vertex.texCoord.x,
			vertex.texCoord.y
		});
	}
	return out;
}

int Scene::getGeometry(Model::Geometry & geometry, Vertex* vArray, int vNumVerticies, uint32_t* iArray, int iSize,
	int numIndicies)
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	// cast to SubmitVertexBufferAndGetView from m_callBacks ???

	RenderingManager& renderingManager = g_ApplicationPtr->GetRenderingManager();
	if (renderingManager.SubmitVertexBufferAndGetView(vArray, vNumVerticies, vertexBufferView))
	{
		return FAIL;
	}

	if (renderingManager.SubmitIndexBufferAndGetView(iArray, iSize, indexBufferView, numIndicies))
	{
		return FAIL;
	}

	geometry.m_indexBufferView = indexBufferView;
	geometry.m_vertexBufferView = vertexBufferView;
	geometry.m_numIndices = numIndicies;

	return OK;
}

int Scene::getMaterial(Model::Material & material, D3D12_FILL_MODE in_rastMode)
{
	HRESULT hr;
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineStateObject;

	ID3D12Device* device = g_ApplicationPtr->GetRenderingManager().getDevice();

	CD3DX12_ROOT_PARAMETER rootParameters[1];
	rootParameters[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters,
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
	D3D12_INPUT_LAYOUT_DESC	inputLayoutDesc = {};
	D3D12_SHADER_BYTECODE	vertexShaderBytecode = {};
	D3D12_SHADER_BYTECODE	pixelShaderBytecode = {};
	if (getShaderEnv(inputLayoutDesc, vertexShaderBytecode, pixelShaderBytecode) != OK)
	{
		return FAIL;
	}
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	CD3DX12_RASTERIZER_DESC rastDesc(D3D12_DEFAULT);
	rastDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	rastDesc.FillMode = in_rastMode;//D3D12_FILL_MODE_WIREFRAME;

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
		return OK;
	}
	else
	{
		return FAIL;
	}
}

int Scene::getPosition(Model::Position & position)
{
	return OK;
}

int Scene::getShaderEnv(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, D3D12_SHADER_BYTECODE& vertexShaderBytecode, D3D12_SHADER_BYTECODE& pixelShaderBytecode)
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
		return FAIL;
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
		return FAIL;
	}

	// fill out shader bytecode structure for pixel shader
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	// create input layout

	// The input layout is used by the Input Assembler so that it knows
	// how to read the vertex data bound to it.

	static D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// fill out an input layout description structure

	// we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
	inputLayoutDesc.NumElements = _countof(inputLayout);
	inputLayoutDesc.pInputElementDescs = inputLayout;
	return OK;
}

int Scene::ClearAll()
{
	return OK;
}