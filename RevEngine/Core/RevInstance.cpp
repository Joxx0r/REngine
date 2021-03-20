#include "stdafx.h"
#include "RevInstance.h"

#include "RevEngineRetrievalFunctions.h"
#include "RevModel.h"

void RevInstance::Initialize(RevModelInitializationData modelInitializationData, DirectX::XMMATRIX transform)
{
   m_modelHandle = RevModelManager::FindModelHandleFromType(modelInitializationData);
   m_transform = transform;
}

void RevInstance::DrawInstance()
{
   RevModelManager::FindModelFromHandle(m_modelHandle)->DrawRasterized();
}
