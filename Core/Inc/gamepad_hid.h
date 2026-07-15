#ifndef GAMEPAD_HID_H
#define GAMEPAD_HID_H

#include <stdint.h>

// Struct 7 bytes: Trình tự phải giống y hệt thứ tự mô tả trong Report Descriptor
typedef struct __attribute__((packed)) {
	uint16_t buttons;    // NÂNG CẤP: Dùng 16 bits để chứa tối đa 16 nút    // Bit 0->7: 8 nút bấm
    uint8_t  left_x;     // 0=Left, 128=Center, 255=Right
    uint8_t  left_y;     // 0=Up, 128=Center, 255=Down
    uint8_t  right_x;    // 0=Left, 128=Center, 255=Right
    uint8_t  right_y;    // 0=Up, 128=Center, 255=Down
    int16_t  gyro_raw;   // Dữ liệu Gyro (-32768 đến 32767)
} GamepadReport_t;

void Gamepad_SendReport(GamepadReport_t *report);

#endif // GAMEPAD_HID_H
