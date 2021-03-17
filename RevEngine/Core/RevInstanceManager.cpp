#include "stdafx.h"
#include "RevInstanceManager.h"

void RevInstanceManager::Initialize(RevModelManager* modelManager)
{
    m_modelManager = modelManager;
}

void RevInstanceManager::AddInstance(int type, Microsoft::WRL::ComPtr<ID3D12Resource> resource, DirectX::XMMATRIX transform)
{
    RevInstance* newInstance = new RevInstance();
    newInstance->Initialize(m_modelManager, type, transform, resource);
    m_instances.push_back(newInstance);
}

void RevInstanceManager::DrawInstances(ID3D12GraphicsCommandList4* list)
{
    for(RevInstance* instance : m_instances)
    {
        assert(instance);
        instance->DrawInstance(list);
    }
}
        