//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include <dxcapi.h>
#include "TopLevelASGenerator.h"
#include "ShaderBindingTableGenerator.h"
#include "Core/RevCamera.h"
#include "Misc/RevTypes.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

#define id3d12resource ID3D12Resource
#define id3d12rootsignature ID3D12RootSignature
#define id3d12stateobjectproperties ID3D12StateObjectProperties
#define id3d12stateobject ID3D12StateObject
#define idxcblob IDxcBlob
#define id3d12descriptorheap ID3D12DescriptorHeap


struct AccelerationStructureBuffers
{
    ComPtr<ID3D12Resource> pScratch;      // Scratch memory for AS builder
    ComPtr<ID3D12Resource> pResult;       // Where the AS is
    ComPtr<ID3D12Resource> pInstanceDesc; // Hold the matrices of the instances
};

class RevEngineMain
{
public:
    RevEngineMain(UINT width, UINT height, std::wstring name);

    virtual void OnInit();
    virtual void OnUpdate(float delta);
    virtual void OnRender();
    virtual void OnDestroy();

    static const UINT FrameCount = 2;

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device5> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList4> m_commandList;
    UINT m_rtvDescriptorSize;
    bool m_raster = false;

    // App resources.
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;

    ComPtr<id3d12resource> m_bottomLevelAS; // Storage for the bottom Level AS

    nv_helpers_dx12::TopLevelASGenerator m_topLevelASGenerator;
    AccelerationStructureBuffers m_topLevelASBuffers;
    std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>> m_instances;

    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList() const;
    void WaitForPreviousFrame();
    void CheckRaytracingSupport() const;
    void OnKeyUp(UINT8 key);
    void OnButtonDown(UINT32 lParam);
    virtual void OnKeyDown(UINT8 /*key*/)   {}
    void OnMouseMove(UINT8 wParam, UINT32 lParam);
    void UpdateInput(float delta);
    
    /// Create the acceleration structure of an instance
    ///
    /// \param     vVertexBuffers : pair of buffer and vertex count
    /// \return    AccelerationStructureBuffers for TLAS
    AccelerationStructureBuffers
    CreateBottomLevelAS(std::vector<std::pair<ComPtr<id3d12resource>, uint32_t>> vVertexBuffers) const;

    /// Create the main acceleration structure that holds
    /// all instances of the scene
    /// \param     instances : pair of BLAS and transform
    void CreateTopLevelAS(
        const std::vector<std::pair<ComPtr<id3d12resource>, DirectX::XMMATRIX>>& instances);

    /// Create all acceleration structures, bottom and top
    void CreateAccelerationStructures();

    // #DXR
    ComPtr<id3d12rootsignature> CreateRayGenSignature() const;
    ComPtr<id3d12rootsignature> CreateMissSignature() const;
    ComPtr<id3d12rootsignature> CreateHitSignature() const;

    void CreateRaytracingPipeline();

    ComPtr<idxcblob> m_rayGenLibrary;
    ComPtr<idxcblob> m_hitLibrary;
    ComPtr<idxcblob> m_missLibrary;

    ComPtr<id3d12rootsignature> m_rayGenSignature;
    ComPtr<id3d12rootsignature> m_hitSignature;
    ComPtr<id3d12rootsignature> m_missSignature;

    // Ray tracing pipeline state
    ComPtr<id3d12stateobject> m_rtStateObject;
    // Ray tracing pipeline state properties, retaining the shader identifiers
    // to use in the Shader Binding Table
    ComPtr<id3d12stateobjectproperties> m_rtStateObjectProps;

    void CreateRaytracingOutputBuffer();
    void CreateShaderResourceHeap();
    // #DXR Extra: Per-Instance Data
    ComPtr<id3d12resource> m_outputResource;
    ComPtr<id3d12descriptorheap> m_srvUavHeap;

    // #DXR
    void CreateShaderBindingTable();
    nv_helpers_dx12::ShaderBindingTableGenerator m_sbtHelper;
    ComPtr<id3d12resource> m_sbtStorage;

    // #DXR Extra: Per-Instance Data
    ComPtr<id3d12resource> m_planeBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_planeBufferView;
    void CreatePlaneVB();

    // #DXR Extra: Perspective Camera
    void CreateCameraBuffer();
    void UpdateCameraBuffer();
    ComPtr< ID3D12Resource > m_cameraBuffer;
    ComPtr< ID3D12DescriptorHeap > m_constHeap;
    uint32_t m_cameraBufferSize = 0;

    RevInputState m_input = {};
    RevCamera m_camera = {};
    
    void CreatePerInstanceConstantBuffers();
    std::vector<ComPtr<id3d12resource>> m_perInstanceConstantBuffers;

    // #DXR Extra: Per-Instance Data
    void CreateGlobalConstantBuffer();
    ComPtr<id3d12resource> m_globalConstantBuffer;

    UINT GetWidth() const           { return m_width; }
    UINT GetHeight() const          { return m_height; }
    const WCHAR* GetTitle() const   { return m_title.c_str(); }

    void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);

protected:
    std::wstring GetAssetFullPath(LPCWSTR assetName);

    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);

    void SetCustomWindowText(LPCWSTR text);
    
    // Viewport dimensions.
    UINT m_width;
    UINT m_height;
    float m_aspectRatio;

    // Adapter info.
    bool m_useWarpDevice;

private:
    // Root assets path.
    std::wstring m_assetsPath;

    // Window title.
    std::wstring m_title;
    
};

