#include "stdafx.h"
#include "RevModel.h"
#include "RevCoreDefines.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModelConstructionFunctions.h"
#include "RevModelTypes.h"
#include "../BottomLevelASGenerator.h"
#include "../d3dx12.h"

void RevModel::Initialize(const RevModelData& modelData, REV_ID_HANDLE handle)
{
	m_modelData = modelData;
	m_handle = handle;
	m_d3dData = RevModelD3DData::Create(m_modelData);
}

void RevModel::DrawRasterized() const
{
	ID3D12GraphicsCommandList4* list = RevEngineRetrievalFunctions::GetCommandList();
	bool drawIndexed = false;
	if(m_d3dData.m_vertexBuffer.Get())
	{
		list->IASetVertexBuffers(0, 1, &m_d3dData.m_vertexBufferView);
	}
	if(m_d3dData.m_indexBuffer.Get())
	{
		drawIndexed = true;
		list->IASetIndexBuffer(&m_d3dData.m_indexBufferView);
	}

	if(drawIndexed)
	{
		/*if(m_d3dData.m_descriptorHeap.Get())
		{
			const int size = RevEngineRetrievalFunctions::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			ID3D12DescriptorHeap* descriptorHeaps[] = { m_d3dData.m_descriptorHeap.Get() };
			list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
			CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_d3dData.m_descriptorHeap->GetGPUDescriptorHandleForHeapStart());
			tex.Offset(0, size);
			list->SetGraphicsRootDescriptorTable(0, tex);
		}
*/
		list->DrawIndexedInstanced(m_d3dData.m_indexCount, 1, 0, 0, 0);
	}
	else
	{
		list->DrawInstanced(m_d3dData.m_vertexCount, 1, 0, 0);
	}
}

AccelerationStructureBuffers RevModel::CreateStructureBuffer()
{
	m_relevantBuffers = RevModelD3DData::CreateAccelerationStructure(m_d3dData);
	return m_relevantBuffers;
}
