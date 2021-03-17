#pragma once

class RevEngineManager 
{
public:
    virtual void InitializeGraphics(ID3D12Device5* device);

    virtual ~RevEngineManager() {};

    ID3D12Device5* m_device = nullptr;
};


