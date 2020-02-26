#include "HeaderInfo.h"



CHeaderInfo::CHeaderInfo()
{
	CTextures::GetInstance()->Add(ID_TEX_HI_BG, L"textures\\HUD.png", D3DCOLOR_XRGB(255, 0, 255));
	this->texBackground = CTextures::GetInstance()->Get(ID_TEX_HI_BG);
}


CHeaderInfo::~CHeaderInfo()
{
}

void CHeaderInfo::Draw(D3DXVECTOR2 position)
{
	CGame::GetInstance()->Draw(0, 0, texBackground, 0, 0, 256, 50,0,0);
}