#include "raylib.h"
#include "anim.h"
#include "hero.h"
#include "projectile.h"
#include "constants.h"
#include <time.h>



int isColliding( float x1, float y1, float w1,float h1,float x2, float y2,float w2,float h2)
{
    return x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1;
}

typedef struct Button
{
    Texture texture,textureActive;
    Vector2 pos;
    int isActive;
} Button;

int projectilesCount, meteorsCount;
Anim animIdle, animDead;


Projectile listProjectiles[MAX_PROJECTILES];
Hero listMeteors[MAX_METEORS];

void addProjetile(float pX,float pY,float pVy)
{
    if(projectilesCount<MAX_PROJECTILES)
    {
        listProjectiles[projectilesCount]= loadProjectile(pX,pY,animIdle,animDead);
        listProjectiles[projectilesCount].velocity.y=pVy;
        projectilesCount++;
    }
}

void updateListProjectiles()
{
    for(int i=projectilesCount-1; i>=0; i--)
    {
        for(int m=meteorsCount-1; m>=0; m--)
        {
            if (listProjectiles[i].del==0 &&
                    isColliding(listProjectiles[i].pos.x, listProjectiles[i].pos.y, listProjectiles[i].idleAnim.frameRec.width/1.5,
                                listProjectiles[i].idleAnim.frameRec.height,
                                listMeteors[m].pos.x,listMeteors[m].pos.y,listMeteors[m].currentAnim.frameRec.width/1.5,
                                listMeteors[m].currentAnim.frameRec.height/8))
            {
                listProjectiles[i].del = 1;
                if (listMeteors[m].energy>0)
                    listMeteors[m].energy-=.5;
printf("Meteor : %.2f\n",listMeteors[m].energy);
                listProjectiles[i].velocity.y=0;
            }
        }
        updateProjectile(&listProjectiles[i]);
        if((listProjectiles[i].del ==1 && listProjectiles[i].deadAnim.ended==1) || listProjectiles[i].pos.y< -100){
            removeProjectile(i);
        }
    }
}

void drawListProjectiles()
{
    for(int i=0; i<projectilesCount; i++)
    {
        drawProjectile(listProjectiles[i]);
    }
}

void removeProjectile(int num)
{
    for (int i=num; i<projectilesCount-1; i++)
    {
        listProjectiles[i] = listProjectiles[i+1];
    }
    projectilesCount--;
}

void unloadListProjectiles()
{
    for(int i=0; i<projectilesCount; i++)
    {
        unloadProjectile(listProjectiles[i]);
    }

}

void addMeteor(float pX, float pY, float pVx, float pVy, int pEnergy, Anim pIdle, Anim pExplosion)
{
    if (meteorsCount<MAX_METEORS)
    {
        listMeteors[meteorsCount] = loadHero();
        listMeteors[meteorsCount].pos.x = pX;
        listMeteors[meteorsCount].type = METEOR;
        listMeteors[meteorsCount].pos.y = pY;
        listMeteors[meteorsCount].velocity.x = pVx;
        listMeteors[meteorsCount].velocity.y = pVy;
        listMeteors[meteorsCount].energy=pEnergy;
        addHeroAnim(&listMeteors[meteorsCount],pIdle);
        addHeroAnim(&listMeteors[meteorsCount],pExplosion);
        meteorsCount++;

    }
}

void updateMeteors()
{
    for (int i=meteorsCount-1; i>=0; i--)
    {
        updateHero(&listMeteors[i]);
        if((listMeteors[i].state==DEAD && listMeteors[i].currentAnim.ended)|| listMeteors[i].pos.y>SCREEN_HEIGHT+20 ){
            deleteMeteor(i);
        }
    }
}

void drawMeteors()
{
    for (int i=0; i<meteorsCount; i++)
    {
        drawHero(listMeteors[i]);
    }
}

void unloadMeteors()
{
    for (int i=0; i<meteorsCount; i++)
    {
        unloadHero(listMeteors[i]);
    }
}

