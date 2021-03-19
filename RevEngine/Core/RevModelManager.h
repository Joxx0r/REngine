#pragma once

#include "RevCoreDefines.h"
#include "RevEngineManager.h"
#include "RevModelTypes.h"

enum RevEModelType : UINT8;
class RevModel;

class RevModelManager : public RevEngineManager
{
public:
    RevModelManager() {};

    /** Finds a model (or tries loading it if desired). */
    static RevModel* FindModel(RevModelRetrievalData desiredType, bool loadIfNotFound = true);
    static RevModel* FindModelFromHandle(REV_ID_HANDLE handle);
    static REV_ID_HANDLE FindModelHandleFromType(RevModelRetrievalData desiredType, bool loadIfNotFound = true);

    /** Generates all SBT models. */
    static void GenerateAccelerationBuffersAllModels();

private:

    RevModel* CreateModelInternal(RevModelRetrievalData desiredType);

    RevModel* FindModelTypeInternal(RevEModelType desiredType);
    RevModel* FindModelHandleInternal(REV_ID_HANDLE handle);
    
    std::vector<RevModel*> m_models;
    REV_ID_HANDLE m_modelCounter = 0;
};

