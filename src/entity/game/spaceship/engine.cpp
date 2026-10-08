#include "engine.h"
#include "ship.h"
#include "background.h"


static Ship goPlayer;
static Background goBG;



namespace SpaceshipGame
{

    void Engine::Update()
    {
        if (!inited) this->Awake();

        for (int i = 0; i < countObjs; ++i)
            gObjects[i]->Update();

        this->Draw(); // each obj draws into leds
    }



    void Engine::Awake()
    {
        // connect settings for singletons
        goBG.pStg = &s.bg;
        // make the arrays, fill counts
        gObjects[0] = &goBG;
        gObjects[1] = &goPlayer;
        countObjs = 2;

        for (int i = 0; i < countObjs; ++i)
            gObjects[i]->Awake();

        inited = true;
    }



    void Engine::Draw()
    {
        if (pLeds == nullptr) return;

        for (int i = 0; i < countObjs; ++i)
        {
            //if (i == 1) continue;  // delete, don't draw player
            gObjects[i]->Draw(pLeds);
        }
    }

}