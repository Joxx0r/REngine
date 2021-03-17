#pragma once

#include "RevCoreDefines.h"
#include "RevEngineManager.h"

enum RevEModelType : UINT8;
class RevModel;

class RevModelManager : public RevEngineManager
{
public:
    RevModelManager() {};
    RevModel* FindModel(RevEModelType desiredType, bool loadIfNotFound = true);
    RevModel* FindModelFromHandle(int handle);
    REV_ID_HANDLE FindModelHandleFromType(RevEModelType desiredType, bool loadIfNotFound = true);

    void GenerateAccelerationBuffersAllModels();

    std::vector<RevModel*> m_models;

    REV_ID_HANDLE m_modelCounter = 0;
};

