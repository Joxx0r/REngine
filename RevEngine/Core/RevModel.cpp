#include "stdafx.h"
#include "RevModel.h"
#include "RevModelTypes.h"
#include "../BottomLevelASGenerator.h"

void RevModel::Initialize(int type, ID3D12Device5* device)
{
    if(type == 0)
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
}
void RevModel::DrawRasterized(ID3D12GraphicsCommandList4* list) const
{
	list->IASetVertexBuffers(0, 1, &m_baseData.m_d3dData.m_vertexBufferView);
	list->IASetIndexBuffer(&m_baseData.m_d3dData.m_indexBufferView);	
}

AccelerationStructureBuffers RevModel::CreateStructureBuffer(ID3D12Device5* device, ID3D12GraphicsCommandList4* list)
{
	return RevVertIndexData::CreateAccelerationStructure(m_baseData, device, list);
}
