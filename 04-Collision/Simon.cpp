#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"

CSimon::CSimon() {
	untouchable = 0;
	jumpStart = 0;
	attackStartSub = 0;
	id = 0;
	attackStart = 0;
	untouchableStart = 0;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchableStart > SIMON_UNTOUCHABLE_TIME)
	{
		untouchableStart = 0;
		untouchable = 0;
	}

	// update jump state
	if (GetTickCount() - jumpStart > SIMON_UNTOUCHABLE_TIME) {
		jumpStart = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx*dx + nx*0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty*dy + ny*0.4f;
		
		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -SIMON_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
								SetState(SIMON_STATE_DIE);
								StartUntouchable(); //làm mờ sau khi hồi sinh

						}
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	//http://gameprogrammingpatterns.com/state.html
	int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_ATTACK) {
		if (nx > 0)
			ani = SIMON_ANI_ATTACK_RIGHT;
		else
			ani = SIMON_ANI_ATTACLK_LEFT;
	}
	else if (state == SIMON_STATE_JUMP) {
		if (vy < 0) {
			if (nx > 0)
				ani = SIMON_ANI_SIT_ATTACK_RIGHT;
			else
				ani = SIMON_ANI_SIT_ATTACK_LEFT;
		}
		else {
			if (nx > 0)
				ani = SIMON_ANI_IDLE_RIGHT;
			else
				ani = SIMON_ANI_IDLE_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT) {
		if (nx > 0)
			ani = SIMON_ANI_SIT_RIGHT;
		else
			ani = SIMON_ANI_SIT_LEFT;
	}
	else {
		if (vx == 0) {
			if (nx > 0)
				ani = SIMON_ANI_IDLE_RIGHT;
			else
				ani = SIMON_ANI_IDLE_LEFT;
		}
		else {
			if (vx > 0)
				ani = SIMON_ANI_WALKING_RIGHT;
			else
				ani = SIMON_ANI_WALKING_LEFT;
		}
	}
	

	int alpha = 255;
	if (untouchableStart>0) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CSimon::RenderBoundingBox(int alpha)
{

}

void CSimon::SetState(int state)
{
	if (jumpStart > 0 && state != SIMON_STATE_ATTACK )
		return;
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_ATTACK:
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	/*left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}*/
	left = x;
	top = y - SIMON_BBOX_HEIGHT;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y;
}

void CSimon::StartUntouchable()
{
	untouchable = 1;
	untouchableStart = GetTickCount();
}

void CSimon::StartAttack() {
	if (attackStart > 0)
		return;
	if (attackStartSub > 0)
		return;

	if (state != SIMON_STATE_JUMP)
		vx = 0;

	ResetAnimation();
	SetState(SIMON_STATE_ATTACK);
	attackStart = GetTickCount();
}

void CSimon::StartJump()
{
	SetState(SIMON_STATE_JUMP);
	jumpStart = GetTickCount();
}
