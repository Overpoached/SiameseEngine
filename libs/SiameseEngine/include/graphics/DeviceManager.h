#pragma once

#include <d3d12.h>
#include <Windows.h>
#include <dxgi1_5.h>
#include <dxgidebug.h>

#include <nvrhi/d3d12.h>
#include <nvrhi/validation.h>

namespace sengine
{
	static nvrhi::Format DXGItoNVRHI(DXGI_FORMAT format);

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
		bool headlessDevice = false;
		bool enableDebugRuntime = false;
		bool enableNvrhiValidationLayer = false;
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

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_2;
	};

	class DeviceManager
	{
	public:
		bool Init(const DeviceCreationParameters& deviceCreationParams);
		bool MoveWindowOntoAdapter(RECT& rect);

		//getters
		std::string GetAdapterName(DXGI_ADAPTER_DESC const& desc) const
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
		bool GetTearingSupported() const
		{
			return m_tearingSupported;
		}

		nvrhi::IDevice* GetDevice()
		{
			return m_nvrhiDevice;
		}

		//some helper tools to create stuff
		bool CreateSwapChain(HWND hWnd, DXGI_SWAP_CHAIN_DESC1& swapChainDesc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC& fullscreenDesc, nvrhi::RefCountPtr<IDXGISwapChain3> swapChainOut, std::vector<nvrhi::RefCountPtr<ID3D12Resource>>& swapChainBuffersOut, std::vector<nvrhi::TextureHandle>& rhiBuffersOut);
		bool CreateFrameFench(nvrhi::RefCountPtr<ID3D12Fence>& frameFenceOut);

	private:
		DeviceCreationParameters m_deviceCreationParams{};
		nvrhi::RefCountPtr<IDXGIFactory2> m_dxgiFactory2;
		nvrhi::RefCountPtr<ID3D12Device> m_device12;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_graphicsQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_computeQueue;
		nvrhi::RefCountPtr<ID3D12CommandQueue> m_copyQueue;
		nvrhi::RefCountPtr<IDXGIAdapter> m_dxgiAdapter;
		
		nvrhi::DeviceHandle m_nvrhiDevice;
		std::string m_rendererString;

		//some capabilities of the device
		bool m_tearingSupported{};

		bool CreateDevice(const DeviceCreationParameters& deviceCreationParams);
		bool CreateRenderTargets();
		void ResizeSwapChain();
		void ReleaseRenderTargets();
		void DestroyDeviceAndSwapChain();
	};
}