﻿#pragma once
#include "RevInstanceManager.h"

class RevEngineMain;
class RevInstanceManager;

class RevScene
{
public:

    void Initialize();

    void DrawScene();
    
    RevInstanceManager* m_instanceManager;
};
