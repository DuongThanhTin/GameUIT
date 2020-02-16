#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
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
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								SetState(SIMON_STATE_DIE);
								StartUntouchable(); //làm mờ sau khi hồi sinh
							}
							else 
								SetState(SIMON_STATE_DIE);
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
	int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	//if (level == MARIO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (nx>0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;
			if (attack > 1) ani = SIMON_ANI_ATTACK;
			
		}
		else if (vx > 0) 
			ani = SIMON_ANI_WALKING_RIGHT;
	
		else ani = SIMON_ANI_WALKING_LEFT;
	}
	

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -MARIO_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_ATTACK:
		vx = 0;
		attack = 2;
		break;
	case SIMON_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

