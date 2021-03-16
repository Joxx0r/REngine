#pragma once

class RevModel;

class RevModelManager
{
public:
    RevModelManager() {};
    bool LoadModel(ID3D12Device5* device, int type, RevModel& outType);

    std::vector<RevModel> m_models;
};

