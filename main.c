#include "raylib.h"
#include "anim.h"
#include "hero.h"
#include "projectile.h"
#include "constants.h"
#include <time.h>

typedef enum
{
    MENU,
    GAMEPLAY,
    CREDITS
} Scene;

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

int projectilesCount, meteorsCount, meteorsCountBackup,gameWon;
Anim animIdle, animDead;


Projectile listProjectiles[MAX_PROJECTILES];
Hero listMeteorsBackup[MAX_METEORS],listMeteors[MAX_METEORS], ship;
Scene gameScene;

Sound sfxClick ;
Sound sfxShoot ;
Sound sfxVictory;
Sound sfxExplosion ;
Sound sfxShipExplosion ;
int isClickPlaying,isVictoryPlaying,isShootPlaying,isExplosionPlaying,isShipExplosionPlaying;

void addProjetile(float pX,float pY,float pVy)
{
    if(projectilesCount<MAX_PROJECTILES)
    {
        PlaySound(sfxShoot);
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
            if (listProjectiles[i].del==0 && listMeteors[m].energy>0 &&
                    isColliding(listProjectiles[i].pos.x, listProjectiles[i].pos.y, listProjectiles[i].idleAnim.frameRec.width/1.5,
                                listProjectiles[i].idleAnim.frameRec.height,
                                listMeteors[m].pos.x,listMeteors[m].pos.y,listMeteors[m].currentAnim.frameRec.width/1.5,
                                listMeteors[m].currentAnim.frameRec.height/8))
            {
                listProjectiles[i].del = 1;
                if (listMeteors[m].energy>0)
                {
                    listMeteors[m].energy-=.5;
                }

                listProjectiles[i].velocity.y=0;
            }
        }
        updateProjectile(&listProjectiles[i]);
        if((listProjectiles[i].del ==1 && listProjectiles[i].deadAnim.ended==1) || listProjectiles[i].pos.y< -100)
        {
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
        if (ship.energy> 0 && listMeteors[i].energy>0 &&  isColliding(ship.pos.x, ship.pos.y, ship.currentAnim.frameRec.width/1.5,
                ship.currentAnim.frameRec.height,
                listMeteors[i].pos.x,listMeteors[i].pos.y,listMeteors[i].currentAnim.frameRec.width/1.5,
                listMeteors[i].currentAnim.frameRec.height/2.5))
        {

            if (listMeteors[i].energy>0)
            {
                listMeteors[i].energy-=2;
            }

            if(ship.energy>0)
            {
                ship.energy-=.5;
                ship.velocity.y=1;
                ship.pos.y+=3;
            }
        }

        if(gameWon)
            listMeteors[i].velocity.y=0;

        updateHero(&listMeteors[i]);
        if((listMeteors[i].state==DEAD && listMeteors[i].currentAnim.ended==1)|| listMeteors[i].pos.y>SCREEN_HEIGHT+20 )
        {
            PlaySound(sfxExplosion);
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
    char timeText[10]="",energyText[10]="";

    InitWindow(screenWidth, screenHeight, GAME_TITLE);
    InitAudioDevice();

    gameScene = MENU;
    projectilesCount=0;
    int exitGame=0;
    gameWon=0;

    // Menu buttons
    Vector2 mousePos;

    // Start
    Button btnStart;
    Image imgStart = LoadImage("resources/images/Gui/Start_BTN.png");
    btnStart.texture = LoadTextureFromImage(imgStart);
    UnloadImage(imgStart);
    btnStart.pos.x = 150;
    btnStart.pos.y= 400;
    btnStart.isActive=0;

    // Exit
    Button btnExit;
    Image imgExit = LoadImage("resources/images/Gui/Exit_BTN.png");
    btnExit.texture = LoadTextureFromImage(imgExit);
    UnloadImage(imgExit);
    btnExit.pos.x = 150;
    btnExit.pos.y= 490;
    btnExit.isActive=0;

    // Credits
    Button btnCredits;
    Image imgCredits = LoadImage("resources/images/Gui/Info_BTN.png");
    btnCredits.texture = LoadTextureFromImage(imgCredits);
    UnloadImage(imgCredits);
    btnCredits.pos.x = 220;
    btnCredits.pos.y= 590;
    btnCredits.isActive=0;

    char textCredits[200] = "\tWere involved in this jam : \n\t\t\tCode : senor16\n\t\t\tMusic: Kneezus\n\n\t\tThanks for Playing :)";
    int framesCounter = 0;

    // Sfx
    sfxClick = LoadSound("resources/sfx/click2.ogg");
    sfxShoot = LoadSound("resources/sfx/shoot.mp3");
    sfxVictory = LoadSound("resources/sfx/victory.mp3");
    sfxExplosion = LoadSound("resources/sfx/Explosion.mp3");
    sfxShipExplosion = LoadSound("resources/sfx/ShipExplosion.mp3");

    isClickPlaying=0;
    isVictoryPlaying=0;
    isShootPlaying=0;
    isExplosionPlaying=0;
    isShipExplosionPlaying=0;

    // Musics
    Music mscCredits = LoadMusicStream("resources/musics/credits music.mp3");
    mscCredits.looping = true;
    int isCreditsPlaying = 0;
    Music mscGameplay = LoadMusicStream("resources/musics/gameplay music.mp3");
    mscGameplay.looping=true;
    int isGameplayPlaying=0;
    Music mscMenu = LoadMusicStream("resources/musics/menu music.mp3");
    mscMenu.looping = true;
    int isMenuPlaying  = 0;

    // Victory / Failure img
    Image imgwin = LoadImage("resources/images/Gui/You_Win.png");
    Texture txtWin = LoadTextureFromImage(imgwin);
    Image imgFailed = LoadImage("resources/images/Gui/You_Lose.png");
    Texture txtFailed = LoadTextureFromImage(imgFailed);



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
    ship = loadHero();
    ship.pos.x = 300;
    ship.pos.y=SCREEN_HEIGHT-200;
    ship.type = SHIP;
    ship.energy=5;
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

    float x=165,y=0;
    for (int i=0; i<14; i++)
    {
        if (x>=SCREEN_WIDTH)
        {
            x=x-SCREEN_WIDTH+170;
            y+=60;
        }
        addMeteor(x,y,0,.1,rand()%3+2,idle[rand()%10],dead);
        x += 50;
    }

    for(int i=0; i<meteorsCount; i++)
    {
        listMeteorsBackup[i] = listMeteors[i];
    }

    meteorsCountBackup= meteorsCount;

    // Projectiles
    animIdle= LoadAnim("resources/images/Ship3/Shot3/idle.png",IDLE,5,2,0);
    animDead = LoadAnim("resources/images/Ship3/Shot3/explode.png",DEAD,7,3,0);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose() && exitGame==0) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (gameScene == MENU)
        {
            if(isMenuPlaying==0)
                PlayMusicStream(mscMenu);

            UpdateMusicStream(mscMenu);
            // Check mouse click
            if(IsMouseButtonPressed(0))
            {
                mousePos = GetMousePosition();
                // Exit the game
                if(isColliding(mousePos.x,mousePos.y,2,2,btnExit.pos.x,btnExit.pos.y,btnExit.texture.width,btnExit.texture.height))
                {
                    exitGame = true;
                }

                // Open Credits
                if(isColliding(mousePos.x,mousePos.y,1,1,
                               btnCredits.pos.x,btnCredits.pos.y,btnCredits.texture.width,btnCredits.texture.height))
                {
                    gameScene = CREDITS;
                    PlaySound(sfxClick);
                }

                // Start game
                if(isColliding(mousePos.x,mousePos.y,1,1,
                               btnStart.pos.x,btnStart.pos.y,btnStart.texture.width,btnStart.texture.height))
                {
                    gameScene = GAMEPLAY;
                    PlaySound(sfxClick);
                }
            }
        }
        if (gameScene == CREDITS)
        {
            framesCounter++;
            if (isMenuPlaying==1)
                StopMusicStream(mscMenu);
            if(isCreditsPlaying==0)
                PlayMusicStream(mscCredits);


            UpdateMusicStream(mscCredits);
            // Check mouse click
            if(IsMouseButtonPressed(0))
            {
                mousePos = GetMousePosition();
                if(isColliding(mousePos.x,mousePos.y,1,1,
                               btnStart.pos.x,btnStart.pos.y+150,btnStart.texture.width,btnStart.texture.height))
                {
                    PlaySound(sfxClick);
                    gameScene = GAMEPLAY;
                }
            }
        }
        if (gameScene == GAMEPLAY)
        {
            if (isMenuPlaying==1)
                StopMusicStream(mscMenu);
            if (isCreditsPlaying==1)
                StopMusicStream(mscCredits);
            if(isGameplayPlaying==0)
                PlayMusicStream(mscGameplay);

            UpdateMusicStream(mscGameplay);

            dt = GetFrameTime();
            if (kunoichi.state!= DEAD && !gameWon )
            {
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
            }
            // Meteors
            updateMeteors();


            // Hero
            updateHero(&kunoichi);
            if ((ship.pos.y>SCREEN_HEIGHT || ship.state==DEAD)&& !gameWon)
            {
                kunoichi.energy=0;
                ship.energy=0;
            }

            // Ship

            if (ship.isEngineOn)
                engineTimer+=dt;
            if (engineTimer>=.4)
            {
                ship.isEngineOn = 0;
            }
            if(gameWon)
                ship.velocity.y=0;
            updateHero(&ship);
            shipEngine.pos.x = ship.pos.x+23;
            shipEngine.pos.y = ship.pos.y+64;

            updateAnim(&shipEngine);

            // Projectiles
            updateListProjectiles();

            if(gameWon){
                  if(isVictoryPlaying==0)
                {
                    PlaySound(sfxVictory);
                    isVictoryPlaying=1;
                }
            }

            if(ship.state==DEAD)
            {
                if(isShipExplosionPlaying==0)
                {
                    PlaySound(sfxShipExplosion);
                    isShipExplosionPlaying=1;
                }
            }

            // Restart the game
            if(ship.state==DEAD || gameWon)
            {
                // Check mouse click
                if(IsMouseButtonPressed(0))
                {
                    mousePos = GetMousePosition();
                    if(isColliding(mousePos.x,mousePos.y,1,1,
                                   btnStart.pos.x+60,btnStart.pos.y+150,btnStart.texture.width,btnStart.texture.height))
                    {
                        StopSound(sfxVictory);
                        PlaySound(sfxClick);
                        gameScene = GAMEPLAY;
                        // Reset Ship
                        ship.pos.x = 300;
                        ship.pos.y=SCREEN_HEIGHT-200;
                        ship.energy=5+1.5;
                        ship.state=IDLE;
                        updateHero(&ship);

                        // Reset Hero
                        kunoichi.energy=5;
                        kunoichi.state=IDLE;

                        // Reset game state
                        timeElapsed=0;
                        gameWon=0;

                        // Reset projectile
                        projectilesCount=0;

                        // Reset meteors
                        for (int m=0; m<MAX_METEORS; m++)
                        {
                            listMeteors[m] = listMeteorsBackup[m];
                        }
                        meteorsCount = meteorsCountBackup;


                    }
                }
            }
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameScene == MENU)
        {
            DrawTexture(bgPurple,0,0,WHITE);
            DrawText("Space Ascend",50,130,60,WHITE);
            DrawText("Made for the Zeno Jam 6",150,200,20,WHITE);

            DrawTexture(btnStart.texture,btnStart.pos.x,btnStart.pos.y,WHITE);
            DrawTexture(btnCredits.texture,btnCredits.pos.x,btnCredits.pos.y,WHITE);
            DrawTexture(btnExit.texture,btnExit.pos.x,btnExit.pos.y,WHITE);

        }
        if (gameScene == CREDITS)
        {
            DrawTexture(bgPurple,0,0,WHITE);
            DrawText("Space Ascend",50,50,60,WHITE);
            DrawText("Made for the Zeno Jam 6",150,120,20,WHITE);
            DrawText(TextSubtext(textCredits, 0, framesCounter/5),50,250,28,WHITE);


            DrawTexture(btnStart.texture,btnStart.pos.x,btnStart.pos.y+150,WHITE);
        }
        if (gameScene == GAMEPLAY)
        {

            // Background
            if (currentBG==1)
                DrawTexture(bgStarfield,0,0,WHITE);
            else
                DrawTexture(bgPurple,0,0,WHITE);

            // Time elapsed
            DrawText("Time elapsed : ",13,110,20,WHITE);
            sprintf(&timeText,"%.0f s",  timeElapsed);
            DrawText(timeText,60,150,20,WHITE);

            // Ship energy
            DrawText("Energy : ",50,200,20,WHITE);
            sprintf(&energyText,"%.2f s",  ship.energy);
            DrawText(energyText,60,240,20,WHITE);

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
            if (ship.energy>0 || (ship.state==DEAD && ship.currentAnim.ended==0))
                drawHero(ship);
            if(ship.isEngineOn==1)
                drawAnim(shipEngine);


            // Meteors
            drawMeteors();

            // Projectiles
            drawListProjectiles();

            if(meteorsCount<=0)
            {
                gameWon=1;
            }


            // Draw Game Over
            if(ship.state==DEAD)
            {
                DrawTexture(txtFailed,70,300,WHITE);
                DrawTexture(btnStart.texture,btnStart.pos.x+60,btnStart.pos.y+150,WHITE);
            }
            if(gameWon)
            {
                DrawTexture(txtWin,100,300,WHITE);
                DrawTexture(btnStart.texture,btnStart.pos.x+60,btnStart.pos.y+150,WHITE);
            }

        }
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
    UnloadTexture(btnStart.texture);
    UnloadTexture(btnCredits.texture);
    UnloadTexture(btnExit.texture);
    unloadMeteors();

    UnloadMusicStream(mscCredits);
    UnloadMusicStream(mscGameplay);
    UnloadMusicStream(mscMenu);

    UnloadSound(sfxClick);
    UnloadSound(sfxExplosion);
    UnloadSound(sfxShipExplosion);
    UnloadSound(sfxVictory);
    UnloadSound(sfxShoot);


    CloseAudioDevice();

    CloseWindow(); // Close window and OpenGL context
//--------------------------------------------------------------------------------------

    return 0;
}
