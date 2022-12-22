#include "projectile.h"

Projectile loadProjectile(int pX,int pY, Anim pIdleAnim, Anim pDeadAnim)
{
    Projectile proj;
    proj.pos.x = pX;
    proj.pos.y = pY;
    proj.idleAnim = pIdleAnim;
    proj.deadAnim = pDeadAnim;
    proj.del = 0;
    proj.velocity.x=0;
    proj.velocity.y=0;
    return proj;
}

void updateProjectile(Projectile *pProj)
{
    pProj->pos.x += pProj->velocity.x;
    pProj->pos.y += pProj->velocity.y;


    if(pProj->del==0)
    {
        pProj->idleAnim.pos =pProj->pos;
        updateAnim(&pProj->idleAnim);
    }
    else
    {
        pProj->deadAnim.pos =pProj->pos;
        updateAnim(&pProj->deadAnim);
    }
}

void drawProjectile(Projectile pProj)
{
     if(pProj.del==0)
    {
        drawAnim(pProj.idleAnim);
    }
    else
    {
       drawAnim(pProj.deadAnim);
    }
}

void unloadProjectile(Projectile pProj)
{
    unloadAnim(pProj.idleAnim);
    unloadAnim(pProj.deadAnim);
}