void deleteMeteor(int num)
{
    for (int i=num; i<meteorsCount-1; i++)
    {
        listMeteors[i]=listMeteors[i+1];
    }
    meteorsCount--;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    float dt,engineTimer=0,timeElapsed=0;
    char timeText[50]="";

    InitWindow(screenWidth, screenHeight, GAME_TITLE);

    projectilesCount=0;

    // Background
    Image imgPurple = LoadImage("resources/images/Background/Purple Nebula 7.png");
    Texture bgPurple = LoadTextureFromImage(imgPurple);
    UnloadImage(imgPurple);
    Image imgStarfield = LoadImage("resources/images/Background/Starfield 8.png");
    Texture bgStarfield = LoadTextureFromImage(imgStarfield);
    UnloadImage(imgStarfield);
    int currentBG=1;

    // Hero
    Hero kunoichi = loadHero();
    kunoichi.pos.x=0;
    kunoichi.pos.y=SCREEN_HEIGHT-150;
    kunoichi.type= HERO;
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Run.png",RUN,8, 7,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Idle.png",IDLE,8, 8,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_1.png",ATTACK_1,10, 5,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_2.png",ATTACK_2,15, 7,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Jump.png",JUMP,12, 9,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Eating.png",EAT,6, 8,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Dead.png",DEAD,8, 4,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Hurt.png",HURT,4, 1,0));

    // Ship
    Hero ship = loadHero();
    ship.pos.x = 300;
    ship.pos.y=SCREEN_HEIGHT-200;
    ship.type = SHIP;
    addHeroAnim(&ship,LoadAnim("resources/images/Ship3/Ship3.png",IDLE,1,0,1));
    addHeroAnim(&ship,LoadAnim("resources/images/Ship3/Ship3_Explosion.png",DEAD,8,10,0));

    // Ship engine
    Anim shipEngine = LoadAnim("resources/images/Ship3/Exhaust/exhaust1.png",IDLE,8,3,1);

    // Left button
    Button btnLeft;
    Image imgLeft = LoadImage("resources/images/Btn/Backward_BTN.png");
    btnLeft.texture = LoadTextureFromImage(imgLeft);
    Image imgLeftAct = LoadImage("resources/images/Btn_Active/Backward_BTN.png");
    btnLeft.textureActive = LoadTextureFromImage(imgLeftAct);
    btnLeft.pos.x=15;
    btnLeft.pos.y=SCREEN_HEIGHT-52;
    btnLeft.isActive=0;
    UnloadImage(imgLeft);
    UnloadImage(imgLeftAct);

    // Right button
    Button btnRight;
    Image imgRight = LoadImage("resources/images/Btn/Forward_BTN.png");
    btnRight.texture = LoadTextureFromImage(imgRight);
    Image imgRightAct = LoadImage("resources/images/Btn_Active/Forward_BTN.png");
    btnRight.textureActive = LoadTextureFromImage(imgRightAct);
    btnRight.pos.x=80;
    btnRight.pos.y=SCREEN_HEIGHT-52;
    btnRight.isActive=0;
    UnloadImage(imgRight);
    UnloadImage(imgRightAct);

    // Box
    Button btnHit;
    Image imgHit = LoadImage("resources/images/BtnHit/Bomb_03.png");
    btnHit.texture = LoadTextureFromImage(imgHit);
    Image imgHitAct = LoadImage("resources/images/BtnHit/Bomb_02.png");
    btnHit.textureActive = LoadTextureFromImage(imgHitAct);
    btnHit.pos.x=110;
    btnHit.pos.y=SCREEN_HEIGHT-82;
    btnHit.isActive=0;
    UnloadImage(imgHit);
    UnloadImage(imgHitAct);

    // Meteors
    meteorsCount=0;

    Anim idle[10];
    idle[0] = LoadAnim("resources/images/Meteors/Meteor_01.png",IDLE,1,0,1);
    idle[1] = LoadAnim("resources/images/Meteors/Meteor_02.png",IDLE,1,0,1);
    idle[2] = LoadAnim("resources/images/Meteors/Meteor_03.png",IDLE,1,0,1);
    idle[3] = LoadAnim("resources/images/Meteors/Meteor_04.png",IDLE,1,0,1);
    idle[4] = LoadAnim("resources/images/Meteors/Meteor_05.png",IDLE,1,0,1);
    idle[5] = LoadAnim("resources/images/Meteors/Meteor_06.png",IDLE,1,0,1);
    idle[6] = LoadAnim("resources/images/Meteors/Meteor_07.png",IDLE,1,0,1);
    idle[7] = LoadAnim("resources/images/Meteors/Meteor_08.png",IDLE,1,0,1);
    idle[8] = LoadAnim("resources/images/Meteors/Meteor_09.png",IDLE,1,0,1);
    idle[9] = LoadAnim("resources/images/Meteors/Meteor_10.png",IDLE,1,0,1);

    Anim dead = LoadAnim("resources/images/Meteors/explosion.png",DEAD,20,10,0);

    float x=0,y=0;
    for (int i=0; i<10; i++)
    {
        x = 170 + 50*i;
        y=80;
        if (x>=SCREEN_WIDTH)
        {
            x=x-SCREEN_WIDTH+170;
            y+=60;
        }

        addMeteor(x,y,0,.1,5,idle[rand()%10],dead);
    }

    // Projectiles
    animIdle= LoadAnim("resources/images/Ship3/Shot3/idle.png",IDLE,5,2,0);
    animDead = LoadAnim("resources/images/Ship3/Shot3/explode.png",DEAD,7,3,0);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        dt = GetFrameTime();
        timeElapsed+=dt;

