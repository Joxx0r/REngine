#pragma once
#include <map>
#include <string>

#include "RevShaderTypes.h"

class RevShaderManager
{
public:
    void Initialize();

    static RevShaderLibrary* GetShaderLibrary(const std::wstring& path, bool loadIfNotExisting = true );
    static RevShaderRasterizer* GetShaderRasterizer(const std::wstring& path, bool loadIfNotExisting = true );

private:

    RevShaderLibrary* AddShaderLibrary(std::wstring path);
    RevShaderRasterizer* AddRasterizerShader(std::wstring path);
    
    static RevShaderLibrary* LoadShaderLibrary(std::wstring path);
    static RevShaderRasterizer* LoadRasterizerShader(std::wstring shaderPath);

    std::map<std::wstring, RevShaderRasterizer*> m_pathToShaderRaster;
    std::map<std::wstring, RevShaderLibrary*> m_pathToShaderLibrary;
    
};

