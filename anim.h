#ifndef ANIM_H_INCLUDED
#define ANIM_H_INCLUDED

#include "raylib.h"

typedef struct
{
    int frameCounter, frameSpeed, currentFrame, maxFrame;
    Vector2 pos;
    Texture texture;
    Rectangle frameRec;
    int loop,id,ended,flipX,flipY;
} Anim;

Anim LoadAnim(const char *pFileName,const int pId, int pFrameSpeed, int pMaxFrame,int pLoop);
void updateAnim(Anim *pAnim);
void drawAnim(Anim pAnim);
void unloadAnim(Anim pAnim);
void resetAnim(Anim *pAnim);
void flipAnimHorizontal(Anim *pAnim);
void flipAnimVertical(Anim *pAnim);

#endif // ANIM_H_INCLUDED
