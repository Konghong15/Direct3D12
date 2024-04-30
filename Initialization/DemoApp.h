#pragma once

#include <DirectXColors.h>
#include "../Common/d3dApp.h"

class DemoApp : public D3DApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	virtual bool Initialize() override;

private:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& timer) override;
	virtual void Draw(const GameTimer& timer) override;
};

