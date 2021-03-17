#pragma once

class RevEngineManager 
{
public:
    virtual void InitializeGraphics(ID3D12Device5* device, ID3D12GraphicsCommandList4* list);

    virtual ~RevEngineManager() {};

    ID3D12Device5* m_device = nullptr;
    ID3D12GraphicsCommandList4* m_list = nullptr;
};


