#include <cstdint>

#include <raylib/raylib.h>
#include <raylib/raymath.h>

#include <fabrik.hpp>

int main()
{
    InitWindow(1800, 800, "FABRIK");
    InitAudioDevice();

    Fabrik fabrik;

    Camera2D cam = {0};
    cam.offset = {900, 400};
    cam.zoom = 1;

    fabrik.AddRoot({0, 0}, {90, 0});
    uint32_t effector1 = fabrik.AddBone({180, 0});
    uint32_t effector2 = fabrik.AddBone({200, 0});

    fabrik.SetMinTheta(effector1, -180);
    fabrik.SetMaxTheta(effector1, 0);

    fabrik.SetMinTheta(effector2, -45);
    fabrik.SetMaxTheta(effector2, 45);

    fabrik.SetThreshold(2);
    fabrik.SetIterationLimit(20);
    fabrik.SetIterationThreshold(0.1);

    Vector2 target1;
    Vector2 target2 = {-200, 0};

    float dir = 1;

    float lastTime = GetTime();
    float deltaTime = 0;

    while(!WindowShouldClose())
    {
        deltaTime = GetTime()-lastTime;
        if(deltaTime < 1.f/60)
        {
            WaitTime(1.f/60-deltaTime);
            deltaTime = GetTime()-lastTime;
        }
        lastTime = GetTime();

        PollInputEvents();

        Vector2 mousePos = GetMousePosition();
        mousePos = GetScreenToWorld2D(mousePos, cam);

        target2.x += dir*50*deltaTime;
        if(target2.x > 200)
        {
            target2.x = 200;
            dir = -1;
        }
        if(target2.x < -200)
        {
            target2.x = -200;
            dir = 1;
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            target1 = mousePos;
            fabrik.Solve({effector1, effector2}, {target1, target2}, {false, false});
        }
        else
        {
            fabrik.Solve({effector2}, {target2}, {false});
        }

        ClearBackground(BLACK);
        BeginMode2D(cam);

        Vector2 start = fabrik.GetBasePosition();
        float thetaGlobal = fabrik.GetBaseTheta();
        uint32_t curr = fabrik.GetRoot();
        while(curr != 0)
        {
            float length = fabrik.GetLength(curr);
            float theta = fabrik.GetTheta(curr);

            thetaGlobal += theta;
            DrawRectanglePro({start.x, start.y, length, 5}, {0, 2.5f}, thetaGlobal, {255, 255, 255, 100});

            start += Vector2Rotate(Vector2{1, 0}, DEG2RAD*thetaGlobal)*length;
            curr = fabrik.GetNextBone(curr);
        }

        DrawCircleV(mousePos, 5, {0, 0, 255, 100});
    
        EndMode2D();

        SwapScreenBuffer();
    }

    CloseAudioDevice();
    CloseWindow();
}