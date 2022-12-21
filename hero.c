#include "hero.h"
#include "constants.h"

HeroState state;
Hero loadHero()
{
    Hero hero;
    hero.pos.x=0;
    hero.pos.y=0;
    hero.velocity.x=0;
    hero.velocity.y=0;
    hero.animCount=0;
    hero.state = IDLE;
    hero.flipX=1;
    hero.flipY=1;
    return hero;
}

void addHeroAnim(Hero* pHero, Anim pAnim)
{
    pHero->listAnim[pHero->animCount++] = pAnim;

}

void playHeroAnim(Hero *pHero, const HeroState pId)
{
    int found=0;
    if (pHero->currentAnim.id != pId)
    {
        for (int i=0; i<pHero->animCount; i++)
        {
            //printf("Searching...\n\t %s <-> %s => %d\n",pName,pHero->listAnim[i].name,strcmp(pName,pHero->listAnim[i].name));
            if(pId==pHero->listAnim[i].id)
            {
                pHero->currentAnim = pHero->listAnim[i];
                resetAnim(&pHero->currentAnim);
                found=1;
                //printf("Playin anim %d => %d\n",i,pHero->listAnim[i].id);
            }
        }
        if(found==0)
        {
            printf("The animation %d is not found\n",pId);
        }
    }
}

void updateHero(Hero *pHero, float dt)
{
    if (pHero->state != DEAD)
    {
        if (pHero->type == HERO)
            pHero->velocity.x=pHero->velocity.y=0;
        state = pHero->state;
        if(pHero->type == HERO)
        {
            if ((pHero->currentAnim.id == RUN || pHero->currentAnim.id == IDLE ) || (pHero->currentAnim.id != RUN && pHero->currentAnim.ended))
            {
                if(IsKeyDown(KEY_A))
                {
                    if (pHero->pos.x> -40)
                    {
                        pHero->velocity.x=-2;
                        pHero->flipX=-1;
                    }
                    state = RUN;
                }

                if(IsKeyDown(KEY_D))
                {
                    if (pHero->pos.x< 26)
                    {
                        pHero->velocity.x=+2;
                        pHero->flipX=1;
                    }
                    state = RUN;
                }

                if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && pHero->currentAnim.id == RUN)
                {
                    state = IDLE;
                }

            }


            if(IsKeyDown(KEY_W))
            {
                state =JUMP;
            }

            if(IsKeyDown(KEY_S))
            {
                state = EAT;
            }

            if(IsKeyDown(KEY_I))
            {
                state = ATTACK_1;
            }

            if(IsKeyDown(KEY_J))
            {
                state = ATTACK_2;
            }

            if(IsKeyDown(KEY_M))
            {
                state = DEAD;
            }

            if(IsKeyDown(KEY_H))
            {
                state = HURT;
            }

            pHero->pos.x+=pHero->velocity.x;
            pHero->pos.y+=pHero->velocity.y;

        }
        printf("x: %lf\n",pHero->pos.x);
        if(pHero->type==SHIP)
        {
            if(pHero->pos.x+55 > SCREEN_WIDTH)
            {
                pHero->pos.x = (float)(SCREEN_WIDTH - 55);
                pHero->velocity.x=0;
            }
            else if(pHero->pos.x<138)
            {
                pHero->pos.x = 138;
                pHero->velocity.x=0;
            }

            float res = pHero->pos.x+pHero->velocity.x;
            if(!(res+55>SCREEN_WIDTH || res<138))
                pHero->pos.x = res;


            pHero->pos.y+=pHero->velocity.y;
        }




        // Play the right animation
        pHero->state = state;
        if(pHero->currentAnim.ended==1 && pHero->state!=DEAD)
        {
            pHero->state = IDLE;
        }
        switch(pHero->state)
        {
        case RUN:
            playHeroAnim(pHero,RUN);
            break;
        case ATTACK_1:
            playHeroAnim(pHero,ATTACK_1);
            break;
        case ATTACK_2:
            playHeroAnim(pHero,ATTACK_2);
            break;
        case JUMP:
            playHeroAnim(pHero,JUMP);
            break;
        case EAT:
            playHeroAnim(pHero,EAT);
            break;
        case HURT:
            playHeroAnim(pHero,HURT);
            break;
        case DEAD:
            playHeroAnim(pHero,DEAD);
            break;
        default :
            playHeroAnim(pHero,IDLE);
            break;
        }





        pHero->currentAnim.pos = pHero->pos;
        if(pHero->currentAnim.flipX != pHero->flipX)
        {
            flipAnimHorizontal(&pHero->currentAnim);
        }
        if (pHero->currentAnim.flipY != pHero->flipY)
        {
            flipAnimVertical(&pHero->currentAnim);
        }
    }
    updateAnim(&pHero->currentAnim);
}

void drawHero(Hero pHero)
{
    drawAnim(pHero.currentAnim);
}

void unloadHero(Hero pHero)
{
    for (int i=0; i<pHero.animCount; i++)
    {
        UnloadTexture(pHero.listAnim[i].texture);
    }
}
