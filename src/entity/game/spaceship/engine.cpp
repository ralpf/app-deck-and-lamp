#include "engine.h"
#include "ship.h"
#include "background.h"


static Ship goPlayer;
static Background goBG;



void SpaceshipGame::Update()
{
    if (!inited) this->Awake();

    for (int i = 0; i < countObjs; ++i)
        gObjects[i]->Update();

    this->Draw(); // each obj draws into leds
}




void SpaceshipGame::Awake()
{
    // make the arrays, fill counts
    gObjects[0] = &goBG;
    gObjects[1] = &goPlayer;
    countObjs = 2;

    for (int i = 0; i < countObjs; ++i)
        gObjects[i]->Awake();

    inited = true;
}



void SpaceshipGame::Draw()
{
    if (pLeds == nullptr) return;

    for (int i = 0; i < countObjs; ++i)
        gObjects[i]->Draw(pLeds);
}