#pragma once
#include "../D3D/RevD3DTypes.h"

class RevModelConstructionFunctions
{
public:
    
    static RevModelData CreateModelDataType(RevEModelType type);
    static RevModelData CreateTriangleData();
    static RevModelData CreatePlaneData();
    
};

