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
    kunoichi.pos.x=100;
    kunoichi.pos.y=300;
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Run.png",RUN,8, 7,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Idle.png",IDLE,8, 8,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_1.png",ATTACK_1,10, 5,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_2.png",ATTACK_2,15, 7,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Jump.png",JUMP,12, 9,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Eating.png",EAT,8, 8,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Dead.png",DEAD,8, 4,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Hurt.png",HURT,4, 1,0));

    //Image imgProj = LoadImage("resources/images/Kunoichi/Spine.png");
    //Projectile proj =  loadProjectile(10,20,LoadTextureFromImage(imgProj));
    //proj.velocity.x=2;


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
        //updateProjectile(&proj);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);


        drawHero(kunoichi);
        //drawProjectile(proj);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    unloadHero(kunoichi);
//    unloadProjectile(proj);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
