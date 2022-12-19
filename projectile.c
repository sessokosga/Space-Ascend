#include "projectile.h"

Projectile loadProjectile(int pX,int pY, Texture pTexture){
    Projectile proj;
    proj.pos.x = pX;
    proj.pos.y = pY;
    proj.texture = pTexture;
    proj.del = 0;
    proj.velocity.x=0;
    proj.velocity.y=0;
    return proj;
}

void updateProjectile(Projectile *pProj){
    pProj->pos.x += pProj->velocity.x;
    pProj->pos.y += pProj->velocity.y;
}

void drawProjectile(Projectile pProj){
    DrawTexture(pProj.texture,pProj.pos.x,pProj.pos.y,WHITE);
}

void unloadProjectile(Projectile pProj){
    UnloadTexture(pProj.texture);
}
