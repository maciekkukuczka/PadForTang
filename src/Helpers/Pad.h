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

bool APressed();
bool BPressed();
bool XPressed();
bool YPressed();

bool L1Pressed();
bool R2Pressed();
bool L2Pressed();
bool R1Pressed();


bool StartPressed();
bool SelectPressed();
bool SharePressed();
bool XBoxPressed();


void StartControllerRadar();
void ControllerUpdate();

private:

};

