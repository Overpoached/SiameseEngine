#include "sepch.h"

#include "graphics/DeviceManager.h"

#include "Application.h"
#include "core/Log.h"

sengine::DefaultMessageCallback sengine::DefaultMessageCallback::s_Instance{};

void sengine::DefaultMessageCallback::message(nvrhi::MessageSeverity severity, const char* messageText)
{
	switch (severity) {
	case nvrhi::MessageSeverity::Info:
		SENGINE_INFO("{}", messageText);
		break;
	case nvrhi::MessageSeverity::Warning:
		SENGINE_WARN("{}", messageText);
		break;
	case nvrhi::MessageSeverity::Error:
		SENGINE_ERROR("{}", messageText);
		break;
	case nvrhi::MessageSeverity::Fatal:
		SENGINE_CRITICAL("{}", messageText);
		break;
	}
}

bool sengine::DeviceManager::MoveWindowOntoAdapter(RECT& rect)
{
	assert(m_dxgiAdapter != NULL);

	HRESULT hres = S_OK;
	unsigned int outputNo = 0;
	while (SUCCEEDED(hres))
	{
		nvrhi::RefCountPtr<IDXGIOutput> pOutput;
		hres = m_dxgiAdapter->EnumOutputs(outputNo++, &pOutput);

		if (SUCCEEDED(hres) && pOutput)
		{
			DXGI_OUTPUT_DESC OutputDesc;
			pOutput->GetDesc(&OutputDesc);
			const RECT desktop = OutputDesc.DesktopCoordinates;
			const int centreX = (int)desktop.left + (int)(desktop.right - desktop.left) / 2;
			const int centreY = (int)desktop.top + (int)(desktop.bottom - desktop.top) / 2;
			const int winW = rect.right - rect.left;
			const int winH = rect.bottom - rect.top;
			const int left = centreX - winW / 2;
			const int right = left + winW;
			const int top = centreY - winH / 2;
			const int bottom = top + winH;
			rect.left = std::max(left, (int)desktop.left);
			rect.right = std::min(right, (int)desktop.right);
			rect.bottom = std::min(bottom, (int)desktop.bottom);
			rect.top = std::max(top, (int)desktop.top);

			// If there is more than one output, go with the first found.  Multi-monitor support could go here.
			return true;
		}
	}
	return false;
}

static bool IsNvDeviceID(UINT id)
{
	return id == 0x10DE;
}

bool sengine::DeviceManager::Init(const DeviceCreationParameters& deviceCreationParams)
{
	bool ret = CreateDevice(deviceCreationParams);
	return ret;
}

bool sengine::DeviceManager::CreateSwapChain(HWND hWnd, DXGI_SWAP_CHAIN_DESC1& swapChainDesc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC& fullscreenDesc, nvrhi::RefCountPtr<IDXGISwapChain3> swapChainOut, std::vector<nvrhi::RefCountPtr<ID3D12Resource>>& swapChainBuffersOut, std::vector<nvrhi::TextureHandle>& rhiBuffersOut)
{
	nvrhi::RefCountPtr<IDXGISwapChain1> pSwapChain1;
	HRESULT hr = m_dxgiFactory2->CreateSwapChainForHwnd(m_graphicsQueue, hWnd, &swapChainDesc, &fullscreenDesc, nullptr, &pSwapChain1);
	HR_RETURN(hr);

	hr = pSwapChain1->QueryInterface(IID_PPV_ARGS(&swapChainOut));
	HR_RETURN(hr);

	swapChainBuffersOut.resize(swapChainDesc.BufferCount);
	rhiBuffersOut.resize(swapChainDesc.BufferCount);

	for (UINT n = 0; n < swapChainDesc.BufferCount; n++)
	{
		const HRESULT hr = swapChainOut->GetBuffer(n, IID_PPV_ARGS(&swapChainBuffersOut[n]));
		HR_RETURN(hr);

		nvrhi::TextureDesc textureDesc;
		textureDesc.width = swapChainDesc.Width;
		textureDesc.height = swapChainDesc.Height;
		textureDesc.sampleCount = swapChainDesc.SampleDesc.Count;
		textureDesc.sampleQuality = swapChainDesc.SampleDesc.Quality;
		textureDesc.format = DXGItoNVRHI(swapChainDesc.Format);
		textureDesc.debugName = "SwapChainBuffer" + std::to_string(n);
		textureDesc.isRenderTarget = true;
		textureDesc.isUAV = false;
		textureDesc.initialState = nvrhi::ResourceStates::Present;
		textureDesc.keepInitialState = true;

		rhiBuffersOut[n] = m_nvrhiDevice->createHandleForNativeTexture(nvrhi::ObjectTypes::D3D12_Resource, nvrhi::Object(swapChainBuffersOut[n]), textureDesc);
	}
	return true;
}

