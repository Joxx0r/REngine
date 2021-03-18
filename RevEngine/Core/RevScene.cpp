﻿#include "stdafx.h"
#include "RevScene.h"
#include "RevModelTypes.h"
#include "../RevEngineMain.h"

void RevScene::Initialize()
{
    m_instanceManager = new RevInstanceManager();
    m_instanceManager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(.6f, 0, 0));
    m_instanceManager->AddInstance(RevEModelType::Triangle, XMMatrixTranslation(-.6f, 0, 0));
    m_instanceManager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(0, 0, 0));
    m_instanceManager->AddInstance(RevEModelType::Plane, XMMatrixTranslation(0, 0, 0));
}

void RevScene::DrawScene()
{
    m_instanceManager->DrawInstances();   
}
