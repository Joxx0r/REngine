#include "stdafx.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModelManager.h"
#include "RevScene.h"
#include "../RevEngineMain.h"

#define RETURN_ASSERT_NULL(p)  \
if(p == nullptr) \
    assert(false); \
return p;

RevModelManager* RevEngineRetrievalFunctions::GetModelManager()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_modelManager);
}
RevEngineMain* RevEngineRetrievalFunctions::GetMain()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get());
}

RevScene* RevEngineRetrievalFunctions::GetScene()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_scene);
}

RevInstanceManager* RevEngineRetrievalFunctions::GetInstanceManager()
{
    RETURN_ASSERT_NULL(GetScene()->m_instanceManager);
}

RevShaderManager* RevEngineRetrievalFunctions::GetShaderManager()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_shaderManager);
}

ID3D12Device5* RevEngineRetrievalFunctions::GetDevice()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_device.Get());
}

ID3D12GraphicsCommandList4* RevEngineRetrievalFunctions::GetCommandList()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_commandList.Get());
}
ID3D12CommandAllocator* RevEngineRetrievalFunctions::GetCommandAllocator()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_commandAllocator.Get());
}
ID3D12CommandQueue* RevEngineRetrievalFunctions::GetCommandQueue()
{
    RETURN_ASSERT_NULL(RevEngineMain::Get()->m_commandQueue.Get());
}
