#include "stdafx.h"
#include "RevD3DTypes.h"

#include "../BottomLevelASGenerator.h"
#include "../DXRHelper.h"
#include "../DXSampleHelper.h"
#include "../Core/RevEngineRetrievalFunctions.h"
#include "../Microsoft/RevDDSTextureLoader.h"

void LoadTexture(const std::wstring& path, struct ID3D12Resource** resourceToEndUpAt)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
	std::unique_ptr<uint8_t[]> ddsData;
	ThrowIfFailed(LoadDDSTextureFromFile(
        RevEngineRetrievalFunctions::GetDevice(),
        path.c_str(),
        resourceToEndUpAt,
        ddsData,
        subResources));
}

RevModelD3DData RevModelD3DData::Create(const RevModelData& data)
{
    RevModelD3DData returnData = {};
	ID3D12Device5* device = RevEngineRetrievalFunctions::GetDevice();
	returnData.m_vertexCount = data.GetNumVertexes();
	returnData.m_vertexStride = data.GetVertexStride();
    if(returnData.m_vertexCount > 0)
    {
        const UINT vertexBufferSize =  data.GetModelVertexSize();
        // Note: using upload heaps to transfer static data like vert buffers is not
        // recommended. Every time the GPU needs it, the upload heap will be
        // marshalled over. Please read up on Default Heap usage. An upload heap is
        // used here for code simplicity and because there are very few verts to
        // actually transfer.
        ThrowIfFailed(device->CreateCommittedResource(
            // ReSharper disable once CppMsExtAddressOfClassRValue
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
            // ReSharper disable once CppMsExtAddressOfClassRValue
            &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&returnData.m_vertexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(
            0, 0); // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(returnData.m_vertexBuffer->Map(
            0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, data.GetData(), vertexBufferSize);
        returnData.m_vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        returnData.m_vertexBufferView.BufferLocation = returnData.m_vertexBuffer->GetGPUVirtualAddress();
        returnData.m_vertexBufferView.StrideInBytes = data.GetVertexStride();
        returnData.m_vertexBufferView.SizeInBytes = vertexBufferSize;         
    }
    if(data.m_indices.size() > 0)
    {
        //----------------------------------------------------------------------------------------------
        // Indices
    	
    	returnData.m_indexCount = data.m_indices.size();
        const UINT indexBufferSize = static_cast<UINT>(returnData.m_indexCount) * sizeof(UINT);
        CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferResource = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
        ThrowIfFailed(device->CreateCommittedResource(
            &heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&returnData.m_indexBuffer)));

        // Copy the triangle data to the index buffer.
        UINT8* pIndexDataBegin;
        CD3DX12_RANGE readRange(
        0, 0); // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(returnData.m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin)));
        memcpy(pIndexDataBegin, data.m_indices.data(), indexBufferSize);
        returnData.m_indexBuffer->Unmap(0, nullptr);

        // Initialize the index buffer view.
        returnData.m_indexBufferView.BufferLocation = returnData.m_indexBuffer->GetGPUVirtualAddress();
        returnData.m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
        returnData.m_indexBufferView.SizeInBytes = indexBufferSize;
    }

	if(data.m_textures.size() > 0)
	{
		returnData.m_textures = data.m_textures;

		std::vector<ID3D12Resource*> resoures;
		for (auto& texture : returnData.m_textures)
		{
			LoadTexture(texture.m_path, &texture.m_resource);
			resoures.push_back(texture.m_resource);
		}

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = resoures.size();
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&returnData.m_descriptorHeap)));
	
		CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(returnData.m_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < resoures.size(); i++)
		{
			ID3D12Resource* resource = resoures[i];
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = resource->GetDesc().Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = resource->GetDesc().MipLevels;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			device->CreateShaderResourceView(resource, &srvDesc, hDescriptor);
			hDescriptor.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		}
		
	}
	
    return returnData;
}
AccelerationStructureBuffers RevModelD3DData::CreateAccelerationStructure(const RevModelD3DData& inData)
{
	ID3D12Device5* device = RevEngineRetrievalFunctions::GetDevice();
	ID3D12GraphicsCommandList4* list = RevEngineRetrievalFunctions::GetCommandList();
    nv_helpers_dx12::BottomLevelASGenerator bottomLevelAS;
	// Adding all vertex buffers and not transforming their position.
	if(inData.m_vertexBuffer.Get() != nullptr)
	{
		if(inData.m_indexBuffer.Get() != nullptr)
		{
			int numVertex = inData.m_vertexCount;
			int numIndicies = inData.m_indexCount;
			bottomLevelAS.AddVertexBuffer(inData.m_vertexBuffer.Get(), 0,
                               numVertex, inData.m_vertexStride,
                               inData.m_indexBuffer.Get(), 0,
                               numIndicies, nullptr, 0, true);	
		}
		else
		{
			int numVertex = inData.m_vertexCount;
			bottomLevelAS.AddVertexBuffer(inData.m_vertexBuffer.Get(), 0,
                                          numVertex, inData.m_vertexStride, 0,
                                          0);
		}
	}

	// The AS build requires some scratch space to store temporary information.
	// The amount of scratch memory is dependent on the scene complexity.
	UINT64 scratchSizeInBytes = 0;
	// The final AS also needs to be stored in addition to the existing vertex
	// buffers. It size is also dependent on the scene complexity.
	UINT64 resultSizeInBytes = 0;

	bottomLevelAS.ComputeASBufferSizes(device, false, &scratchSizeInBytes,
	                                   &resultSizeInBytes);

	// Once the sizes are obtained, the application is responsible for allocating
	// the necessary buffers. Since the entire generation will be done on the GPU,
	// we can directly allocate those on the default heap
	AccelerationStructureBuffers buffers;
	buffers.pScratch = nv_helpers_dx12::CreateBuffer(
		device, scratchSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON,
		nv_helpers_dx12::kDefaultHeapProps);
	buffers.pResult = nv_helpers_dx12::CreateBuffer(
		device, resultSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		nv_helpers_dx12::kDefaultHeapProps);

	// Build the acceleration structure. Note that this call integrates a barrier
	// on the generated AS, so that it can be used to compute a top-level AS right
	// after this method.
	bottomLevelAS.Generate(list, buffers.pScratch.Get(),
	                       buffers.pResult.Get(), false, nullptr);

	return buffers;
}
