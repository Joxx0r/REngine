#include "stdafx.h"
#include "RevInstanceManager.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModel.h"
#include "../TopLevelASGenerator.h"

RevInstanceManager* GetInstanceManagerInternal()
{
    return RevEngineRetrievalFunctions::GetInstanceManager();
}

void RevInstanceManager::AddInstance(RevEModelType desiredType,  DirectX::XMMATRIX transform)
{
    RevInstanceManager* instanceManager = GetInstanceManagerInternal();
    if(!instanceManager)
    {
        return;
    }
    RevInstance* newInstance = new RevInstance();
    newInstance->Initialize(desiredType, transform);
    instanceManager->m_instances.push_back(newInstance);
}

void RevInstanceManager::AddAllInstancesToSBT(nv_helpers_dx12::TopLevelASGenerator* generator)
{
    RevInstanceManager* instanceManager = GetInstanceManagerInternal();
    if(!instanceManager)
    {
        return;
    }
    assert(generator);
    RevModelManager* modelManager = RevEngineRetrievalFunctions::GetModelManager();
    for (size_t i = 0; i < instanceManager->m_instances.size(); i++)
    {
        RevInstance* instance = instanceManager->m_instances[i];
        ID3D12Resource* resource = RevModelManager::FindModelFromHandle(instance->m_modelHandle)->m_relevantBuffers.pResult.Get();
        generator->AddInstance(resource, instance->m_transform,
                                          static_cast<UINT>(i), static_cast<UINT>(i));
    }
}

void RevInstanceManager::DrawInstances()
{
    for(RevInstance* instance : m_instances)
    {
        assert(instance);
        instance->DrawInstance();
    }
}        