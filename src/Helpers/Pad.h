#pragma once
#include "Config.h"
#include <XboxSeriesXControllerESP32_asukiaaa.hpp> // Biblioteka do obsługi kontrolera Xbox Series X


class Pad
{
public:
Pad();
XboxSeriesXControllerESP32_asukiaaa::Core xboxController;
bool padWasConnected = false;
bool Connected();
bool UpPressed();
bool DownPressed();
bool LeftPressed();
bool RightPressed();
bool StartPressed();
bool SelectPressed();

void StartControllerRadar();
void ControllerUpdate();

private:

};

