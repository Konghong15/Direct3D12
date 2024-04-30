#include "DemoApp.h"

DemoApp::DemoApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

DemoApp::~DemoApp()
{
}

bool DemoApp::Initialize()
{
	if (!D3DApp::Initialize())
	{
		return false;
	}

	return true;
}

void DemoApp::OnResize()
{
	D3DApp::OnResize();
}

void DemoApp::Update(const GameTimer& gt)
{

}

void DemoApp::Draw(const GameTimer& gt)
{
	using namespace DirectX;

	// ��� �Ҵ��� ������ ���� Reset
	// ��� �Ҵ��ڴ� ��� ������ ����� ���� �� �ʱ�ȭ �ؾ� �Ѵ�.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// ��� ��� ������ ���� Reset
	// ��� ����� ���� �ߴٸ� ������ ������ �����ϴ�.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// ����۸� ���� Ÿ������ ����ϰڴٰ� ���¸� ��ȯ�Ѵ�.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
																D3D12_RESOURCE_STATE_PRESENT,
																D3D12_RESOURCE_STATE_RENDER_TARGET));

	// ����Ʈ�� ���� ���簢�� ���� ����� ��� ��Ͽ� �߰��Ѵ�.
	// �ʱ�ȭ���� �� ���� �ʱ�ȭ����� �Ѵ�.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// ����ۿ� ���� ���۸� �����.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// �������� ���۸� �����Ѵ�.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	// ����۸� ���ÿ����� ��ȯ�Ѵ�.?
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
																D3D12_RESOURCE_STATE_RENDER_TARGET, 
																D3D12_RESOURCE_STATE_PRESENT));

	// ��ɾ� ����� �ݾ��ش�.
	ThrowIfFailed(mCommandList->Close());

	// ��� ����� ��⿭�� �߰��Ѵ�.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// ����
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// ������ ����� �Ϸ�� ������ ��ٸ���. 
	FlushCommandQueue();
}
