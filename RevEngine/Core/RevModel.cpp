#include "stdafx.h"
#include "RevModel.h"
#include "RevCoreDefines.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModelConstructionFunctions.h"
#include "RevModelTypes.h"
#include "../BottomLevelASGenerator.h"
#include "../d3dx12.h"
#include "../Misc/RevTypes.h"

void RevModel::Initialize(const RevModelData& modelData, REV_ID_HANDLE handle)
{
	m_modelData = modelData;
	m_handle = handle;
	m_d3dData = RevModelD3DData::Create(m_modelData);
}

void RevModel::DrawRasterized(const RevDrawData& data) const
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

	ID3D12GraphicsCommandList* commandList = RevEngineRetrievalFunctions::GetCommandList();
	commandList->SetPipelineState(m_d3dData.m_pso);
	commandList->SetGraphicsRootSignature(m_d3dData.m_rootSignature.Get());
	commandList->SetGraphicsRootConstantBufferView(
    0, data.m_cameraCB.Get()->GetGPUVirtualAddress());
	
	if(m_d3dData.m_descriptorHeap.Get())
	{
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_d3dData.m_descriptorHeap.Get() };
		commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	}
	if(drawIndexed)
	{
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
