#pragma once

struct IDxcBlob;
struct RevShaderBase
{
    ComPtr<ID3DBlob> m_vertexShader;
    ComPtr<ID3DBlob> m_pixelShader;
};

struct RevShaderLibrary
{
    IDxcBlob* m_blob;
};