//        printf("Meteors : %d, Projectiles : %d\n",meteorsCount,projectilesCount);

        // Left button
        btnLeft.isActive = isColliding(btnLeft.pos.x,btnLeft.pos.y,btnLeft.texture.width,btnLeft.texture.height,
                                       kunoichi.pos.x+60,kunoichi.pos.y+109,20,20);
        if(btnLeft.isActive==1)
        {
            ship.velocity.x=-1;
        }

        // Right button
        btnRight.isActive = isColliding(btnRight.pos.x,btnRight.pos.y,btnRight.texture.width,btnRight.texture.height,
                                        kunoichi.pos.x+50,kunoichi.pos.y+109,20,20);
        if (btnRight.isActive==1)
        {
            ship.velocity.x=1;
        }

        if (btnRight.isActive==0 && btnLeft.isActive==0)
            ship.velocity.x=0;

        // Hit button
        if (kunoichi.pos.x>=-12 && kunoichi.flipX==1)
        {
            if (kunoichi.state == ATTACK_1)
            {
                if (kunoichi.currentAnim.currentFrame == 3 )
                {
                    btnHit.isActive=1;
                    ship.velocity.y=-1;
                    ship.isEngineOn = 1;
                    engineTimer=0;
                }
                else
                {
                    btnHit.isActive=0;
                }
            }
            if (kunoichi.state == ATTACK_2)
            {
                if (kunoichi.currentAnim.currentFrame == 5 && btnHit.isActive==0 )
                {
                    btnHit.isActive=1;
                    addProjetile(ship.pos.x+28,ship.pos.y-20,-4);
                }
                else
                {
                    btnHit.isActive=0;
                }
            }
        }



        updateHero(&kunoichi);

        // Ship
        if (ship.isEngineOn)
            engineTimer+=dt;
        if (engineTimer>=.4)
        {
            ship.isEngineOn = 0;
        }

        updateHero(&ship);
        shipEngine.pos.x = ship.pos.x+23;
        shipEngine.pos.y = ship.pos.y+64;
        updateAnim(&shipEngine);

        // Meteors
        updateMeteors();

        // Projectiles
        updateListProjectiles();


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Background
        if (currentBG==1)
            DrawTexture(bgStarfield,0,0,WHITE);
        else
            DrawTexture(bgPurple,0,0,WHITE);

        // Time elapsed
        DrawText("Time elapsed : ",13,60,20,WHITE);

        sprintf(&timeText,"%.0f s",  timeElapsed);
        DrawText(timeText,60,100,20,WHITE);

        // Left button
        if (btnLeft.isActive==0)
            DrawTexture(btnLeft.texture,btnLeft.pos.x,btnLeft.pos.y,WHITE);
        else
            DrawTexture(btnLeft.textureActive,btnLeft.pos.x,btnLeft.pos.y,WHITE);

        // Right button
        if (btnRight.isActive==0)
            DrawTexture(btnRight.texture,btnRight.pos.x,btnRight.pos.y,WHITE);
        else
            DrawTexture(btnRight.textureActive,btnRight.pos.x,btnRight.pos.y,WHITE);

        // Hit button
        if (btnHit.isActive==0)
            DrawTexture(btnHit.texture,btnHit.pos.x,btnHit.pos.y,WHITE);
        else
            DrawTexture(btnHit.textureActive,btnHit.pos.x,btnHit.pos.y,WHITE);



        // Hero
        drawHero(kunoichi);
        drawHero(ship);
        if(ship.isEngineOn==1)
            drawAnim(shipEngine);

        // Projectiles

        // Meteors
        drawMeteors();

        // Projectiles
        drawListProjectiles();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    unloadHero(kunoichi);
    unloadHero(ship);
    unloadAnim(shipEngine);

    UnloadTexture(bgPurple);
    UnloadTexture(bgStarfield);
    UnloadTexture(btnLeft.texture);
    UnloadTexture(btnLeft.textureActive);
    UnloadTexture(btnRight.texture);
    UnloadTexture(btnRight.textureActive);
    UnloadTexture(btnHit.texture);
    UnloadTexture(btnHit.textureActive);
    unloadMeteors();


    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