bool sengine::DeviceManager::CreateFrameFench(nvrhi::RefCountPtr<ID3D12Fence>& frameFenceOut)
{
	HRESULT hr = m_device12->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&frameFenceOut));
	HR_RETURN(hr);
	return true;
}

bool sengine::DeviceManager::CreateDevice(const DeviceCreationParameters& deviceCreationParams)
{
	//most of this is copied from donut
	m_deviceCreationParams = deviceCreationParams;
	if (m_deviceCreationParams.enableDebugRuntime)
	{
		nvrhi::RefCountPtr<ID3D12Debug> pDebug;
		HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug));
		pDebug->EnableDebugLayer();
	}
	int adapterIndex = m_deviceCreationParams.adapterIndex;
	if (adapterIndex < 0)
		adapterIndex = 0;
	//create factory
	HRESULT res = CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory2));
	if (res != S_OK)
		__debugbreak();

	if (m_dxgiFactory2->EnumAdapters(static_cast<UINT>(adapterIndex), &m_dxgiAdapter) != S_OK)
	{
		if (adapterIndex == 0)
			SENGINE_ERROR("Cannot find any DXGI adapters in the system.");
		else
			SENGINE_ERROR("The specified DXGI adapter %d does not exist.", adapterIndex);
		__debugbreak();
	}

	{
		DXGI_ADAPTER_DESC aDesc;
		m_dxgiAdapter->GetDesc(&aDesc);

		std::string name = GetAdapterName(aDesc);
		bool isNvidia = IsNvDeviceID(aDesc.VendorId);
		SENGINE_INFO("Adapter name: {}", name);
		SENGINE_INFO("Is nvidia: {}", isNvidia);
	}

	HRESULT hr = D3D12CreateDevice(
		m_dxgiAdapter,
		m_deviceCreationParams.featureLevel,
		IID_PPV_ARGS(&m_device12));
	HR_RETURN(hr);

	if (m_deviceCreationParams.enableDebugRuntime)
	{
		nvrhi::RefCountPtr<ID3D12InfoQueue> pInfoQueue;
		m_device12->QueryInterface(&pInfoQueue);

		if (pInfoQueue)
		{
#ifdef _DEBUG
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif

			D3D12_MESSAGE_ID disableMessageIDs[] = {
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_COMMAND_LIST_STATIC_DESCRIPTOR_RESOURCE_DIMENSION_MISMATCH, // descriptor validation doesn't understand acceleration structures
			};

			D3D12_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.pIDList = disableMessageIDs;
			filter.DenyList.NumIDs = sizeof(disableMessageIDs) / sizeof(disableMessageIDs[0]);
			pInfoQueue->AddStorageFilterEntries(&filter);
		}
	}

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	ZeroMemory(&queueDesc, sizeof(queueDesc));
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.NodeMask = 1;
	hr = m_device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_graphicsQueue));
	HR_RETURN(hr);
	m_graphicsQueue->SetName(L"Graphics Queue");

	if (m_deviceCreationParams.enableComputeQueue)
	{
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		hr = m_device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_computeQueue));
		HR_RETURN(hr);
		m_computeQueue->SetName(L"Compute Queue");
	}

	if (m_deviceCreationParams.enableCopyQueue)
	{
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
		hr = m_device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_copyQueue));
		HR_RETURN(hr);
		m_copyQueue->SetName(L"Copy Queue");
	}

	nvrhi::d3d12::DeviceDesc deviceDesc;
	deviceDesc.errorCB = &DefaultMessageCallback::GetInstance();
	deviceDesc.pDevice = m_device12;
	deviceDesc.pGraphicsCommandQueue = m_graphicsQueue;
	deviceDesc.pComputeCommandQueue = m_computeQueue;
	deviceDesc.pCopyCommandQueue = m_copyQueue;

	m_nvrhiDevice = nvrhi::d3d12::createDevice(deviceDesc);

	if (m_deviceCreationParams.enableNvrhiValidationLayer)
	{
		m_nvrhiDevice = nvrhi::validation::createValidationLayer(m_nvrhiDevice);
	}
	
	//get some capabilties of the device
	nvrhi::RefCountPtr<IDXGIFactory5> pDxgiFactory5;
	if (SUCCEEDED(m_dxgiFactory2->QueryInterface(IID_PPV_ARGS(&pDxgiFactory5))))
	{
		BOOL supported = 0;
		if (SUCCEEDED(pDxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &supported, sizeof(supported))))
			m_tearingSupported = (supported != 0);
	}

	return true;
}

