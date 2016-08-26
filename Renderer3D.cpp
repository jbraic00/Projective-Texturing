#include "pch.h"

#include "Renderer3D.h"

#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Models\Basic Shapes\Cubes.h"
#include "Floor.h"
#include "Wall.h"

using namespace vxe;
using namespace std;
using namespace concurrency;

using namespace DirectX;
using namespace Windows::Foundation;

void Renderer3D::CreateDeviceDependentResources() 
{
	DebugPrint(string("\t Renderer3D::CreateDeviceDependentResources() ... \n"));

	auto device = m_deviceResources->GetD3DDevice();

	vector<task<void>> tasks;

	_vertexshader = make_shared<VertexShader<VertexPositionNormalTexture>>();
	tasks.push_back(_vertexshader->CreateAsync(device, L"VertexShader.cso"));

	_pixelshader = make_shared<PixelShader>();
	tasks.push_back(_pixelshader->CreateAsync(device, L"PixelShader.cso"));

	_floormodel = make_shared<Floor<VertexPositionNormalTexture, unsigned short>>();
	tasks.push_back(_floormodel->CreateAsync(device));

	_cubemodel = make_shared<Cube<VertexPositionNormalTexture, unsigned short>>();
	tasks.push_back(_cubemodel->CreateAsync(device));

	_wallmodel = make_shared<Wall<VertexPositionNormalTexture, unsigned short>>();
	tasks.push_back(_wallmodel->CreateAsync(device));

	_secondcubemodel = make_shared<Cube<VertexPositionNormalTexture, unsigned short>>();
	tasks.push_back(_secondcubemodel->CreateAsync(device));

	_texture = make_shared<Texture2D>(device, DXGI_FORMAT_R8G8B8A8_UNORM);

	_floortexture = make_shared<Texture2D>(device, DXGI_FORMAT_R8G8B8A8_UNORM);
	tasks.push_back(_floortexture->CreateDDSAsync(m_deviceResources->GetD3DDeviceContext(), L"Assets\\stone.dds"));

	_cubetexture = make_shared<Texture2D>(device, DXGI_FORMAT_R8G8B8A8_UNORM);
	tasks.push_back(_cubetexture->CreateDDSAsync(m_deviceResources->GetD3DDeviceContext(), L"Assets\\seafloor.dds"));

	_projectiontexture = make_shared<Texture2D>(device, DXGI_FORMAT_R8G8B8A8_UNORM);
	tasks.push_back(_projectiontexture->CreateDDSAsync(m_deviceResources->GetD3DDeviceContext(), L"Assets\\dx11.dds"));

	when_all(tasks.begin(), tasks.end()).then([this]() {
		m_loadingComplete = true;
		DebugPrint(string("\t -- A lambda: Loading is complete! \n"));
		
		// Should wait for loading
		//SetTexturing();
		SetProjectionTexture();
	});

	_world = make_shared<WorldTransforms>(device);

	_cubeworld = make_shared<WorldTransforms>(device);
	_cubeworld->ScaleRotateYAndTranslate(2.0f, 2.0f, 2.0f, 45.0f, -5.0f, 2.0f, 6.0f);

	//SetLighting();
}

//void Renderer3D::SetLighting() 
//{
//	DebugPrint(string("\t Renderer3D::SetLighting() ...\n"));
//
//	auto device = m_deviceResources->GetD3DDevice();
//	auto context = m_deviceResources->GetD3DDeviceContext();
//
//	XMFLOAT4 ambient = XMFLOAT4(0.9f, 0.1f, 0.5f, 1.0f);
//	XMFLOAT4 diffuse = XMFLOAT4(0.3f, 0.5f, 0.7f, 1.0f);
//	XMFLOAT4 specular = XMFLOAT4(0.7f, 0.3f, 0.1f, 16.0f);
//	XMFLOAT4 reflection = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	_material = make_shared<Material>(device);
//	_material->SetComponents(ambient, diffuse, specular, reflection);
//
//	_material->Update(context);
//	_material->GetConstantBuffer()->Bind(context, ProgrammableStage::PixelShaderStage);
//
//	ambient = XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f);
//	diffuse = XMFLOAT4(0.6f, 0.5f, 0.3f, 1.0f);
//	specular = XMFLOAT4(0.4f, 0.5f, 0.1f, 1.0f);
//	XMFLOAT3 direction = XMFLOAT3(-0.5f, 0.5f, 0.3f);
//
//	_directional = make_shared<DirectionalLight>(device);
//	_directional->SetComponents(ambient, diffuse, specular, direction);
//	
//	_directional->Update(context);
//	_directional->GetConstantBuffer()->Bind(context, ProgrammableStage::PixelShaderStage, 2);
//}

void Renderer3D::SetTexturing() 
{
	DebugPrint(string("\t Renderer3D::SetTexturing() ...\n"));

	auto context = m_deviceResources->GetD3DDeviceContext();

	_texture->CreateSamplerState();
	_texture->BindSamplerState(context);

	_texture->CreateShaderResourceView();
	_texture->BindShaderResourceView(context);
}

void Renderer3D::SetProjectionTexture()
{
	DebugPrint(string("\t Renderer3D::SetProjectionTexture() ...\n"));

	auto context = m_deviceResources->GetD3DDeviceContext();

	_projectiontexture->CreateShaderResourceView();
	_projectiontexture->BindShaderResourceView(context, ProgrammableStage::PixelShaderStage, 1);
}

