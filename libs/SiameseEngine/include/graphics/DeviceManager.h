#pragma once

#include <d3d12.h>
#include <Windows.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>

#include <nvrhi/d3d12.h>
#include <nvrhi/validation.h>

namespace sengine
{
	struct AdapterInfo
	{
		std::string name;
		uint32_t vendorID = 0;
		uint32_t deviceID = 0;
		uint64_t dedicatedVideoMemory = 0;
		nvrhi::RefCountPtr<IDXGIAdapter> dxgiAdapter;
	};
	struct DefaultMessageCallback : public nvrhi::IMessageCallback
	{
		static DefaultMessageCallback& GetInstance() { return s_Instance; }

		void message(nvrhi::MessageSeverity severity, const char* messageText) override;

		static DefaultMessageCallback s_Instance;
	};
	struct DeviceCreationParameters
	{
		bool enableDebugRuntime = false;
		bool headlessDevice = false;
		bool startMaximized = false;
		bool startFullscreen = false;
		bool allowModeSwitch = true;
		int windowPosX = -1;            // -1 means use default placement
		int windowPosY = -1;
		uint32_t backBufferWidth = 1280;
		uint32_t backBufferHeight = 720;
		uint32_t refreshRate = 0;
		uint32_t swapChainBufferCount = 3;
		nvrhi::Format swapChainFormat = nvrhi::Format::SRGBA8_UNORM;
		uint32_t swapChainSampleCount = 1;
		uint32_t swapChainSampleQuality = 0;
		uint32_t maxFramesInFlight = 2;
		bool enableNvrhiValidationLayer = false;
		bool vsyncEnabled = false;
		bool enableRayTracingExtensions = false; // for vulkan
		bool enableComputeQueue = false;
		bool enableCopyQueue = false;

		// Index of the adapter (DX11, DX12) or physical device (Vk) on which to initialize the device.
		// Negative values mean automatic detection.
		// The order of indices matches that returned by DeviceManager::EnumerateAdapters.
		int adapterIndex = -1;

		// set to true to enable DPI scale factors to be computed per monitor
		// this will keep the on-screen window size in pixels constant
		//
		// if set to false, the DPI scale factors will be constant but the system
		// may scale the contents of the window based on DPI
		//
		// note that the backbuffer size is never updated automatically; if the app
		// wishes to scale up rendering based on DPI, then it must set this to true
		// and respond to DPI scale factor changes by resizing the backbuffer explicitly
		bool enablePerMonitorDPI = false;

		DXGI_USAGE swapChainUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_2;
	};

	class DeviceManager
	{
	public:
		bool Init(const DeviceCreationParameters& deviceCreationParams);

		std::string GetAdapterName(DXGI_ADAPTER_DESC const& desc)
		{
			size_t length = wcsnlen(desc.Description, _countof(desc.Description));
			std::string name;
			name.resize(length);
			WideCharToMultiByte(CP_ACP, 0, desc.Description, static_cast<int>(length), name.data(), static_cast<int>(name.size()), nullptr, nullptr);
			return name;
		}
		const char* GetRendererString() const
		{
			return m_rendererString.c_str();
		}
		nvrhi::IDevice* GetDevice() const
		{
			return m_nvrhiDevice;
		}

	private:
		DeviceCreationParameters m_deviceCreationParams{};
		nvrhi::RefCountPtr<IDXGIFactory2> m_dxgiFactory2;
		nvrhi::RefCountPtr<ID3D12Device> m_device12;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_graphicsQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_computeQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_copyQueue;
		nvrhi::RefCountPtr<IDXGISwapChain3> m_swapChain;
		DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_fullscreenDesc{};
		nvrhi::RefCountPtr<IDXGIAdapter> m_dxgiAdapter;
		HWND m_hWnd{ nullptr };
		bool m_tearingSupported{ false };
		
		std::vector<std::shared_ptr<ID3D12Resource>> m_swapChainBuffers;
		std::vector<nvrhi::TextureHandle> m_rhiSwapChainBuffers;
		nvrhi::RefCountPtr<ID3D12Fence> m_frameFence;
		std::vector<HANDLE> m_frameFenceEvents;

		uint64_t m_frameCount{ 1 };

		nvrhi::DeviceHandle m_nvrhiDevice;
		std::string m_rendererString;

		bool CreateDevice(const DeviceCreationParameters& deviceCreationParams);
		bool CreateSwapChain();
		bool CreateRenderTargets();
		void ResizeSwapChain();
		void ReleaseRenderTargets();
		void DestroyDeviceAndSwapChain();
	};
}