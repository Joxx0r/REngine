#include "stdafx.h"
#include "RevModelManager.h"
#include "RevCoreDefines.h"
#include "RevEngineRetrievalFunctions.h"
#include "RevModel.h"

RevModelManager* GetModelManagerInternal()
{
    return RevEngineRetrievalFunctions::GetModelManager();
}

RevModel* RevModelManager::FindModel(RevEModelType desiredType, bool loadIfNotFound /*= true*/)
{
    RevModelManager* modelManager = GetModelManagerInternal();
    if(!modelManager)
    {
        return nullptr;
    }
    if(RevModel* model = modelManager->FindModelTypeInternal(desiredType))
    {
        return model;
    }
    if(loadIfNotFound)
    {
        return modelManager->CreateModelInternal(desiredType);
    }
    return nullptr;
}
RevModel* RevModelManager::FindModelFromHandle( REV_ID_HANDLE handle)
{
    RevModelManager* modelManager = GetModelManagerInternal();
    if(!modelManager)
    {
        return nullptr;
    }
    return modelManager->FindModelHandleInternal(handle);
}

REV_ID_HANDLE RevModelManager::FindModelHandleFromType( RevEModelType desiredType, bool loadIfNotFound)
{
    RevModelManager* modelManager = GetModelManagerInternal();
    if(!modelManager)
    {
        return REV_ID_NONE;
    }
    
    if(RevModel* model = modelManager->FindModelTypeInternal(desiredType))
    {
        return model->m_handle;
    }

    if(loadIfNotFound)
    {
        return modelManager->CreateModelInternal(desiredType)->m_handle;
    }
    return REV_ID_NONE;
}

RevModel* RevModelManager::CreateModelInternal(RevEModelType desiredType)
{
    RevModel* model = new RevModel();
    m_modelCounter++;
    model->Initialize(desiredType,m_modelCounter);
    m_models.push_back(model);
    return model;
}

void RevModelManager::GenerateAccelerationBuffersAllModels()
{
    RevModelManager* modelManager = GetModelManagerInternal();
    if(!modelManager)
    {
        return;
    }
    for(int index = 0; index < modelManager->m_models.size(); index++)
    {
        assert(modelManager->m_models[index]);
        modelManager->m_models[index]->CreateStructureBuffer();
    }
}

RevModel* RevModelManager::FindModelTypeInternal(RevEModelType desiredType)
{
    for(int index = 0; index < m_models.size(); index++)
    {
        assert(m_models[index]);
        if(m_models[index]->m_type == desiredType)
        {
            return m_models[index];
        }
    }
    return nullptr;
}
RevModel* RevModelManager::FindModelHandleInternal(REV_ID_HANDLE handle)
{
    for(int index = 0; index < m_models.size(); index++)
    {
        assert(m_models[index]);
        if(m_models[index]->m_handle == handle)
        {
            return m_models[index];
        }
    }
    return nullptr;
}
