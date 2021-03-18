#pragma once

struct IDxcBlob;

struct RevShaderBase 
{
    std::wstring m_path;
};

struct RevShaderLibrary : public RevShaderBase
{
    IDxcBlob* m_blob;
};

struct RevShaderRasterizer : public RevShaderBase
{
    ComPtr<ID3DBlob> m_vertexShader;
    ComPtr<ID3DBlob> m_pixelShader;
};