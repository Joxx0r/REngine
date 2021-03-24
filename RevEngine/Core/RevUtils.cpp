#include "stdafx.h"
#include "RevUtils.h"
#include "d3dcompiler.h"
#include "RevShadermanager.h"
#include "..\d3dx12.h"
#include <array>
#include "RevCoreDefines.h"
#include "RevEngineRetrievalFunctions.h"
#include "../DXSampleHelper.h"
#include "../D3D//RevD3DTypes.h"

struct ID3D12Resource* RevUtils::CreateDefaultBuffer(
		ID3D12Device5* device, 
		ID3D12GraphicsCommandList* cmdList, 
		const void* initData, 
		UINT64 byteSize, 
		ID3D12Resource* uploadBuffer)
{
	ID3D12Resource* defaultBuffer;
	// Create the actual default buffer resource.
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&defaultBuffer)));

	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap. 
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)));


	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(cmdList, defaultBuffer, uploadBuffer, 0, 0, 1, &subResourceData);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.

	return nullptr;
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> RevGetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };

}

void RevUtils::CreateModelRootDescription(RevModelD3DData& outData)
{
	auto staticSamplers = RevGetStaticSamplers();
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(
        0, nullptr, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// #DXR Extra: Perspective Camera
	// The root signature describes which data is accessed by the shader. The camera matrices are held
	// in a constant buffer, itself referenced the heap. To do this we reference a range in the heap,
	// and use that range as the sole parameter of the shader. The camera buffer is associated in the
	// index 0, making it accessible in the shader in the b0 register.
	CD3DX12_ROOT_PARAMETER constantParameter;
	CD3DX12_DESCRIPTOR_RANGE range;
	range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	constantParameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_ALL);

	rootSignatureDesc.Init(1, &constantParameter, 0, nullptr,
                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	ThrowIfFailed(D3D12SerializeRootSignature(
        &rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ThrowIfFailed(RevEngineRetrievalFunctions::GetDevice()->CreateRootSignature(
        0, signature->GetBufferPointer(), signature->GetBufferSize(),
        IID_PPV_ARGS(&outData.m_rootSignature)));
	

}

void RevUtils::CreatePSO(
	RevPSOInitializationData& initializationData)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { initializationData.m_inputLayoutData, initializationData.m_nInputLayout };
	psoDesc.pRootSignature = initializationData.m_rootSignature;
	if (initializationData.m_shader->m_vertexShader.Get())
	{
		psoDesc.VS =
		{
			reinterpret_cast<BYTE*>(initializationData.m_shader->m_vertexShader.Get()->GetBufferPointer()),
			initializationData.m_shader->m_vertexShader.Get()->GetBufferSize()
		};
	}
	if (initializationData.m_shader->m_pixelShader.Get())
	{
		psoDesc.PS =
		{
			reinterpret_cast<BYTE*>(initializationData.m_shader->m_pixelShader->GetBufferPointer()),
			initializationData.m_shader->m_pixelShader->GetBufferSize()
		};
	}
	psoDesc.BlendState = initializationData.m_blendDesc ? CD3DX12_BLEND_DESC(*initializationData.m_blendDesc) : CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState = initializationData.m_rasterizerDesc ? CD3DX12_RASTERIZER_DESC(*initializationData.m_rasterizerDesc) : CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	if (!initializationData.m_useDepth)
	{
		psoDesc.DepthStencilState.DepthEnable = initializationData.m_useDepth;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
	else
	{
		psoDesc.DepthStencilState = initializationData.m_depthStencilDesc ? CD3DX12_DEPTH_STENCIL_DESC(*initializationData.m_depthStencilDesc) : CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.DSVFormat = REV_DEPTH_STENCIL_FORMAT;
	}

	psoDesc.DepthStencilState.StencilEnable = initializationData.m_useStencil;

	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = initializationData.m_numRenderTargets;

	for (UINT index = 0; index < initializationData.m_numRenderTargets; index++)
	{
		//todo johlander will not work for deferred once we make stuff look nicer (normal formats). & HDR
		psoDesc.RTVFormats[index] = initializationData.m_rtvFormats ? initializationData.m_rtvFormats[index] : REV_BACK_BUFFER_FORMAT;
	}

	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	ThrowIfFailed(RevEngineRetrievalFunctions::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(initializationData.m_pso)));

}
