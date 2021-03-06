#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED		0.06f 

#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.1f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f


#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_ATTACK			500
#define SIMON_STATE_SIT				600
#define SIMON_STATE_SIT_ATTACK		700


#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_ATTACK_RIGHT		4
#define SIMON_ANI_ATTACLK_LEFT		5
#define SIMON_ANI_SIT_RIGHT			6
#define SIMON_ANI_SIT_LEFT			7
#define SIMON_ANI_SIT_ATTACK_RIGHT	8
#define SIMON_ANI_SIT_ATTACK_LEFT	9
#define SIMON_ANI_DIE				10


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define SIMON_BBOX_WIDTH  15
#define SIMON_BBOX_HEIGHT 30


#define SIMON_UNTOUCHABLE_TIME 5000

#define SIMON_ATTACK_TIME			350
#define SIMON_JUMP_TIME				650

class CSimon : public CGameObject
{
	int level;
	int untouchable;

	DWORD untouchableStart;
	DWORD attackStart;
	DWORD attackStartSub;
	DWORD jumpStart;
public: 
	CSimon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void RenderBoundingBox(int alpha = 255);
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable();
	void StartAttack();
	void StartAttackSub();
	void StartJump();

	DWORD GetAttackStart() { return attackStart; }
	DWORD GetJumpStart() { return jumpStart; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};