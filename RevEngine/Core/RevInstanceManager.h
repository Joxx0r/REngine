#pragma once

#include <DirectXMath.h>

#include "RevEngineManager.h"
#include "RevInstance.h"

struct RevDrawData;
class RevCamera;
enum RevEModelType : UINT8;

namespace nv_helpers_dx12 {
class TopLevelASGenerator;
}

class RevModelManager;

class RevInstanceManager : public RevEngineManager
{
public:
    RevInstanceManager() {};

    static void AddInstance(RevModelInitializationData data, DirectX::XMMATRIX transform);
    static void AddAllInstancesToSBT(  nv_helpers_dx12::TopLevelASGenerator* generator);
    
    void DrawInstances(const RevDrawData& data);

private:
    
    std::vector<RevInstance*> m_instances;
    
};

