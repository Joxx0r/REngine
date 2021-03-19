#pragma once

class RevEngineMain;
class RevModelManager;
class RevScene;
class RevInstanceManager;
class RevShaderManager;

class RevEngineRetrievalFunctions
{
public:
    
    static RevModelManager* GetModelManager();
    static RevEngineMain* GetMain();
    static RevScene* GetScene();
    static RevInstanceManager* GetInstanceManager();
    static RevShaderManager* GetShaderManager();

    static ID3D12Device5* GetDevice();
    static ID3D12GraphicsCommandList4* GetCommandList();
    static ID3D12CommandAllocator* GetCommandAllocator();
    static ID3D12CommandQueue* GetCommandQueue();
    
};

