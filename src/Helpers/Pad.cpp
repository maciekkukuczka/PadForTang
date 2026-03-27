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

bool Pad::APressed()
{
    if (xboxController.xboxNotif.btnA)return true;
    return false;
}
bool Pad::BPressed()
{
    if (xboxController.xboxNotif.btnB)return true;
    return false;
}
bool Pad::XPressed()
{
    if (xboxController.xboxNotif.btnX)return true;
    return false;
}
bool Pad::YPressed()
{
    if (xboxController.xboxNotif.btnY)return true;
    return false;
}


bool Pad::L1Pressed()
{
    if (xboxController.xboxNotif.btnLB)return true;
    return false;
}
bool Pad::L2Pressed()
{
    if (xboxController.xboxNotif.trigLT)return true;
    return false;
}
bool Pad::R1Pressed()
{
    if (xboxController.xboxNotif.btnRB)return true;
    return false;
}
bool Pad::R2Pressed()
{
    if (xboxController.xboxNotif.trigRT)return true;
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
bool Pad::SharePressed()
{
    if (xboxController.xboxNotif.btnShare) return true;
    return false;
}

bool Pad::XBoxPressed()
{
    if (xboxController.xboxNotif.btnXbox) return true;
    return false;
}


