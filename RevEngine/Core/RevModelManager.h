#pragma once

#include "RevCoreDefines.h"
#include "RevEngineManager.h"

class RevModel;

class RevModelManager : public RevEngineManager
{
public:
    RevModelManager() {};
    RevModel* FindModel(int type, bool loadIfNotFound = true);
    RevModel* FindModelFromHandle(int handle);
    REV_ID_HANDLE FindModelHandleFromType(int type, bool loadIfNotFound = true);

    void GenerateAccelerationBuffersAllModels();

    std::vector<RevModel*> m_models;

    REV_ID_HANDLE m_modelCounter = 0;
};

