#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "gamepad_hid.h"

// Biến lưu trạng thái tay cầm chia sẻ giữa các task
extern volatile GamepadReport_t g_gamepad_report;

void InputManager_Init(void);
void InputManager_Update(void);

#endif // INPUT_MANAGER_H