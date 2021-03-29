#include "stdafx.h"
#include "RevScene.h"
#include "RevModelTypes.h"
#include "../RevEngineMain.h"

void RevScene::Initialize()
{
    m_instanceManager = new RevInstanceManager();
    m_instanceManager->AddInstance(std::wstring(L"Data//Models//CleaningBot//cleaningBot.dae"), XMMatrixTranslation(0, -1, 3));
}

void RevScene::DrawScene(const RevDrawData& data)
{
    m_instanceManager->DrawInstances(data);   
}
