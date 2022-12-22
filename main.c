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

int projectileCount, meteorsCount;

Projectile listProjectiles[MAX_PROJECTILES];
Hero listMeteors[MAX_METEORS];

void addProjetile(Image pImg,float pX,float pY)
{
    if(projectileCount<MAX_PROJECTILES)
        listProjectiles[projectileCount++]= loadProjectile(pX,pY,LoadTextureFromImage(pImg));
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
    for (int i=0; i<meteorsCount; i++)
    {
        updateHero(&listMeteors[i]);
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
    unloadHero(listMeteors[num]);
    for (int i=num; i<meteorsCount-1; i++)
    {
        listMeteors[i]=listMeteors[i+1];
    }
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

    projectileCount=0;

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

    char *idle[10];
    idle[0] = "resources/images/Meteors/Meteor_01.png";
    idle[1] = "resources/images/Meteors/Meteor_02.png";
    idle[2] = "resources/images/Meteors/Meteor_03.png";
    idle[3] = "resources/images/Meteors/Meteor_04.png";
    idle[4] = "resources/images/Meteors/Meteor_05.png";
    idle[5] = "resources/images/Meteors/Meteor_06.png";
    idle[6] = "resources/images/Meteors/Meteor_07.png";
    idle[7] = "resources/images/Meteors/Meteor_08.png";
    idle[8] = "resources/images/Meteors/Meteor_09.png";
    idle[9] = "resources/images/Meteors/Meteor_10.png";

    float x=0,y=0;
    for (int i=0; i<10; i++)
    {
        x = 170 + 50*i;
        y=80;
        if (x>=SCREEN_WIDTH){
           x=x-SCREEN_WIDTH+170;
           y+=60;
        }

        addMeteor(x,y,0,.1,5,LoadAnim(idle[rand()%10],IDLE,1,0,1),
                  LoadAnim("resources/images/Meteors/explosion.png",DEAD,20,10,0));
    }


    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        dt = GetFrameTime();
        timeElapsed+=dt;

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
        if (kunoichi.pos.x>=-12)
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
                if (kunoichi.currentAnim.currentFrame == 5 )
                {
                    btnHit.isActive=1;
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
        printf("Kunoichi x : %f\n",kunoichi.pos.x);


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
