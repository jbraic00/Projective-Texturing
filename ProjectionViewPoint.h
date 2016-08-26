#pragma once

#include "pch.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Input\Input Controller.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Core\Common\StepTimer.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\View Transform.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\Projection Transform.h"
#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Scene\Transforms\Position.h"

namespace vxe
{
	class ProjectionViewPoint 
	{
	public:
		ProjectionViewPoint(_In_ ID3D11Device2* device):
			_alfa{ -0.1f },
			_beta{ DirectX::XM_PI }, //0.0f
			_view{ device },
			_projection{ device },
			_eye{ device }
		{
			DebugPrint(std::string("\t ProjectionViewPoint::Ctor ...\n"));
		}

		void Update(InputController^, DX::StepTimer const&);

		void ProcessInput(InputController^, float);
		DirectX::XMFLOAT3 ProcessKeyboard(InputController^, float);
		void ProcessPointer(InputController^, float);

		void Reset();

		void InitializeView(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR);
		void SetView();
		void SetProjection(DirectX::CXMMATRIX, float, float, float, float);

		void UpdateEye(_In_ ID3D11DeviceContext2* context) { _eye.Update(context); }
		void UpdateView(_In_ ID3D11DeviceContext2* context) { _view.Update(context); }
		void UpdateProjection(_In_ ID3D11DeviceContext2* context) { _projection.Update(context); }

		void BindView(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage shader = ProgrammableStage::VertexShaderStage,
			unsigned slot = 0,
			unsigned count = 1)
		{
			_view.GetConstantBuffer()->Bind(context, shader, slot, count);
		}

		void BindProjection(_In_ ID3D11DeviceContext2* context,
			ProgrammableStage shader = ProgrammableStage::VertexShaderStage,
			unsigned slot = 0,
			unsigned count = 1)
		{
			_projection.GetConstantBuffer()->Bind(context, shader, slot, count);
		}

	private:
		// Rotations about x and y axis 
		float _alfa;
		float _beta;
		DirectX::XMMATRIX _rotation;
		DirectX::XMFLOAT2 _lastpointerposition;
		DirectX::XMFLOAT2 _currentpointerposition;

		static const DirectX::XMFLOAT3 _movevelocity;
		static const DirectX::XMFLOAT2 _rotationvelocity;
		static const float _debounce;

		ViewTransform _view;
		ProjectionTransform _projection;

		Position _eye;
		DirectX::XMFLOAT3 _lookat;
		DirectX::XMFLOAT3 _up;
	};
}
