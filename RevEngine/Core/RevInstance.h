#pragma once
#include <DirectXMath.h>


#include "RevCoreDefines.h"
#include "RevModelManager.h"

struct RevDrawData;
enum RevEModelType : UINT8;
class RevInstance
{
public:
    RevInstance() {};

    void Initialize(RevModelInitializationData modelInitializationData, DirectX::XMMATRIX transform);
    void DrawInstance(const RevDrawData& data);

    DirectX::XMMATRIX m_transform;

    RevModelManager* m_modelManager = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource = nullptr;
    REV_ID_HANDLE m_modelHandle;
    REV_ID_HANDLE m_instanceHandle;
};

