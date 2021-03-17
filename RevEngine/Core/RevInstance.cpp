#include "stdafx.h"
#include "RevInstance.h"

#include "RevEngineRetrievalFunctions.h"
#include "RevModel.h"

void RevInstance::Initialize(RevEModelType desiredType, DirectX::XMMATRIX transform)
{
   m_modelHandle = RevEngineRetrievalFunctions::GetModelManager()->FindModelHandleFromType(desiredType);
   m_transform = transform;
}

void RevInstance::DrawInstance()
{
   RevEngineRetrievalFunctions::GetModelManager()->FindModelFromHandle(m_modelHandle)->DrawRasterized();
}
