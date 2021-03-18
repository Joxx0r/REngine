#include "stdafx.h"
#include "RevModel.h"
#include "RevCoreDefines.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModelConstructionFunctions.h"
#include "RevModelTypes.h"
#include "../BottomLevelASGenerator.h"

void RevModel::Initialize(RevEModelType type, REV_ID_HANDLE handle)
{
	m_type = type;
	m_handle = handle;
    if(type == RevEModelType::Triangle)
    {
    	RevVertInitializationData initData = {};
        initData.m_generatedData = RevModelConstructionFunctions::CreateTriangleData();
    	m_baseData = RevVertIndexData::Create(initData);
    }
    else
    if(type == RevEModelType::Plane)
    {
    	RevVertInitializationData initData = {};
    	initData.m_generatedData = RevModelConstructionFunctions::CreatePlaneData();
    	m_baseData = RevVertIndexData::Create(initData);
    }
	else
	{
		assert(false);
	}
}
void RevModel::DrawRasterized() const
{
	ID3D12GraphicsCommandList4* list = RevEngineRetrievalFunctions::GetCommandList();
	bool drawIndexed = false;
	if(m_baseData.m_d3dData.m_vertexBuffer.Get())
	{
		list->IASetVertexBuffers(0, 1, &m_baseData.m_d3dData.m_vertexBufferView);
	}
	if(m_baseData.m_d3dData.m_indexBuffer.Get())
	{
		drawIndexed = true;
		list->IASetIndexBuffer(&m_baseData.m_d3dData.m_indexBufferView);
	}

	const int indexCount = m_baseData.m_modelData.m_indices.size();
	const int vertexCount = m_baseData.m_modelData.m_vertexes.size();
	if(drawIndexed)
	{
		list->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
	}
	else
	{
		list->DrawInstanced(vertexCount, 1, 0, 0);
	}
}

AccelerationStructureBuffers RevModel::CreateStructureBuffer()
{
	m_relevantBuffers =  RevVertIndexData::CreateAccelerationStructure(m_baseData);
	return m_relevantBuffers;
}
