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
    const int screenWidth = 800;
    const int screenHeight = 450;
    float dt;

    InitWindow(screenWidth, screenHeight, "Learning Raylib");

    projectileCount=0;

    Hero kunoichi = loadHero();
    kunoichi.pos.x=100;
    kunoichi.pos.y=200;
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Run.png",RUN,8, 7,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Idle.png",IDLE,8, 8,1));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_1.png",ATTACK_1,10, 5,0));
    addHeroAnim(&kunoichi,LoadAnim("resources/images/Kunoichi/Attack_2.png",ATTACK_2,15, 7,0));

    Image imgProj = LoadImage("resources/images/Kunoichi/Spine.png");
    Projectile proj =  loadProjectile(10,20,LoadTextureFromImage(imgProj));
    proj.velocity.x=2;


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
        updateProjectile(&proj);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);


        drawHero(kunoichi);
        drawProjectile(proj);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    unloadHero(kunoichi);
    unloadProjectile(proj);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
