#include "stdafx.h"
#include "RevInstance.h"
#include "RevModel.h"

void RevInstance::Initialize(RevModelManager* manager,int modelType, DirectX::XMMATRIX transform,   Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
   m_resource = resource;
   m_modelManager = manager;
   m_modelHandle = m_modelManager->FindModelHandleFromType(modelType);
   m_transform = transform;
}

void RevInstance::DrawInstance(ID3D12GraphicsCommandList4* list)
{
   m_modelManager->FindModelFromHandle(m_modelHandle)->DrawRasterized(list);
}
