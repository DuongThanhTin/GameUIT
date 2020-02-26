#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Game.h"
#include "Textures.h"

#define ID_TEX_HI_BG	50

class CHeaderInfo
{
private:
	LPDIRECT3DTEXTURE9 texBackground;
public:
	CHeaderInfo();
	~CHeaderInfo();
	void Draw(D3DXVECTOR2 position);
};

typedef CHeaderInfo* LPHI;

