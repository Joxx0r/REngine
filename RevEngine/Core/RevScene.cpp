#include "stdafx.h"
#include "RevScene.h"
#include "RevModelTypes.h"
#include "../RevEngineMain.h"

void RevScene::Initialize(RevEngineMain* manager)
{
    m_manager = new RevInstanceManager();
    m_manager->InitializeGraphics(manager->m_device.Get(), manager->m_commandList.Get());
    m_manager->Initialize(manager->m_modelManager);
    m_manager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(.6f, 0, 0));
    m_manager->AddInstance(RevEModelType::Triangle, XMMatrixTranslation(-.6f, 0, 0));
    m_manager->AddInstance(RevEModelType::Triangle,  XMMatrixTranslation(0, 0, 0));
    m_manager->AddInstance(RevEModelType::Plane, XMMatrixTranslation(0, 0, 0));
}

void RevScene::DrawScene()
{
    m_manager->DrawInstances();   
}
