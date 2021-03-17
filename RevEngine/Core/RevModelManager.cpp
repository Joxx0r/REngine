#include "stdafx.h"
#include "RevModelManager.h"

#include "RevCoreDefines.h"
#include "RevModel.h"

RevModel* RevModelManager::FindModel(int type, bool loadIfNotFound /*= true*/)
{
    for(int index = 0; index < m_models.size(); index++)
    {
        assert(m_models[index]);
        if(m_models[index]->m_type == type)
        {
            return m_models[index];
        }
    }

    if(loadIfNotFound)
    {
        RevModel* model = new RevModel();
        m_modelCounter++;
        model->Initialize(type, m_modelCounter, m_device);
        m_models.push_back(model);
        return model;
    }
    
    return nullptr;
}
RevModel* RevModelManager::FindModelFromHandle( int handle)
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

REV_ID_HANDLE RevModelManager::FindModelHandleFromType( int type, bool loadIfNotFound)
{
    for(int index = 0; index < m_models.size(); index++)
    {
        assert(m_models[index]);
        if(m_models[index]->m_type == type)
        {
            return m_models[index]->m_handle;
        }
    }

    if(loadIfNotFound)
    {
        RevModel* model = new RevModel();
        m_modelCounter++;
        model->Initialize(type,m_modelCounter, m_device);
        m_models.push_back(model);
        return model->m_handle;
    }

    return -1;
    
}
void RevModelManager::GenerateAccelerationBuffersAllModels()
{
    for(int index = 0; index < m_models.size(); index++)
    {
        assert(m_models[index]);
        m_models[index]->CreateStructureBuffer(m_device, m_list);
    }
}
