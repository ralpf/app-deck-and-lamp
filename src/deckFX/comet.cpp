#include "comet.h"


void Comet::Update()
{
    if (!isRunning) return;

    ui32 nowMs = millis();
    ui32 elapsedMs = nowMs - lastUpdateMs;
    lastUpdateMs = nowMs;

    float direction = directionLeftToRight ? 1.0f : -1.0f;
    position += direction * speed * (elapsedMs * 0.001f);

    ui16 length = (ui16)headLenth + tailLength;
    bool isEmpty = length == 0;
    bool tailExitedRight = position >= leds.Count() + length - 1;
    bool tailExitedLeft = position <= -(float)length;
    bool hasExited = directionLeftToRight ? tailExitedRight : tailExitedLeft;

    if (isEmpty || hasExited)
    {
        isRunning = false;
        return;
    }

    leds.ScaleDown(backgroundScaleDown8);

    for (ui16 i = 0; i < headLenth; ++i)
        leds.OverlayColor(headColor, position - direction * i);

    for (ui16 i = 0; i < tailLength; ++i)
    {
        ui8 mix = tailLength > 1 ? i * 255 / (tailLength - 1) : 0;
        CRGB color = blend(tailStartColor, tailEndColor, mix);
        ui8 fade = (tailLength - i) * 255 / (tailLength + 1);
        color.nscale8_video(fade);
        leds.OverlayColor(color, position - direction * (headLenth + i));
    }
}


void Comet::StartOne()
{
    if (this->speed == 0) this->speed = 1;  // otherwise the bg dimm will never end
    position = directionLeftToRight ? -1.0f : (float)leds.Count();
    lastUpdateMs = millis();
    isRunning = true;
}
