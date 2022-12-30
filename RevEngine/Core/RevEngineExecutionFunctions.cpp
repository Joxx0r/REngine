#include "stdafx.h"
#include "RevEngineExecutionFunctions.h"
#include "RevEngineRetrievalFunctions.h"
#include "../RevEngineMain.h"

void RevEngineExecutionFunctions::FlushCommandQueue()
{
    RevEngineRetrievalFunctions::GetMain()->FlushCommandQueue();
}
