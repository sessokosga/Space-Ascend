#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include "raylib.h"

typedef struct {

    Texture texture;
    Vector2 pos,velocity;
    int del;
    struct Projectile* next;
} Projectile;

Projectile loadProjectile(int pX,int pY, Texture pTexture);
void updateProjectile(Projectile *pProj);
void drawProjectile(Projectile pProj);
void unloadProjectile(Projectile pProj);


#endif // PROJECTILE_H_INCLUDED
