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

	// 명령 할당자 재사용을 위한 Reset
	// 명령 할당자는 모든 랜더링 명령이 끝난 후 초기화 해야 한다.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// 명령 목록 재사용을 위한 Reset
	// 명령 목록은 재출 했다면 언제든 재사용이 가능하다.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// 백버퍼를 랜더 타겟으로 사용하겠다고 상태를 전환한다.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
																D3D12_RESOURCE_STATE_PRESENT,
																D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 뷰포트와 가위 직사각형 설정 명령을 명령 목록에 추가한다.
	// 초기화마다 이 값을 초기화해줘야 한다.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// 백버퍼와 깊이 버퍼를 지운다.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 랜더링할 버퍼를 지정한다.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	// 백버퍼를 제시용으로 전환한다.?
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
																D3D12_RESOURCE_STATE_RENDER_TARGET, 
																D3D12_RESOURCE_STATE_PRESENT));

	// 명령어 목록을 닫아준다.
	ThrowIfFailed(mCommandList->Close());

	// 명령 목록을 대기열에 추가한다.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 제시
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// 프레임 명령이 완료될 때까지 기다린다. 
	FlushCommandQueue();
}
