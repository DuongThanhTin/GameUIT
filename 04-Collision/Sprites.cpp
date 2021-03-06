#include "Sprites.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int isFlipImage, D3DXVECTOR2 position)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
	this->isFlipImage = isFlipImage;
	this->position = position;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	int xTmp = x + position.x;
	int yTmp = y + position.y - (bottom - top);

	game->Draw(xTmp, yTmp, texture, left, top, right, bottom, alpha, isFlipImage);
	//game->Draw(x + position.x, y + position.y, texture, left, top, right, bottom, alpha, isFlipImage);
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int isFlipImage, D3DXVECTOR2 position )
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex, isFlipImage,position);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}



void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t=this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	//ViewPort
	CViewPort* viewport = CViewPort::GetInstance();
	x = viewport->ConvertWorldToViewPort({ x,y }).x;
	y = viewport->ConvertWorldToViewPort({ x,y }).y;
	isFinished = false;

	DWORD now = GetTickCount();
	if (currentFrame == -1) 
	{
		currentFrame = 0; 
		lastFrameTime = now;
		if (currentFrame == frames.size())
		{
			currentFrame = 0;
			isFinished = true;
		}
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
		
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::Render(float x, float y, int frame, int alpha)
{
	if (frame == -1 || frame == frames.size())
		frame = 0;
	frames[frame]->GetSprite()->Draw(x, y);
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}


