#include "stdafx.h"
#include "RevInstanceManager.h"
#include "RevModel.h"
#include "../TopLevelASGenerator.h"

void RevInstanceManager::Initialize(RevModelManager* modelManager)
{
    m_modelManager = modelManager;
}

void RevInstanceManager::AddInstance(RevEModelType desiredType,  DirectX::XMMATRIX transform)
{
    RevInstance* newInstance = new RevInstance();
    newInstance->Initialize(m_modelManager, desiredType, transform);
    m_instances.push_back(newInstance);
}

void RevInstanceManager::AddAllInstancesToSBT(nv_helpers_dx12::TopLevelASGenerator* generator)
{
    assert(generator);
    for (size_t i = 0; i < m_instances.size(); i++)
    {
        RevInstance* instance = m_instances[i];
        ID3D12Resource* resource = m_modelManager->FindModelFromHandle(instance->m_modelHandle)->m_relevantBuffers.pResult.Get();
        generator->AddInstance(resource, instance->m_transform,
                                          static_cast<UINT>(i), static_cast<UINT>(i));
    }
}

void RevInstanceManager::DrawInstances()
{
    for(RevInstance* instance : m_instances)
    {
        assert(instance);
        instance->DrawInstance(m_list);
    }
}
        