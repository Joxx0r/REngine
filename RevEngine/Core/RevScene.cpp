#include "stdafx.h"
#include "RevScene.h"
#include "RevModelTypes.h"
#include "../RevEngineMain.h"

void RevScene::Initialize()
{
    m_instanceManager = new RevInstanceManager();
    //m_instanceManager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(.6f, 0, 0));
    //m_instanceManager->AddInstance(RevEModelType::Triangle, XMMatrixTranslation(-.6f, 0, 0));
   // m_instanceManager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(0, 0, 0));
    m_instanceManager->AddInstance(std::wstring(L"Data//Models//CleaningBot//cleaningBot.dae"), XMMatrixTranslation(0, -1, 3));
    m_instanceManager->AddInstance(RevEModelType::Plane, XMMatrixTranslation(0, 0, 0));
    //m_instanceManager->AddInstance(RevEModelType::Plane, XMMatrixTranslation(0, 0, -10));
  //  m_instanceManager->AddInstance(std::wstring(L"Data//Models//CleaningBot//cleaningBot.dae"), XMMatrixTranslation(0, -1, 3));
}

void RevScene::DrawScene()
{
    m_instanceManager->DrawInstances();   
}
