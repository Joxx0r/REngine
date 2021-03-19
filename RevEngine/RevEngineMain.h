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
#include "Core/RevEngineManager.h"
#include "Core/RevModel.h"
#include "Core/RevModelManager.h"
#include "Core/RevShaderManager.h"
#include "Misc/RevTypes.h"

class RevScene;
using namespace DirectX;

using Microsoft::WRL::ComPtr;

class RevInstanceManager;
class RevModelManager;
class RevShaderManager;

#define id3d12resource ID3D12Resource
#define id3d12rootsignature ID3D12RootSignature
#define id3d12stateobjectproperties ID3D12StateObjectProperties
#define id3d12stateobject ID3D12StateObject
#define idxcblob IDxcBlob
#define id3d12descriptorheap ID3D12DescriptorHeap

class RevEngineMain
{
public:
    virtual ~RevEngineMain() = default;
    RevEngineMain(const RevWindowData& data);

    /** Static instance retrivals */
    static RevEngineMain* Construct(const RevWindowData& data);
    static void Destroy();
    static RevEngineMain* Get();

    void LoadPipeline();
    void LoadAssets();
    
    void PopulateCommandList() const;
    void WaitForPreviousFrame();
    void FlushCommandQueue();
    
    void CheckRaytracingSupport() const;
    
    virtual void OnInit();
    virtual void OnUpdate(float delta);
    virtual void OnRender();
    virtual void OnDestroy();
    void OnKeyUp(UINT8 key);
    void OnButtonDown(UINT32 lParam);
    void OnKeyDown(UINT8 /*key*/) {};
    void OnMouseMove(UINT8 wParam, UINT32 lParam);

    void CreateTopLevelAS();
    void CreateAccelerationStructures();
    ComPtr<id3d12rootsignature> CreateRayGenSignature() const;
    ComPtr<id3d12rootsignature> CreateMissSignature() const;
    ComPtr<id3d12rootsignature> CreateHitSignature() const;
    void CreateRaytracingOutputBuffer();
    void CreateShaderResourceHeap();
    void CreateShaderBindingTable();
    void CreateCameraBuffer();
    void CreateDepthBuffer();
    void CreatePerInstanceConstantBuffers();
    void CreateRaytracingPipeline();
    
    UINT GetWidth() const           { return m_windowData.m_width; }
    UINT GetHeight() const          { return m_windowData.m_height; }
    const WCHAR* GetTitle() const   { return m_windowData.m_title.c_str(); }
    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);
    
 
    void SetCustomWindowText(LPCWSTR text);

    // #DXR Extra: Depth Buffering
    // #DXR Extra: Perspective Camera
    void UpdateCameraBuffer();
    void UpdateInput(float delta);
    
    static const UINT FrameCount = 2;
  
    // Synchronization objects.
    UINT64 m_fenceValue;
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;

    ComPtr<id3d12resource> m_bottomLevelAS; // Storage for the bottom Level AS

    nv_helpers_dx12::TopLevelASGenerator m_topLevelASGenerator;
    AccelerationStructureBuffers m_topLevelASBuffers;

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

    ComPtr<id3d12rootsignature> m_rayGenSignature;
    ComPtr<id3d12rootsignature> m_hitSignature;
    ComPtr<id3d12rootsignature> m_missSignature;

    // Ray tracing pipeline state
    ComPtr<id3d12stateobject> m_rtStateObject;
    // Ray tracing pipeline state properties, retaining the shader identifiers
    // to use in the Shader Binding Table
    ComPtr<id3d12stateobjectproperties> m_rtStateObjectProps;

    // #DXR Extra: Per-Instance Data
    ComPtr<id3d12resource> m_outputResource;
    ComPtr<id3d12descriptorheap> m_srvUavHeap;

    nv_helpers_dx12::ShaderBindingTableGenerator m_sbtHelper;
    ComPtr<id3d12resource> m_sbtStorage;

    ComPtr< ID3D12Resource > m_cameraBuffer;
    ComPtr< ID3D12DescriptorHeap > m_constHeap;
    uint32_t m_cameraBufferSize = 0;

    RevInputState m_input = {};
    RevCamera m_camera = {};

    std::vector<ComPtr<id3d12resource>> m_perInstanceConstantBuffers;

    ComPtr< ID3D12DescriptorHeap > m_dsvHeap;
    ComPtr< ID3D12Resource > m_depthStencil;

    RevWindowData m_windowData;

    // Root assets path.
    std::wstring m_assetsPath;

    RevModelManager* m_modelManager;
    RevShaderManager* m_shaderManager;
    std::vector<RevEngineManager*> m_managers;


    RevScene* m_scene = nullptr;
    static RevEngineMain* s_instance;
};

