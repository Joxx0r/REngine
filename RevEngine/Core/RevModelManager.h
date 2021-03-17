#pragma once

#include "RevEngineManager.h"

class RevModel;

class RevModelManager : public RevEngineManager
{
public:
    RevModelManager() {};
    RevModel* FindModel(int type, bool loadIfNotFound = true);
    RevModel* FindModelFromHandle(int handle);
    int FindModelHandleFromType(int type, bool loadIfNotFound = true);

    std::vector<RevModel*> m_models;

    int m_modelCounter = 0;
};

