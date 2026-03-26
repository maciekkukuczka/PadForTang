#include "Pad.h"

Pad::Pad():xboxController(XBOX_CONTROLLER_MAC){};

bool Pad::Connected()
{
    if(xboxController.isConnected()) return true;
    return false;
}

void Pad::StartControllerRadar()
{
    xboxController.begin();
}

void Pad::ControllerUpdate(){
    xboxController.onLoop();
}


// Buttons

bool Pad::UpPressed()
{
    if (xboxController.xboxNotif.btnDirUp) return true;
    return false;
}
bool Pad::DownPressed()
{
    if (xboxController.xboxNotif.btnDirDown) return true;
    return false;
}
bool Pad::LeftPressed()
{
    if (xboxController.xboxNotif.btnDirLeft) return true;
    return false;
}
bool Pad::RightPressed()
{
    if (xboxController.xboxNotif.btnDirRight) return true;
    return false;
}


bool Pad::StartPressed()
{
    if (xboxController.xboxNotif.btnStart) return true;
    return false;
}

bool Pad::SelectPressed()
{
    if (xboxController.xboxNotif.btnSelect) return true;
    return false;
}


