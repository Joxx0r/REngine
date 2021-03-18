#include "stdafx.h"
#include "RevShaderManager.h"
#include <dxcapi.h>
#include "RevEngineRetrievalFunctions.h"
#include "../DXRHelper.h"
#include "../DXSampleHelper.h"

RevShaderManager* GetShaderManagerInternal()
{
    return RevEngineRetrievalFunctions::GetShaderManager();
}

void RevShaderManager::Initialize()
{
    AddRasterizerShader( L"Data//Shaders//Shaders.hlsl");
    AddShaderLibrary(L"Data//Shaders//RayGen.hlsl");
    AddShaderLibrary(L"Data//Shaders//Miss.hlsl");
    AddShaderLibrary(L"Data//Shaders//Hit.hlsl");
}

RevShaderLibrary* RevShaderManager::GetShaderLibrary(const std::wstring& path, bool loadIfNotExisting /*= true*/)
{
    RevShaderManager* shaderManager = GetShaderManagerInternal();
    if(!shaderManager)
    {
        return nullptr;
    }

    RevShaderLibrary* library = nullptr;
    if(shaderManager->m_pathToShaderLibrary.find(path) == shaderManager->m_pathToShaderLibrary.end())
    {
        if(loadIfNotExisting)
        {
           return shaderManager->AddShaderLibrary(path);
        }
        return nullptr;
    }
    else
    {
        library = shaderManager->m_pathToShaderLibrary[path];
    }
    assert(library != nullptr);
    return library;
}
RevShaderRasterizer* RevShaderManager::GetShaderRasterizer(const std::wstring& path, bool loadIfNotExisting)
{
    RevShaderManager* shaderManager = GetShaderManagerInternal();
    if(!shaderManager)
    {
        return nullptr;
    }
    RevShaderRasterizer* rasterizer = nullptr;
    if(shaderManager->m_pathToShaderRaster.find(path) == shaderManager->m_pathToShaderRaster.end())
    {
        if(loadIfNotExisting)
        {
            rasterizer = shaderManager->AddRasterizerShader(path);
        }
    }
    else
    {
        rasterizer = shaderManager->m_pathToShaderRaster[path];
    }
    assert(rasterizer != nullptr);
    return rasterizer;
}

RevShaderLibrary* RevShaderManager::AddShaderLibrary(std::wstring path)
{
    if(m_pathToShaderLibrary.find(path) == m_pathToShaderLibrary.end())
    {
        RevShaderLibrary* library = LoadShaderLibrary(path);
        if(library)
        {
            m_pathToShaderLibrary.insert_or_assign(path, library);
            return library;
        }
        return nullptr;
    }
    else
    {
        return m_pathToShaderLibrary[path];
    }
}

RevShaderRasterizer* RevShaderManager::AddRasterizerShader(std::wstring path)
{
    RevShaderRasterizer* returnValue = nullptr;
    if(m_pathToShaderRaster.find(path) == m_pathToShaderRaster.end())
    {
       RevShaderRasterizer* rasterShader = LoadRasterizerShader(path);
       m_pathToShaderRaster.insert_or_assign(path, rasterShader);
       returnValue = m_pathToShaderRaster[path];
    }
    else
    {
       returnValue = m_pathToShaderRaster[path];
    }
    return returnValue;
}

RevShaderLibrary* RevShaderManager::LoadShaderLibrary(std::wstring path)
{
    RevShaderLibrary* library = new RevShaderLibrary();
    library->m_path = path;
    library->m_blob = nv_helpers_dx12::CompileShaderLibrary(path.c_str());
    return library;
}

RevShaderRasterizer* RevShaderManager::LoadRasterizerShader( std::wstring shaderPath)
{
    RevShaderRasterizer* rasterizer = new RevShaderRasterizer();
#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(),
                                     nullptr, nullptr, "VSMain", "vs_5_0",
                                     compileFlags, 0, &rasterizer->m_vertexShader, nullptr));
    ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(),
                                     nullptr, nullptr, "PSMain", "ps_5_0",
                                     compileFlags, 0, &rasterizer->m_pixelShader, nullptr));
    rasterizer->m_path = shaderPath;
    return rasterizer;
}