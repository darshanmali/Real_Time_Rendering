
#include<stdio.h>
#include<d3d11.h>
#include<math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


int main(void)
{
	//Variable
	IDXGIFactory* pIDXIFactory = NULL;
	IDXGIAdapter* pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiadapterdesc;
	HRESULT hr;
	char str[255];
	
	//code
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), 
		(void**)&pIDXIFactory);
	if (FAILED(hr))
	{
		printf("createDXGIFactory failed...!\n");
		goto CleanUP;
	}
	else
	{
		printf("createDXGIFactory created...!\n");

	}

	if (pIDXIFactory->EnumAdapters(1, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		printf("dxgi can not found..!\n");
		goto CleanUP;
	}
	else
	{
		printf("dxgi found..!\n");
	}

	ZeroMemory((void*)&dxgiadapterdesc, sizeof(DXGI_ADAPTER_DESC));

	printf("ZeroMemory done.\n");

	hr = pIDXGIAdapter->GetDesc(&dxgiadapterdesc);
	if (FAILED(hr))
	{
		printf("GetDesc failed...!\n");
		goto CleanUP;
	}

	WideCharToMultiByte(CP_ACP, 
		0,
		dxgiadapterdesc.Description,
		255,
		str,
		255,
		NULL,
		NULL);

	printf("Graphics card Name IS: %s\n", str);

	printf("My Graphics card VRAM In GB = %d GB\n", (int)(ceil(dxgiadapterdesc.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0)));


CleanUP:
	if (pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;

	}
	if (pIDXIFactory) {
		pIDXIFactory->Release();
		pIDXIFactory = NULL;
	}

	return (0);

}

