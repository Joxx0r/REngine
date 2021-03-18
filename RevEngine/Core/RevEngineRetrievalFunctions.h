#pragma once

class RevModelManager;
class RevScene;
class RevInstanceManager;
class RevShaderManager;

class RevEngineRetrievalFunctions
{
public:
    
    static RevModelManager* GetModelManager();
    static RevScene* GetScene();
    static RevInstanceManager* GetInstanceManager();
    static RevShaderManager* GetShaderManager();

    static ID3D12Device5* GetDevice();
    static ID3D12GraphicsCommandList4* GetCommandList();
    
};