void Renderer3D::SetProjectionViewPoint() 
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	_projectionviewpointview->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, 3);

	_projectionviewpointprojection->GetConstantBuffer()->Bind(context, ProgrammableStage::VertexShaderStage, 4);

	/*_projectionviewpoint->UpdateEye(context);
	_projectionviewpoint->UpdateView(context);
	_projectionviewpoint->BindView(context, ProgrammableStage::VertexShaderStage, 3);*/
}

void Renderer3D::CreateWindowSizeDependentResources() 
{
	DebugPrint(string("\t Renderer3D::CreateWindowSizeDependentResources() ...\n"));

	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	//_view = make_shared<ViewTransform>(device, Handedness::RightHanded);
	_camera = make_shared<FirstPersonCamera>(device);

	//iz gotovih projekata
	/*static const XMVECTORF32 eye = { 0.0f, 0.0f, 3.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };*/
	
	static const XMVECTORF32 eye = { 10.0f, 10.0f, 15.0f, 0.0f }; //bilo je y = 7.0f, z = -10.0f, x = 0.0f
	static const XMVECTORF32 at = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	/*_view->SetView(eye, at, up);
	_view->Update(context);*/

	_camera->InitializeView(eye, at, up);
	_camera->UpdateView(context);
	_camera->UpdateEye(context);

	/*_eye = make_shared<Position>(device);
	_eye->SetPosition(eye);
	_eye->Update(context);*/

	Size outputSize = m_deviceResources->GetOutputSize();
	DebugPrint(string("\t\t Width:") + to_string(outputSize.Width) + string("\n"));
	DebugPrint(string("\t\t Height:") + to_string(outputSize.Height) + string("\n"));

	float r = outputSize.Width / outputSize.Height;
	float fov = 70.0f * XM_PI / 180.0f;
	float n = 0.01f;
	float f = 1000.0f;

	if (r < 1.0f) { fov *= 2.0f; }

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	/*_projection = make_shared<ProjectionTransform>(device, Handedness::RightHanded);
	_projection->SetProjection(orientationMatrix, fov, r, n, f);

	_projection->Update(context);*/

	_camera->SetProjection(orientationMatrix, fov, r, n, f);
	_camera->UpdateProjection(context);
	_camera->BindProjection(context, ProgrammableStage::VertexShaderStage, 2);

	//for projection texture
	_projectionviewpointview = make_shared<ViewTransform>(device, Handedness::RightHanded);

	//_projectionviewpoint = make_shared<ProjectionViewPoint>(device);

	static const XMVECTORF32 projectionEye = { 2.0f, 5.0f, 4.0f, 0.0f };
	static const XMVECTORF32 projectionAt = { 0.0f, 0.0f, 2.0f, 0.0f };
	/*static const XMVECTORF32 projectionEye = { 4.0f, 0.0f, 0.0f, 0.0f };
	static const XMVECTORF32 projectionAt = { 2.0f, 0.0f, 0.0f, 0.0f };*/

	_projectionviewpointview->SetView(projectionEye, projectionAt, up);
	_projectionviewpointview->Update(context);

	/*_projectionviewpoint->InitializeView(projectionEye, projectionAt, up);
	_projectionviewpoint->UpdateView(context);
	_projectionviewpoint->UpdateEye(context);*/

	fov = XM_PI / 2.0f;
	r = 1.0f;
	n = 0.1f;
	f = 100.0f;

	_projectionviewpointprojection = make_shared<ProjectionTransform>(device, Handedness::RightHanded);
	_projectionviewpointprojection->SetProjection(orientationMatrix, fov, r, n, f);
	_projectionviewpointprojection->Update(context);

	/*_projectionviewpoint->SetProjection(orientationMatrix, fov, r, n, f);
	_projectionviewpoint->UpdateProjection(context);
	_projectionviewpoint->BindProjection(context, ProgrammableStage::VertexShaderStage, 4);*/
}

void Renderer3D::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete) { return; }

	auto context = m_deviceResources->GetD3DDeviceContext();

	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	_vertexshader->Bind(context);
	_pixelshader->Bind(context);

	//SetCamera();

	_texture = _floortexture;
	SetTexturing();
	Draw(_floormodel, _world);
	Draw(_wallmodel, _world);
	_texture = _cubetexture;
	SetTexturing();
	Draw(_cubemodel, _world);
	Draw(_secondcubemodel, _cubeworld);

	SetCamera();
	SetProjectionViewPoint();

	m_deviceResources->SetRasterizerState();
}

void Renderer3D::ReleaseDeviceDependentResources()
{
	DebugPrint(string("\t Renderer3D::ReleaseDeviceDependentResources() ... \n"));

	m_loadingComplete = false;

	_vertexshader->Reset();
	_pixelshader->Reset();

	_floormodel->Reset();
	_cubemodel->Reset();
	_wallmodel->Reset();
	_secondcubemodel->Reset();

	_world->Reset();
	_cubeworld->Reset();
	/*_view->Reset();
	_projection->Reset();*/
	//_eye->Reset();
	_camera->Reset();

	_projectionviewpointview->Reset();
	_projectionviewpointprojection->Reset();
	//_projectionviewpoint->Reset();

	/*_material->Reset();
	_directional->Reset();*/
}