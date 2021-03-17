#include "stdafx.h"
#include "RevShaderManager.h"
#include <dxcapi.h>
#include "../DXRHelper.h"
#include "../DXSampleHelper.h"

void RevShaderManager::Initialize()
{
#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    
    const std::wstring shader_path = L"Shaders/Shaders.hlsl";
    RevShaderBase shader = {};
    ThrowIfFailed(D3DCompileFromFile(shader_path.c_str(),
                                     nullptr, nullptr, "VSMain", "vs_5_0",
                                     compileFlags, 0, &shader.m_vertexShader, nullptr));
    ThrowIfFailed(D3DCompileFromFile(shader_path.c_str(),
                                     nullptr, nullptr, "PSMain", "ps_5_0",
                                     compileFlags, 0, &shader.m_pixelShader, nullptr));
    m_pathToShader.insert_or_assign(shader_path, shader);

    LoadShaderLibrary(L"shaders/RayGen.hlsl");
    LoadShaderLibrary(L"shaders/Miss.hlsl");
    LoadShaderLibrary(L"shaders/Hit.hlsl");
}

IDxcBlob* RevShaderManager::GetBlob(const std::wstring& path, bool loadIfNotExisting /*= true*/)
{
    if(m_pathToShaderLibrary.find(path) == m_pathToShaderLibrary.end())
    {
        if(loadIfNotExisting)
        {
            return LoadShaderLibrary(path);
        }
        return nullptr;
    }
    else
    {
        return m_pathToShaderLibrary[path].m_blob;
    }
}

IDxcBlob* RevShaderManager::LoadShaderLibrary(std::wstring path)
{
    RevShaderLibrary library = {};
    library.m_blob =  nv_helpers_dx12::CompileShaderLibrary(path.c_str());
    if( library.m_blob != nullptr)
    {
        m_pathToShaderLibrary.insert_or_assign(path, library);
    }
    
    return library.m_blob;
}
