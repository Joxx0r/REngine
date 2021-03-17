#include "stdafx.h"
#include "RevInstance.h"
#include "RevModel.h"

void RevInstance::Initialize(RevModelManager* manager, ID3D12Device5* device, int modelType, DirectX::XMMATRIX transform,   Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
   m_resource = resource;
   m_device = device;
   m_modelManager = manager;
   m_modelHandle = m_modelManager->FindModelHandleFromType(device, modelType);
   m_transform = transform;
}

void RevInstance::DrawInstance(ID3D12GraphicsCommandList4* list)
{
   m_modelManager->FindModelFromHandle(m_device, m_modelHandle)->DrawRasterized(list);
}
