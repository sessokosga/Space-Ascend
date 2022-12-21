#include "raylib.h"
#include "anim.h"
#include "hero.h"
#include "projectile.h"
#include "constants.h"

int projectileCount;

Projectile listProjectiles[MAX_PROJECTILE];

void addProjetile(Image pImg,float pX,float pY){

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

    Hero kunoichi = loadHero();
    kunoichi.pos.x=30;
    kunoichi.pos.y=300;
    kunoichi.type= HERO;
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Run.png",RUN,8, 7,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Idle.png",IDLE,8, 8,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_1.png",ATTACK_1,10, 5,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_2.png",ATTACK_2,15, 7,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Jump.png",JUMP,12, 9,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Eating.png",EAT,8, 8,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Dead.png",DEAD,8, 4,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Hurt.png",HURT,4, 1,0));

    Image imgProj = LoadImage("resources/images/Ship3/Ship3.png");
    Projectile proj =  loadProjectile(300,380,LoadTextureFromImage(imgProj));
    proj.velocity.y=0;

    Hero hitBox = loadHero();
    hitBox.pos.x=130;
    hitBox.pos.y=370;
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

        updateHero(&kunoichi,dt);
        updateHero(&hitBox,dt);
        updateProjectile(&proj);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);


        drawHero(kunoichi);
        drawHero(hitBox);
        drawProjectile(proj);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    unloadHero(kunoichi);
    unloadHero(hitBox);
    unloadProjectile(proj);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
