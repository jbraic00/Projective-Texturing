#pragma once
//ili popravit projectionviewpoint ili maknit i ostavit staro bez pomicanja
#include "pch.h"

#include "ProjectionViewPoint.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Third Party\DirectX Tool Kit\VertexTypes.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Models\Mesh Base.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Pipeline Stages\Shaders\Vertex Shader.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Pipeline Stages\Shaders\Pixel Shader.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\World Transforms.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\View Transform.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\Projection Transform.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\Position.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Cameras\FirstPerson Camera.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Input\Input Controller.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Pipeline Resources\Textures\Texture2D.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Lighting\Material.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Lighting\Directional Light.h"

namespace vxe
{
	class Renderer3D : public RendererBase3D 
	{
	public:
		Renderer3D::Renderer3D(InputController^ inputcontroller = nullptr) : RendererBase3D(inputcontroller)
		{
			DebugPrint(std::string("\t Renderer3D::Ctor... \n"));
		}

		virtual void CreateDeviceDependentResources() override;
		virtual void CreateWindowSizeDependentResources() override;
		virtual void ReleaseDeviceDependentResources() override;

		virtual void Update(DX::StepTimer const& timer) override
		{
			RendererBase3D::Update(timer);

			//_projectionviewpoint->Update(_inputcontroller, timer);
			_camera->Update(_inputcontroller, timer);
		}

		virtual void Render() override;

		//void SetLighting();
		void SetTexturing();
		void SetProjectionTexture();

		/*void Rotate(float radians) override
		{
			_world->RotateY(radians);
		}*/

	private:
		void SetCamera()
		{
			auto context = m_deviceResources->GetD3DDeviceContext();

			/*_view->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, 1);

			_projection->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, 2);*/

			//_eye->GetConstantBuffer()->Bind(context, ProgrammableStage::PixelShaderStage, 1);

			_camera->UpdateEye(context);
			_camera->UpdateView(context);
			_camera->BindView(context, ProgrammableStage::VertexShaderStage, 1);
		}

		void SetProjectionViewPoint();

		template <typename T, typename U>
		void Draw(std::shared_ptr<MeshBase<T, U>>& mesh, std::shared_ptr<WorldTransforms>& world, bool indexed = true)
		{
			auto context = m_deviceResources->GetD3DDeviceContext();

			world->Update(context);
			world->GetConstantBuffer()->Bind(context);

			mesh->BindVertexBuffer(context);
			mesh->BindIndexBuffer(context);

			/*if (isCube) { _texture = _cubetexture; }
			else { _texture = _floortexture; }
			SetTexturing();*/

			if (indexed) mesh->DrawIndexed(context);
			else mesh->Draw(context);
		}

		std::shared_ptr<VertexShader<DirectX::VertexPositionNormalTexture>> _vertexshader;
		std::shared_ptr<PixelShader> _pixelshader;

		std::shared_ptr<MeshBase<DirectX::VertexPositionNormalTexture, unsigned short>> _floormodel;
		std::shared_ptr<MeshBase<DirectX::VertexPositionNormalTexture, unsigned short>> _cubemodel;
		std::shared_ptr<MeshBase<DirectX::VertexPositionNormalTexture, unsigned short>> _wallmodel;
		std::shared_ptr<MeshBase<DirectX::VertexPositionNormalTexture, unsigned short>> _secondcubemodel;

		std::shared_ptr<WorldTransforms> _world;
		std::shared_ptr<WorldTransforms> _cubeworld;
		/*std::shared_ptr<ViewTransform> _view;
		std::shared_ptr<ProjectionTransform> _projection;*/
		//std::shared_ptr<Position> _eye;
		std::shared_ptr<FirstPersonCamera> _camera;

		std::shared_ptr<ViewTransform> _projectionviewpointview;
		std::shared_ptr<ProjectionTransform> _projectionviewpointprojection;
		//std::shared_ptr<ProjectionViewPoint> _projectionviewpoint;

		/*std::shared_ptr<Material> _material;
		std::shared_ptr<DirectionalLight> _directional;*/

		std::shared_ptr<Texture2D> _texture;
		std::shared_ptr<Texture2D> _floortexture;
		std::shared_ptr<Texture2D> _cubetexture;
		std::shared_ptr<Texture2D> _projectiontexture;
	};
}