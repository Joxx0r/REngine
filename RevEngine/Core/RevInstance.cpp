#include "stdafx.h"
#include "RevInstance.h"

#include "RevEngineRetrievalFunctions.h"
#include "RevModel.h"

void RevInstance::Initialize(RevEModelType desiredType, DirectX::XMMATRIX transform)
{
   m_modelHandle = RevModelManager::FindModelHandleFromType(desiredType);
   m_transform = transform;
}

void RevInstance::DrawInstance()
{
   RevModelManager::FindModelFromHandle(m_modelHandle)->DrawRasterized();
}