nvrhi::Format sengine::DXGItoNVRHI(DXGI_FORMAT format)
{
	switch (format) {
	case DXGI_FORMAT_R8G8B8A8_UNORM:		return nvrhi::Format::RGBA8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:	return nvrhi::Format::SRGBA8_UNORM;
	case DXGI_FORMAT_B8G8R8A8_UNORM:		return nvrhi::Format::BGRA8_UNORM;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:	return nvrhi::Format::SBGRA8_UNORM;

	case DXGI_FORMAT_R16G16B16A16_FLOAT:	return nvrhi::Format::RGBA16_FLOAT;
	case DXGI_FORMAT_R16G16B16A16_UNORM:	return nvrhi::Format::RGBA16_UNORM;

	case DXGI_FORMAT_R32G32B32A32_FLOAT:	return nvrhi::Format::RGBA32_FLOAT;
	case DXGI_FORMAT_R32G32B32_FLOAT:		return nvrhi::Format::RGB32_FLOAT;

	case DXGI_FORMAT_R11G11B10_FLOAT:		return nvrhi::Format::R11G11B10_FLOAT;
	case DXGI_FORMAT_R10G10B10A2_UNORM:		return nvrhi::Format::R10G10B10A2_UNORM;

	case DXGI_FORMAT_R32_FLOAT:				return nvrhi::Format::R32_FLOAT;
	case DXGI_FORMAT_R32_UINT:				return nvrhi::Format::R32_UINT;
	case DXGI_FORMAT_R32_SINT:				return nvrhi::Format::R32_SINT;

		// Depth/Stencil
	case DXGI_FORMAT_D32_FLOAT:				return nvrhi::Format::D32;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:		return nvrhi::Format::D24S8;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:	return nvrhi::Format::D32S8;
	case DXGI_FORMAT_D16_UNORM:				return nvrhi::Format::D16;

		// BC Compressed (Standard for Textures)
	case DXGI_FORMAT_BC1_UNORM:				return nvrhi::Format::BC1_UNORM;
	case DXGI_FORMAT_BC3_UNORM:				return nvrhi::Format::BC3_UNORM;
	case DXGI_FORMAT_BC5_UNORM:				return nvrhi::Format::BC5_UNORM;
	case DXGI_FORMAT_BC7_UNORM:				return nvrhi::Format::BC7_UNORM;

	default:								return nvrhi::Format::UNKNOWN;
	}
}
