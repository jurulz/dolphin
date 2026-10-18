// Copyright 2026 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <array>
#include <memory>

#include "Core/HW/Triforce/IOPorts.h"
#include "Core/HW/Triforce/SerialDevice.h"

namespace Triforce
{

// FFB wheel used by FZeroAX and FZeroAXMonster.
class FZeroAXSteeringWheel final : public SerialDevice
{
public:
  void Update() override;

  bool IsInitializing() const;

  s8 GetServoPosition() const { return m_servo_position; }

  // Last (byte 1 << 8 | byte 2) received for each wheel command, or -1 if never received.
  s32 GetCommandValue(u8 command) const { return m_command_values[command & 0x7f]; }
  // Number of times each wheel command was received (a repeated identical value is otherwise invisible).
  u32 GetCommandCount(u8 command) const { return m_command_counts[command & 0x7f]; }
  // Incremented every time a wheel command is received (lets callers skip work when nothing arrived).
  u32 GetCommandSerial() const { return m_command_serial; }
  u8 GetInitState() const { return m_init_state; }

  void DoState(PointerWrap&) override;

private:
  void ProcessRequest(std::span<const u8>);

  u8 m_init_state = 0;

  s8 m_servo_position = 0;

  std::array<s32, 0x80> m_command_values = [] {
    std::array<s32, 0x80> values;
    values.fill(-1);
    return values;
  }();
  std::array<u32, 0x80> m_command_counts{};
  u32 m_command_serial = 0;
};

// Publishes cabinet outputs (lamps) to external software such as BackForceFeeder, using the same
// "network outputs" protocol as MAME/Flycast. Defined in FZeroAX.cpp.
class NetworkOutputs;

// Built-in force feedback: drives a force-feedback device (steering wheel) from the game's target
// wheel position. Defined in FZeroAX.cpp.
class WheelForceFeedback;

// Used for both FZeroAX and FZeroAXMonster.
class FZeroAXCommon_IOAdapter final : public IOAdapter
{
public:
  explicit FZeroAXCommon_IOAdapter(FZeroAXSteeringWheel* steering_wheel);
  ~FZeroAXCommon_IOAdapter() override;

  void Update() override;

protected:
  void HandleGenericOutputsChanged(std::span<const u8> bits_set,
                                   std::span<const u8> bits_cleared) override;

private:
  FZeroAXSteeringWheel* const m_steering_wheel;
  std::unique_ptr<NetworkOutputs> m_outputs;
  std::unique_ptr<WheelForceFeedback> m_ffb;
  u32 m_ffb_counter = 0;

  // What was last published on the network outputs (only changes are published, and only when
  // the wheel actually received something: this runs on every serial transfer).
  u32 m_published_serial = ~0u;
  int m_published_servo = 0x7fffffff;
  int m_published_init = 0x7fffffff;
  u8 m_lamp_bits = 0;
};

// Includes seat motion handling.
class FZeroAXDeluxe_IOAdapter final : public IOAdapter
{
public:
  void Update() override;

  void DoState(PointerWrap&) override;

private:
  u32 m_delay = 0;
  u8 m_rx_reply = 0xF0;
};

class FZeroAXMonster_IOAdapter final : public IOAdapter
{
public:
  void Update() override;
};

}  // namespace Triforce
