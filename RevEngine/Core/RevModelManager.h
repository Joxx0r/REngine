#pragma once

class RevModel;

class RevModelManager
{
public:
    RevModelManager() {};
    RevModel* FindModel(ID3D12Device5* device, int type, bool loadIfNotFound = true);
    RevModel* FindModelFromHandle(ID3D12Device5* device, int handle);
    int FindModelHandleFromType(ID3D12Device5* device, int type, bool loadIfNotFound = true);

    std::vector<RevModel*> m_models;

    int m_modelCounter = 0;
};

