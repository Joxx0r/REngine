#include "stdafx.h"
#include "RevD3DTypes.h"

#include "../BottomLevelASGenerator.h"
#include "../DXRHelper.h"
#include "../DXSampleHelper.h"
#include "../Core/RevEngineRetrievalFunctions.h"

RevVertIndexData RevVertIndexData::Create(const RevVertInitializationData& data)
{
    RevVertIndexData returnData = {};
	ID3D12Device5* device = RevEngineRetrievalFunctions::GetDevice();
    if(data.m_generatedData.m_vertexes.size() > 0)
    {
        const UINT vertexBufferSize = sizeof(Vertex) * static_cast<UINT>(data.m_generatedData.m_vertexes.size());

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
            IID_PPV_ARGS(&returnData.m_d3dData.m_vertexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(
            0, 0); // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(returnData.m_d3dData.m_vertexBuffer->Map(
            0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, data.m_generatedData.m_vertexes.data(), vertexBufferSize);
        returnData. m_d3dData.m_vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        returnData.m_d3dData.m_vertexBufferView.BufferLocation = returnData.m_d3dData.m_vertexBuffer->GetGPUVirtualAddress();
        returnData.m_d3dData.m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        returnData.m_d3dData.m_vertexBufferView.SizeInBytes = vertexBufferSize;         
    }
    if(data.m_generatedData.m_indices.size() > 0)
    {
        //----------------------------------------------------------------------------------------------
        // Indices
        const UINT indexBufferSize = static_cast<UINT>(data.m_generatedData.m_indices.size()) * sizeof(UINT);
        CD3DX12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferResource = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
        ThrowIfFailed(device->CreateCommittedResource(
            &heapProperty, D3D12_HEAP_FLAG_NONE, &bufferResource, //
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&returnData.m_d3dData.m_indexBuffer)));

        // Copy the triangle data to the index buffer.
        UINT8* pIndexDataBegin;
        CD3DX12_RANGE readRange(
        0, 0); // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(returnData.m_d3dData.m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin)));
        memcpy(pIndexDataBegin, data.m_generatedData.m_indices.data(), indexBufferSize);
        returnData.m_d3dData.m_indexBuffer->Unmap(0, nullptr);

        // Initialize the index buffer view.
        returnData.m_d3dData.m_indexBufferView.BufferLocation = returnData.m_d3dData.m_indexBuffer->GetGPUVirtualAddress();
        returnData.m_d3dData.m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
        returnData.m_d3dData.m_indexBufferView.SizeInBytes = indexBufferSize;
    }
    returnData.m_modelData = data.m_generatedData;
    return returnData;
}
AccelerationStructureBuffers RevVertIndexData::CreateAccelerationStructure(const RevVertIndexData& inData)
{
	ID3D12Device5* device = RevEngineRetrievalFunctions::GetDevice();
	ID3D12GraphicsCommandList4* list = RevEngineRetrievalFunctions::GetCommandList();
    nv_helpers_dx12::BottomLevelASGenerator bottomLevelAS;
	// Adding all vertex buffers and not transforming their position.
	if(inData.m_d3dData.m_vertexBuffer.Get() != nullptr)
	{
		if(inData.m_d3dData.m_indexBuffer.Get() != nullptr)
		{
			int numVertex = inData.m_modelData.m_vertexes.size();
			int numIndicies = inData.m_modelData.m_indices.size();
			bottomLevelAS.AddVertexBuffer(inData.m_d3dData.m_vertexBuffer.Get(), 0,
                               numVertex, sizeof(Vertex),
                               inData.m_d3dData.m_indexBuffer.Get(), 0,
                               numIndicies, nullptr, 0, true);	
		}
		else
		{
			int numVertex = inData.m_modelData.m_vertexes.size();
			bottomLevelAS.AddVertexBuffer(inData.m_d3dData.m_vertexBuffer.Get(), 0,
                                          numVertex, sizeof(Vertex), 0,
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
