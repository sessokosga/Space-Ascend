#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "anim.h"
#include "raylib.h"
#include <string.h>
#include <stdio.h>

typedef enum {
    IDLE = 0,
    RUN = 1,
    JUMP=2,
    ATTACK_1=3,
    ATTACK_2=4,
    DEAD=5,
    EAT=6,
    HURT=7
} HeroState;

typedef enum {
    HERO,
    METEOR,
    SHIP
} HeroType;

typedef struct
{
    Vector2 pos, velocity;
    Anim currentAnim;
    int animCount,flipX,flipY;
    HeroState state;
    HeroType type;
    Anim listAnim [10];
} Hero;



Hero loadHero();
void addHeroAnim(Hero* pHero, Anim pAnim);
void playHeroAnim(Hero *pHero,const HeroState pId);
void updateHero(Hero *pHero, float dt);
void drawHero(Hero pHero);
void unloadHero(Hero pHero);

#endif // HERO_H_INCLUDED
