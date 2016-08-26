#include "pch.h"

#include "ProjectionViewPoint.h"

using namespace Windows::System;
using namespace vxe;
using namespace DirectX;

const DirectX::XMFLOAT3 ProjectionViewPoint::_movevelocity = { 20.0f, 10.f, 10.0f };
const DirectX::XMFLOAT2 ProjectionViewPoint::_rotationvelocity = { 0.5f, 0.5f };
const float ProjectionViewPoint::_debounce = 2.0f;

void ProjectionViewPoint::InitializeView(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR lookat, DirectX::FXMVECTOR up)
{
	DebugPrint(std::string("\t ProjectionViewPoint::InitializeView() \n"));

	_eye.SetPosition(eye);
	DirectX::XMStoreFloat3(&_lookat, lookat);
	DirectX::XMStoreFloat3(&_up, up);

	_view.SetView(eye, lookat, up);
}

void ProjectionViewPoint::SetView()
{
	auto eye = _eye.GetPosition();
	auto lookat = DirectX::XMLoadFloat3(&_lookat);
	auto up = DirectX::XMLoadFloat3(&_up);

	_view.SetView(eye, lookat, up);
}

void ProjectionViewPoint::SetProjection(DirectX::CXMMATRIX orientation, float a, float b, float zn, float zf)
{
	_projection.SetProjection(orientation, a, b, zn, zf);
}

void ProjectionViewPoint::Update(InputController^ inputcontroller, DX::StepTimer const& timer)
{
	float time = (float)timer.GetElapsedSeconds();

	auto rotationx = XMMatrixRotationX(_alfa);
	auto rotationy = XMMatrixRotationY(_beta);

	_rotation = XMMatrixMultiply(rotationy, rotationx);

	ProcessInput(inputcontroller, time);

	SetView();
}

void ProjectionViewPoint::ProcessInput(InputController^ inputcontroller, float time)
{
	auto movement = ProcessKeyboard(inputcontroller, time);

	ProcessPointer(inputcontroller, time);

	auto d = XMVector3Transform(XMLoadFloat3(&movement), _rotation);
	_eye.ChangePosition(XMVectorGetX(d), XMVectorGetY(d), XMVectorGetZ(d));

	auto forward = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	auto eye = _eye.GetValue();
	auto lookat = XMLoadFloat4(&eye) + XMVector3Transform(forward, _rotation);

	_lookat.x = XMVectorGetX(lookat);
	_lookat.y = XMVectorGetY(lookat);
	_lookat.z = XMVectorGetZ(lookat);
}

XMFLOAT3 ProjectionViewPoint::ProcessKeyboard(InputController^ inputcontroller, float time)
{
	auto movement = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if (inputcontroller->IsKeyDown(VirtualKey::Left))
		movement.x += time * _movevelocity.x;

	if (inputcontroller->IsKeyDown(VirtualKey::Right))
		movement.x -= time * _movevelocity.x;

	if (inputcontroller->IsKeyDown(VirtualKey::Q))
		movement.y += time * _movevelocity.y;

	if (inputcontroller->IsKeyDown(VirtualKey::E))
		movement.y -= time * _movevelocity.y;

	if (inputcontroller->IsKeyDown(VirtualKey::Up))
		movement.z += time * _movevelocity.z;

	if (inputcontroller->IsKeyDown(VirtualKey::Down))
		movement.z -= time * _movevelocity.z;

	return movement;
}

void ProjectionViewPoint::ProcessPointer(InputController^ inputcontroller, float time)
{
	if (!inputcontroller->IsLeftButtonPressed()) return;

	_lastpointerposition = _currentpointerposition;
	_currentpointerposition = inputcontroller->GetPointerPosition();

	if (_currentpointerposition.y - _lastpointerposition.y > _debounce)
		_alfa += _rotationvelocity.x * time;

	else if (_currentpointerposition.y - _lastpointerposition.y < -_debounce)
		_alfa -= _rotationvelocity.x * time;

	else if (_currentpointerposition.x - _lastpointerposition.x > _debounce)
		_beta += _rotationvelocity.y * time;

	else if (_currentpointerposition.x - _lastpointerposition.x < -_debounce)
		_beta -= _rotationvelocity.y * time;

	if (_alfa > 1.4f) _alfa = 1.4f;
	else if (_alfa < -1.4f) _alfa = -1.4f;

	if (_beta > XM_PI) _beta -= 2 * XM_PI;
	else if (_beta < -XM_PI) _beta += 2 * XM_PI;
}

void ProjectionViewPoint::Reset()
{
	DebugPrint(std::string("\t\t ProjectionViewPoint::Reset() ...\n"));

	_view.Reset();
	_projection.Reset();
	_eye.Reset();
}