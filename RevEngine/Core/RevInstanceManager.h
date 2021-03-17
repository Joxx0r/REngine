#pragma once

#include <DirectXMath.h>

#include "RevEngineManager.h"
#include "RevInstance.h"

class RevModelManager;

class RevInstanceManager : public RevEngineManager
{
public:
    RevInstanceManager() {};

    void Initialize(RevModelManager* modelManager);
    void AddInstance(int type, Microsoft::WRL::ComPtr<ID3D12Resource> resource, DirectX::XMMATRIX transform);

    void DrawInstances(ID3D12GraphicsCommandList4* list);
    
    RevModelManager* m_modelManager = nullptr;

    std::vector<RevInstance*> m_instances;
    
};

