#include "stdafx.h"
#include "RevModel.h"

#include "RevCoreDefines.h"
#include "RevModelTypes.h"
#include "../BottomLevelASGenerator.h"

void RevModel::Initialize(RevEModelType type, REV_ID_HANDLE handle, ID3D12Device5* device)
{
	m_type = type;
	m_handle = handle;
    if(type == RevEModelType::Triangle)
    {
    	RevVertInitializationData initData = {};
    	initData.m_device = device;
        {
            // Define the geometry for a triangle.
            initData.m_generatedData.m_vertexes = {
                {{sqrtf(8.f / 9.f), 0.f, -1.f / 3.f}, {1.f, 0.f, 0.f, 1.f}},
{{-sqrtf(2.f / 9.f), sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 1.f, 0.f, 1.f}},
{{-sqrtf(2.f / 9.f), -sqrtf(2.f / 3.f), -1.f / 3.f}, {0.f, 0.f, 1.f, 1.f}},
{{0.f, 0.f, 1.f}, {1, 0, 1, 1}}};
        }
        {
            initData.m_generatedData.m_indices = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};
        }
    	m_baseData = RevVertIndexData::Create(initData);
    }
    else
    if(type == RevEModelType::Plane)
    {
    	RevVertInitializationData initData = {};
    	initData.m_device = device;
	    {
		    // Define the geometry for a plane.
    		initData.m_generatedData.m_vertexes = {
    			{{-1.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 0
                {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
                {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
                {{01.5f, -.8f, 01.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 2
                {{-1.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 1
                {{01.5f, -.8f, -1.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}  // 4
    		};
	    }
    	m_baseData = RevVertIndexData::Create(initData);
    }
	else
	{
		assert(false);
	}
}
void RevModel::DrawRasterized(ID3D12GraphicsCommandList4* list) const
{
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

AccelerationStructureBuffers RevModel::CreateStructureBuffer(ID3D12Device5* device, ID3D12GraphicsCommandList4* list)
{
	m_relevantBuffers =  RevVertIndexData::CreateAccelerationStructure(m_baseData, device, list);
	return m_relevantBuffers;
}
