#pragma once

#include "pch.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Utilities.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Models\Mesh Base.h"

namespace vxe
{
	template <typename T, typename U> class Wall : public MeshBase<T, U> {};

	template <>
	class Wall<DirectX::VertexPositionNormalTexture, unsigned short> : public MeshBase<DirectX::VertexPositionNormalTexture, unsigned short>
	{
	public:
		Wall() {}

		//vertices are hardcoded
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device) override
		{
			DebugPrint(std::string("\t Wall<VertexPositionNormalTexture, unsigned short>::CreateAsync() ...\n"));

			//Right-handed
			std::vector<DirectX::VertexPositionNormalTexture> vertices = {
				/*DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, 9.0f, -10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(10.0f, 9.0f, -10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(10.0f, -1.0f, -10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, -1.0f, -10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },*/
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, 9.0f, 10.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, 9.0f, -10.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, -1.0f, -10.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
				DirectX::VertexPositionNormalTexture{ DirectX::XMFLOAT3(-10.0f, -1.0f, 10.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
			};

			std::vector<unsigned short> indices = {
				0, 1, 2,
				0, 2, 3
			};

			return MeshBase::CreateAsync(device, vertices, indices);
		}

		//loading from a file
		virtual concurrency::task<void> LoadAsync(_In_ ID3D11Device2* device, const std::wstring&) override
		{
			DebugPrint(std::string("\t Wall<VertexPositionNormalTexture, unsigned short>::CreateAsync() ...\n"));

			std::vector<DirectX::VertexPositionNormalTexture> vertices;
			std::vector<unsigned short> indices;

			//Loading from a file

			return MeshBase::CreateAsync(device, vertices, indices);
		}

		//creating from memory
		virtual concurrency::task<void> CreateAsync(_In_ ID3D11Device2* device, const std::vector<char>&)
		{
			DebugPrint(std::string("\t Wall<VertexPositionNormalTexture, unsigned>::CreateAsync() ...\n"));

			std::vector<DirectX::VertexPositionNormalTexture> vertices;
			std::vector<unsigned short> indices;

			//Extract (parse) vertices from memory

			return MeshBase::CreateAsync(device, vertices, indices);
		}
	};
}