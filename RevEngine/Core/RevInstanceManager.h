#pragma once

#include <DirectXMath.h>

#include "RevEngineManager.h"
#include "RevInstance.h"

enum RevEModelType : UINT8;

namespace nv_helpers_dx12 {
class TopLevelASGenerator;
}

class RevModelManager;

class RevInstanceManager : public RevEngineManager
{
public:
    RevInstanceManager() {};

    void AddInstance(RevEModelType desiredType, DirectX::XMMATRIX transform);

    void AddAllInstancesToSBT(  nv_helpers_dx12::TopLevelASGenerator* generator);
    
    void DrawInstances();
    

    std::vector<RevInstance*> m_instances;
    
};

