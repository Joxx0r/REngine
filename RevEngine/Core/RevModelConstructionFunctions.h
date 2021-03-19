#pragma once
#include "../D3D/RevD3DTypes.h"

class RevModelConstructionFunctions
{
public:
    
    static RevModelData CreateModelDataType(RevModelRetrievalData data);
    static RevModelData CreateTriangleData();
    static RevModelData CreatePlaneData();
    
};

