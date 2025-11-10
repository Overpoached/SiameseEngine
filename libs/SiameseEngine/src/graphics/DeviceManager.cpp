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

static bool IsNvDeviceID(UINT id)
{
	return id == 0x10DE;
}

bool sengine::DeviceManager::Init(const DeviceCreationParameters& deviceCreationParams)
{
	bool ret = CreateDevice(deviceCreationParams);
	return ret;
}

bool sengine::DeviceManager::CreateDevice(const DeviceCreationParameters& deviceCreationParams)
{
	//most of this is copied from donut
	m_deviceCreationParams = deviceCreationParams;
#define HR_RETURN(hr) if(FAILED(hr)) return false;
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

	if (m_dxgiFactory2->EnumAdapters(adapterIndex, &m_dxgiAdapter))
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
		SENGINE_INFO("Adapter name:{}", name);
		SENGINE_INFO("Is nvidia:{}", isNvidia);
	}

	HRESULT hr = D3D12CreateDevice(
		m_dxgiAdapter,
		m_deviceCreationParams.featureLevel,
		IID_PPV_ARGS(&m_device12));
	HR_RETURN(hr)

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
	HR_RETURN(hr)
		m_graphicsQueue->SetName(L"Graphics Queue");

	if (m_deviceCreationParams.enableComputeQueue)
	{
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		hr = m_device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_computeQueue));
		HR_RETURN(hr)
			m_computeQueue->SetName(L"Compute Queue");
	}

	if (m_deviceCreationParams.enableCopyQueue)
	{
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
		hr = m_device12->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_copyQueue));
		HR_RETURN(hr)
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

	return true;
}
