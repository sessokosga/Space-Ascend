#include "anim.h"

Anim LoadAnim(const char *pFileName,const int pId, int pFrameSpeed, int pMaxFrame,int pLoop)
{
    Anim anim;
    anim.frameCounter = 0;
    anim.frameSpeed = pFrameSpeed;
    anim.maxFrame = pMaxFrame;
    Image img = LoadImage(pFileName);
    anim.texture = LoadTextureFromImage(img);
    UnloadImage(img);
    anim.frameRec.x = 0;
    anim.frameRec.y = 0;
    anim.frameRec.width = (float)(anim.texture.width / (pMaxFrame + 1));
    anim.frameRec.height = anim.texture.height;
    anim.pos.x = 0;
    anim.pos.y = 0;
    anim.id = pId;
    anim.currentFrame = 0;
    anim.loop =pLoop;
    anim.ended=0;
    anim.flipX=1;
    anim.flipY=1;

    return anim;
}

void updateAnim(Anim *pAnim)
{
    if (pAnim->ended==0)
    {
        pAnim->frameCounter++;

        if (pAnim->frameCounter >= 60 / pAnim->frameSpeed )
        {
            pAnim->frameCounter = 0;
            pAnim->currentFrame++;

            if (pAnim->currentFrame > pAnim->maxFrame)
            {
                if( pAnim->loop==1)
                {
                    pAnim->currentFrame = 0;
                    pAnim->ended = 0;
                }
                else
                {
                    pAnim->ended = 1;
                    pAnim->currentFrame = pAnim->maxFrame;
                }
            }
            pAnim->frameRec.x = (float)pAnim->currentFrame * (float)(pAnim->texture.width / (pAnim->maxFrame + 1));
        }
    }
}
void drawAnim(Anim pAnim)
{
    DrawTextureRec(pAnim.texture, pAnim.frameRec, pAnim.pos, WHITE);
}

void unloadAnim(Anim pAnim)
{
    UnloadTexture(pAnim.texture);
}

void resetAnim(Anim *pAnim)
{
    pAnim->currentFrame=0;
    pAnim->frameCounter=0;
    pAnim->ended=0;
}

void flipAnimHorizontal(Anim *pAnim){
    pAnim->frameRec.width*=-1;
    pAnim->flipX*=-1;
}

void flipAnimVertical(Anim *pAnim){
    pAnim->frameRec.height*=-1;
    pAnim->flipY*=-1;
}


