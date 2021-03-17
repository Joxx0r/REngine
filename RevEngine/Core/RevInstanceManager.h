#pragma once

#include <DirectXMath.h>

#include "RevEngineManager.h"
#include "RevInstance.h"

namespace nv_helpers_dx12 {
class TopLevelASGenerator;
}

class RevModelManager;

class RevInstanceManager : public RevEngineManager
{
public:
    RevInstanceManager() {};

    void Initialize(RevModelManager* modelManager);
    void AddInstance(int type, DirectX::XMMATRIX transform);

    void AddAllInstancesToSBT(  nv_helpers_dx12::TopLevelASGenerator* generator);
    
    void DrawInstances(ID3D12GraphicsCommandList4* list);
    
    RevModelManager* m_modelManager = nullptr;

    std::vector<RevInstance*> m_instances;
    
};

