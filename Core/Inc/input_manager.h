#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "gamepad_hid.h"
#include "L3GD20.h" // Thêm thư viện Gyro

typedef enum {
    MODE_STANDARD = 0, // Chơi bằng Joystick vật lý
    MODE_MOTION   = 1  // Chơi bằng Gyroscope
} GamepadMode_t;

// Biến lưu trạng thái Mode hiện tại
extern volatile GamepadMode_t g_current_mode;

// Biến lưu dữ liệu báo cáo USB
extern volatile GamepadReport_t g_gamepad_report;

void InputManager_Init(void);
void InputManager_Update(void);
void InputManager_ToggleMode(void); // Hàm đổi Mode

#endif // INPUT_MANAGER_H
