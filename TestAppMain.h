#pragma once

#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"D3dcompiler.lib") // for shader compile
#pragma comment(lib,"Dxgi.lib") // for CreateDXGIFactory1

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"

// Affiche le contenu Direct3D à l'écran.
namespace TestApp
{
	class TestAppMain
	{
	public:
		TestAppMain();
		void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		// TODO: remplacez par vos propres convertisseurs de contenu.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;

		// Minuteur de boucle de rendu.
		DX::StepTimer m_timer;
	};
}