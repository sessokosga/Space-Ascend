#include "raylib.h"
#include "anim.h"
#include "hero.h"
#include "projectile.h"
#include "constants.h"

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

int projectileCount;

Projectile listProjectiles[MAX_PROJECTILE];

void addProjetile(Image pImg,float pX,float pY)
{

    listProjectiles[projectileCount++]= loadProjectile(pX,pY,LoadTextureFromImage(pImg));
}



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    float dt;

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
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Eating.png",EAT,8, 8,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Dead.png",DEAD,8, 4,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Hurt.png",HURT,4, 1,0));

    // Ship
    Image imgProj = LoadImage("resources/images/Ship3/Ship3.png");
    Projectile proj =  loadProjectile(300,380,LoadTextureFromImage(imgProj));
    proj.velocity.y=0;

    // Left button
    Button btnLeft;
    Image imgLeft = LoadImage("resources/images/Btn/Backward_BTN.png");
    btnLeft.texture = LoadTextureFromImage(imgLeft);
    Image imgLeftAct = LoadImage("resources/images/Btn_Active/Backward_BTN.png");
    btnLeft.textureActive = LoadTextureFromImage(imgLeftAct);
    btnLeft.pos.x=10;
    btnLeft.pos.y=460;
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
    btnRight.pos.y=460;
    btnRight.isActive=0;
    UnloadImage(imgRight);
    UnloadImage(imgRightAct);



    // Box
    Hero hitBox = loadHero();
    hitBox.pos.x=120;
    hitBox.pos.y=430;
    hitBox.type = HITBOX;
    addHeroAnim(&hitBox,LoadAnim("resources/images/Bomb_03.png",IDLE,1,0,0));
    addHeroAnim(&hitBox,LoadAnim("resources/images/Bomb_02.png",HURT,1,0,0));

    //playHeroAnim(&kunoichi,IDLE);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        dt = GetFrameTime();

        // Left button
        btnLeft.isActive = isColliding(btnLeft.pos.x,btnLeft.pos.y,btnLeft.texture.width,btnLeft.texture.height,
                kunoichi.pos.x+60,kunoichi.pos.y+109,20,20);

        // Right button
        btnRight.isActive = isColliding(btnRight.pos.x,btnRight.pos.y,btnRight.texture.width,btnRight.texture.height,
                kunoichi.pos.x+50,kunoichi.pos.y+109,20,20);

        updateHero(&kunoichi,dt);
        updateHero(&hitBox,dt);
        updateProjectile(&proj);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Background
        if (currentBG==1)
            DrawTexture(bgStarfield,0,0,WHITE);
        else
            DrawTexture(bgStarfield,0,0,WHITE);

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

        // Hero
        drawHero(kunoichi);

        // Box
        drawHero(hitBox);

        // Projectiles
        drawProjectile(proj);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    unloadHero(kunoichi);
    unloadHero(hitBox);
    unloadProjectile(proj);
    UnloadTexture(bgPurple);
    UnloadTexture(bgStarfield);
    UnloadTexture(btnLeft.texture);
    UnloadTexture(btnLeft.textureActive);
    UnloadTexture(btnRight.texture);
    UnloadTexture(btnRight.textureActive);


    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
