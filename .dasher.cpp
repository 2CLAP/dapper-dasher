#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float runningTime;
    float updateTime;
};

AnimData animationUpdate(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;

        data.rec.x = data.frame * data.rec.width;
        data.frame++;

        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

int main() 
{
    const int windowWidth{512};
    const int windowHeight{380};
    InitWindow(windowWidth, windowHeight, "Dappy Dasher");

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    const int gravity{1000};

    float velocity{};

    int jumpVel{-600};

    int nebVel{-200};

    bool isInAir{false};

    bool collision{false};

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0.0;
    scarfyData.rec.y = 0.0;
    scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowHeight - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.runningTime = 0.0;
    scarfyData.updateTime = 1.0/12.0;

    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int sizeOfNebula{3};
    AnimData nebulae[sizeOfNebula];
    for (int i = 0; i < sizeOfNebula; i++)
    {
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].pos.x = windowWidth + i * 300;
        nebulae[i].pos.y = windowHeight - nebulae[i].rec.height;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
    }
    
    float finishLine = nebulae[sizeOfNebula - 1].pos.x;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT = GetFrameTime();

        if (!collision)
        {
            bgX += -40 * dT;
            if (bgX <= -background.width * 2)
            {
                bgX = 0;
            }
            mgX += -60 * dT;
            if (mgX <= -midground.width * 2)
            {
                mgX = 0;
            }
            fgX += -80 * dT;
            if (fgX <= -foreground.width * 2)
            {
                fgX = 0;
            }
        }
        
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2, WHITE);
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2, WHITE);
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2, WHITE);

        if (isOnGround(scarfyData, windowHeight))
        {
            velocity = 0;
            isInAir = false;
        } else {
            velocity += gravity * dT;
            isInAir = true;
        }
        
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity = jumpVel;
        }
        scarfyData.pos.y += velocity * dT;

        if (!isInAir)
        {
            scarfyData = animationUpdate(scarfyData, dT, 5);
        }
        
        if (!collision)
        {
            for (int i = 0; i < sizeOfNebula; i++)
            {
                nebulae[i].pos.x += nebVel * dT;
            }
        }
        
        for (int i = 0; i < sizeOfNebula; i++)
        {
            nebulae[i] = animationUpdate(nebulae[i], dT, 7);
        }
        
        for (AnimData nebula : nebulae)
        {
            const int pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad,
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height,
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }
        
        finishLine += nebVel * dT;

        if (collision)
        {
            DrawText("Game Over!", windowWidth/4, windowHeight/2, 50, WHITE);
        } else if (scarfyData.pos.x > finishLine + 200)
        {
            DrawText("You Win!", windowWidth/4, windowHeight/2, 50, WHITE);
        }
        
        for (int i = 0; i < sizeOfNebula; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
        
        if (!collision)
        {
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        
        EndDrawing();
    }
    UnloadTexture(background);

    CloseWindow();
}