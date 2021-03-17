#pragma once
#include <map>
#include <string>

#include "RevShaderTypes.h"

class RevShaderManager
{
public:
    void Initialize();

    std::map<std::wstring, RevShaderBase> m_pathToShader;
    std::map<std::wstring, RevShaderLibrary> m_pathToShaderLibrary;

    IDxcBlob* GetBlob(const std::wstring& path, bool loadIfNotExisting = true );

private:

    IDxcBlob* LoadShaderLibrary(std::wstring path);
};

