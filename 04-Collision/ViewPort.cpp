#include "ViewPort.h"

CViewPort* CViewPort::__instance = NULL;

CViewPort::CViewPort()
{
	position = { 50, 0 };
	height = 176;
	width = 320;
}

CViewPort::CViewPort(D3DXVECTOR2 position, int width, int height)
{
	this->position = position;
	this->width = width;
	this->height = height;
}

void CViewPort::SetPosition(D3DXVECTOR2 position) {
	this->position = position;
}

D3DXVECTOR2 CViewPort::GetPosition() {
	return position;
}

D3DXVECTOR2 CViewPort::ConvertWorldToViewPort(D3DXVECTOR2 worldPosition) {
	return{ worldPosition.x - position.x, worldPosition.y - position.y };
}

D3DXVECTOR2 CViewPort::ConvertViewPortToWorld(D3DXVECTOR2 viewportPosition) {
	return{ viewportPosition.x + position.x, viewportPosition.y + position.y };
}

void CViewPort::Update(D3DXVECTOR2 playerPosition) {
	position.x = playerPosition.x- 320 / 2 +15;
	//position.x = 160;
	
	//Kiểm tra viewport ra ngoài world
	if (position.x < 0)
		position.x = 0;
	if (position.y < 0)
		position.y = 0;
	DebugOut(L"Position: %d\n", position.x);
}

CViewPort::~CViewPort()
{
}


CViewPort * CViewPort::GetInstance()
{
	if (__instance == NULL) __instance = new CViewPort();
	return __instance;
}