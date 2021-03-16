#include "stdafx.h"
#include "RevModelManager.h"
#include "RevModel.h"

bool RevModelManager::LoadModel(ID3D12Device5* device, int type, RevModel& outType)
{
    for(int index = 0; index < m_models.size(); index++)
    {
        if(m_models[index].m_type == type)
        {
            outType = m_models[index];
            return true;
        }
    }
    outType.Initialize(type, device);
    m_models.push_back(outType);
    return true;
}
