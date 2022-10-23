#pragma once
#include "dxptr.h"
#include "dxStructures.h"
#include <vector>
#include <string>

namespace mini
{
	class Window;
}

class DxDevice
{
public:
	//DxDevice(DxDevice const&) = delete;
	//void operator=(DxDevice const&) = delete; 
	inline static std::shared_ptr<DxDevice> instance;
	inline static SIZE windowSize;
	
	explicit DxDevice(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain);

	ID3D11DeviceContext* context() const { return m_context; }

	IDXGISwapChain* swapChain() const { return m_swapChain; }

	ID3D11Device* operator->() const { return m_device; }

	mini::dx_ptr<ID3D11RenderTargetView> CreateRenderTargetView(const mini::dx_ptr<ID3D11Texture2D>& texture) const;

	mini::dx_ptr<ID3D11Texture2D> CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const;

	mini::dx_ptr<ID3D11ShaderResourceView> CreateShaderResourceView(const std::wstring& texPath) const;

	mini::dx_ptr<ID3D11ShaderResourceView> CreateShaderResourceView(const mini::dx_ptr<ID3D11Texture2D>& texture, D3D11_SHADER_RESOURCE_VIEW_DESC* description) const;

	mini::dx_ptr<ID3D11DepthStencilView> CreateDepthStencilView(const mini::dx_ptr<ID3D11Texture2D>& texture) const;

	mini::dx_ptr<ID3D11Buffer> CreateBuffer(const void* data, const D3D11_BUFFER_DESC& desc) const;

	static std::vector<BYTE> LoadByteCode(const std::wstring& filename);

	mini::dx_ptr<ID3D11VertexShader> CreateVertexShader(std::vector<BYTE> vsCode) const; 
	
	mini::dx_ptr<ID3D11GeometryShader> CreateGeometryShader(std::vector<BYTE> psCode) const;

	mini::dx_ptr<ID3D11PixelShader> CreatePixelShader(std::vector<BYTE> psCode) const;

	mini::dx_ptr<ID3D11InputLayout> CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC> elements, std::vector<BYTE> vsCode) const;

	mini::dx_ptr < ID3D11DepthStencilView> CreateDepthStencilView(SIZE size) const;

	template<class T> mini::dx_ptr < ID3D11Buffer>
	CreateVertexBuffer(const std::vector<T>& vertices) const {
		size_t e = vertices.size();
		size_t t = sizeof(T);
		size_t s = e * t;
		s = max(s, 1);
		auto desc = BufferDescription::VertexBufferDescription(s);
		return CreateBuffer(s == 0 ? nullptr : reinterpret_cast<const void*>( vertices.data()), desc);

	}

	template<class T> mini::dx_ptr < ID3D11Buffer> CreateIndexBuffer(const std::vector<T>& indices) const
	{
		size_t e = indices.size();
		size_t t = sizeof(T);
		size_t s = e * t;
		auto desc = BufferDescription::IndexBufferDescription(s);
		return CreateBuffer(reinterpret_cast<const void*>(indices.data()), desc);
	}

	template < typename T>
	mini::dx_ptr<ID3D11Buffer> CreateConstantBuffer() const
	{
		BufferDescription desc =
			BufferDescription::ConstantBufferDescription(
				sizeof(T));
		return CreateBuffer(nullptr, desc);
	}


	mini::dx_ptr<ID3D11BlendState> CreateBlendState(const BlendDescription& desc = {}) const;

	mini::dx_ptr<ID3D11SamplerState> CreateSamplerState(const SamplerDescription& desc) const;

private:

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapChain;
};