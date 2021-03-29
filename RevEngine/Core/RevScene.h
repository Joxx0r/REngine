#pragma once
#include "RevInstanceManager.h"

struct RevDrawData;
class RevCamera;
class RevEngineMain;
class RevInstanceManager;

class RevScene
{
public:

    void Initialize();

    void DrawScene(const RevDrawData& data);
    
    RevInstanceManager* m_instanceManager;
};
