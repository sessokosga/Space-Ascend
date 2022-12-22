#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include "raylib.h"
#include "anim.h"

typedef struct {
    Anim deadAnim;
    Anim idleAnim;
    Vector2 pos,velocity;
    int del;
} Projectile;

Projectile loadProjectile(int pX,int pY, Anim pIdleAnim, Anim pDeadAnim);
void updateProjectile(Projectile *pProj);
void drawProjectile(Projectile pProj);
void unloadProjectile(Projectile pProj);


#endif // PROJECTILE_H_INCLUDED
