#pragma once
#include <DirectXMath.h>


#include "RevCoreDefines.h"
#include "RevModelManager.h"

enum RevEModelType : UINT8;
class RevInstance
{
public:
    RevInstance() {};

    void Initialize(RevModelManager* manager,  RevEModelType desiredType, DirectX::XMMATRIX transform);
    void DrawInstance(ID3D12GraphicsCommandList4* list);

    DirectX::XMMATRIX m_transform;

    ID3D12Device5* m_device = nullptr;
    RevModelManager* m_modelManager = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource = nullptr;
    REV_ID_HANDLE m_modelHandle;
    REV_ID_HANDLE m_instanceHandle;
};

