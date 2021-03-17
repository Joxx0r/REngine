#include "stdafx.h"
#include "RevInstance.h"
#include "RevModel.h"

void RevInstance::Initialize(RevModelManager* manager,RevEModelType desiredType, DirectX::XMMATRIX transform)
{
   m_modelManager = manager;
   m_modelHandle = m_modelManager->FindModelHandleFromType(desiredType);
   m_transform = transform;
}

void RevInstance::DrawInstance(ID3D12GraphicsCommandList4* list)
{
   m_modelManager->FindModelFromHandle(m_modelHandle)->DrawRasterized(list);
}
