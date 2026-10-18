// Copyright 2017 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DolphinQt/Config/Mapping/GCPadEmu.h"

#include <QGridLayout>
#include <QGroupBox>

#include "Core/HW/GCPad.h"
#include "Core/HW/GCPadEmu.h"

#include "InputCommon/InputConfig.h"

GCPadEmu::GCPadEmu(MappingWindow* window, SubType sub_type) : MappingWidget(window)
{
  CreateMainLayout(sub_type);
}

void GCPadEmu::CreateMainLayout(SubType sub_type)
{
  auto* layout = new QGridLayout;

  // Arcade (Triforce) cabinets are driven through this GameCube pad: say what each control does
  // in F-Zero AX, since the labels below are the GameCube ones.
  const bool arcade = sub_type == SubType::AMBaseboard;

  layout->addWidget(
      CreateGroupBox(arcade ? tr("Buttons: A = Boost, X = Left paddle, Y = Right paddle, "
                                 "Start = Start (B, Z unused)") :
                              tr("Buttons"),
                     Pad::GetGroup(GetPort(), PadGroup::Buttons)),
      0, 0);
  layout->addWidget(CreateGroupBox(arcade ? tr("D-Pad: Right / Left / Up / Down = Views 1 / 2 / 3 / 4") :
                                            tr("D-Pad"),
                                   Pad::GetGroup(GetPort(), PadGroup::DPad)),
                    1, 0);

  if (sub_type == SubType::AMBaseboard)
  {
    layout->addWidget(CreateGroupBox(tr("Triforce: Test, Service and Coin buttons"),
                                     Pad::GetGroup(GetPort(), PadGroup::Triforce)),
                      2, 0);
  }

  layout->addWidget(CreateGroupBox(arcade ? tr("Control Stick: X axis = Steering wheel") :
                                            tr("Control Stick"),
                                   Pad::GetGroup(GetPort(), PadGroup::MainStick)),
                    0, 1, -1, 1);
  layout->addWidget(CreateGroupBox(arcade ? tr("C Stick (unused by the game)") : tr("C Stick"),
                                   Pad::GetGroup(GetPort(), PadGroup::CStick)),
                    0, 2, -1, 1);
  layout->addWidget(CreateGroupBox(arcade ? tr("Triggers: L = Brake pedal, R = Accelerator pedal") :
                                            tr("Triggers"),
                                   Pad::GetGroup(GetPort(), PadGroup::Triggers)),
                    0, 4);
  layout->addWidget(CreateGroupBox(arcade ? tr("Rumble (unused: force feedback is in FZeroFFB.ini)") :
                                            tr("Rumble"),
                                   Pad::GetGroup(GetPort(), PadGroup::Rumble)),
                    1, 4);

  layout->addWidget(CreateGroupBox(tr("Options"), Pad::GetGroup(GetPort(), PadGroup::Options)), 2,
                    4);

  setLayout(layout);
}

void GCPadEmu::LoadSettings()
{
  Pad::LoadConfig();
}

void GCPadEmu::SaveSettings()
{
  Pad::GetConfig()->SaveConfig();
}

InputConfig* GCPadEmu::GetConfig()
{
  return Pad::GetConfig();
}
