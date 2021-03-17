#include "stdafx.h"
#include "RevEngineManager.h"

void RevEngineManager::InitializeGraphics(ID3D12Device5* device, ID3D12GraphicsCommandList4* list)
{
    m_device = device;
    m_list = list;
}
