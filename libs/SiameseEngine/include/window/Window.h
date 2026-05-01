#pragma once
#include <dxgi1_5.h>
#include <nvrhi/d3d12.h>

#include "memory/SiameseAllocator.h"

class GLFWwindow;
namespace sengine
{
	class InputManager;
	class Clock;
	class DeviceManager;
	struct WindowConfig
	{
		std::string title{ "Window" };
		int width{ 1280 }, height{ 720 };
		bool startMaximized = false;
		bool startFullscreen = false;
		bool allowModeSwitch = true;
		nvrhi::Format swapChainFormat = nvrhi::Format::SRGBA8_UNORM;
		uint32_t maxFramesInFlight = 2;
		int windowPosX = -1;	// -1 means use default placement
		int windowPosY = -1;
		uint32_t backBufferWidth = 1280;
		uint32_t backBufferHeight = 720;
		uint32_t refreshRate = 0;
		uint32_t swapChainBufferCount = 3;
		DXGI_USAGE swapChainUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		uint32_t swapChainSampleCount = 1;
		uint32_t swapChainSampleQuality = 0;
		bool vsyncEnabled = false;
		bool tearingEnabled = false;
	};

	class Window
	{
	public:
		~Window();

		void SetUpGlfwInputCallbacks(std::shared_ptr<sengine::InputManager> inputManagerPtr, std::shared_ptr<sengine::Clock>clockPtr);
		bool ShouldClose() const;
		bool CreateSwapChain(SSharedPtr<DeviceManager> deviceManager);

		void PollEvents() const;

		WindowConfig m_config{};
	private:
		GLFWwindow* m_window{ nullptr };
		//dx12 stuff
		DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_fullscreenDesc{};
		HWND m_hWnd{ nullptr };
		uint64_t m_frameCount{ 1 };
		nvrhi::RefCountPtr<IDXGISwapChain3> m_swapChain;
		std::vector<nvrhi::RefCountPtr<ID3D12Resource>> m_swapChainBuffers;	//actual pixel data buffer
		std::vector<nvrhi::TextureHandle> m_rhiSwapChainBuffers;			//the texture view for us to decide how to intepret the data, and in turn edit it
		//fences and events for swap chain to swap
		nvrhi::RefCountPtr<ID3D12Fence> m_frameFence;
		std::vector<HANDLE> m_frameFenceEvents;

	public:
		static Window* Create(const WindowConfig& config);
		static void InitGlfw();
		static void TerminateGlfw();
		static void GlfwErrorCallback(int _error, const char* description);
	};
}