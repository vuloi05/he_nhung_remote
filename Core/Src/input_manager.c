#include "input_manager.h"
#include "main.h"
#include "adc.h" // Để gọi hàm HAL_ADC_Start_DMA
#include <math.h>
#include "usbd_def.h"

// Biến lưu trạng thái USB
volatile uint8_t g_usb_connected = 0;

// Lấy "tay nắm" của USB Device từ thư viện sinh bởi CubeMX
extern USBD_HandleTypeDef hUsbDeviceHS;

volatile GamepadReport_t g_gamepad_report = {0};
volatile GamepadMode_t g_current_mode = MODE_STANDARD;

// Buffer lưu 3 kênh ADC1 (PA5, PA7, PC3)
uint16_t adc1_buf[3];

// Buffer lưu 1 kênh ADC3 (PF6)
uint16_t adc3_buf[1];

// Hàm phụ trợ xử lý ADC: Tạo vùng chết (Deadzone) và thu nhỏ 12-bit -> 8-bit
static uint8_t Process_ADC_Axis(uint16_t raw_adc) {
    if (raw_adc > 1848 && raw_adc < 2248) {
        return 128;
    }
    return (uint8_t)(raw_adc >> 4);
}

void InputManager_Init(void) {
    // Khởi động ADC1 (3 kênh) và ADC3 (1 kênh) qua DMA
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_buf, 3);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc3_buf, 1);

    // Khởi động Gyroscope
    L3GD20_Init();

    // Khởi động ở Mode 1: Bật LED Xanh, tắt LED Đỏ
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

    // Set joystick nằm ở giữa lúc khởi động
    g_gamepad_report.left_x = 128;
    g_gamepad_report.left_y = 128;
    g_gamepad_report.right_x = 128;
    g_gamepad_report.right_y = 128;
    g_gamepad_report.buttons = 0;
}

// Hệ số nhạy Gyro: Điều chỉnh số này để tay cầm nhạy hơn hoặc chậm hơn
// Giá trị 2.0 nghĩa là nghiêng 45° = joystick đẩy hết cỡ (128*2 ≈ 255 - 128)
// Tăng lên 3.0-4.0 nếu muốn nhạy hơn (nghiêng ít hơn cũng phản hồi mạnh)
// Hệ số cho hàm bình phương (~45 độ sẽ đẩy joystick chạm đỉnh 255)
#define GYRO_QUAD_CONST 0.0627f

void InputManager_Update(void) {
	// Kểm tra xem máy tính đã nhận diện USB Gamepad thành công chưa
	    if (hUsbDeviceHS.dev_state == USBD_STATE_CONFIGURED) {
	        g_usb_connected = 1;
	    } else {
	        g_usb_connected = 0;
	    }

    // BẮT BUỘC: Luôn gọi vòng lặp Gyro để cập nhật sensor (dù đang ở Mode nào)
    L3GD20_loop();

    // --- 1. CẬP NHẬT JOYSTICK TRÁI (Luôn dùng ADC) ---
    g_gamepad_report.left_x  = Process_ADC_Axis(adc1_buf[0]);
    g_gamepad_report.left_y  = Process_ADC_Axis(adc1_buf[2]);

    // --- 2. CẬP NHẬT JOYSTICK PHẢI (Theo Mode) ---
    if (g_current_mode == MODE_STANDARD) {
        // Mode 1: Dùng giá trị ADC vật lý
        g_gamepad_report.right_x = Process_ADC_Axis(adc1_buf[1]);
        g_gamepad_report.right_y = Process_ADC_Axis(adc3_buf[0]);
    } else {
        // Mode 2: Dùng giá trị Gyro giả lập Joystick phải
        // Đã đổi hàm lấy trục Z sang trục Y để dùng thao tác lật nghiêng trái/phải để quẹo
        float angle_y = get_Angle_Y(); // Roll: lật nghiêng trái/phải
        float angle_x = get_Angle_X(); // Pitch: gập lên/xuống

        // VÙNG CHẾT (DEADZONE): Bỏ qua các góc rung tay nhỏ dưới 2 độ
        if (angle_y > -2.0f && angle_y < 2.0f) angle_y = 0.0f;
        if (angle_x > -2.0f && angle_x < 2.0f) angle_x = 0.0f;

        // ÁNH XẠ PHI TUYẾN (NON-LINEAR MAPPING):
        // Dùng fabsf() để lấy trị tuyệt đối số thực, roundf() để làm tròn toán học
        int32_t mapped_x = 128 + (int32_t)roundf(angle_y * fabsf(angle_y) * GYRO_QUAD_CONST);
        int32_t mapped_y = 128 + (int32_t)roundf(angle_x * fabsf(angle_x) * GYRO_QUAD_CONST);

        // Clamp để không tràn số 8-bit
        if (mapped_x < 0) mapped_x = 0;
        if (mapped_x > 255) mapped_x = 255;
        if (mapped_y < 0) mapped_y = 0;
        if (mapped_y > 255) mapped_y = 255;

        g_gamepad_report.right_x = (uint8_t)mapped_x;
        g_gamepad_report.right_y = (uint8_t)mapped_y;
    }

    // --- 3. CẬP NHẬT NÚT BẤM ---
    uint16_t btns = 0;
    if (HAL_GPIO_ReadPin(BTN_CROSS_GPIO_Port, BTN_CROSS_Pin) == GPIO_PIN_RESET) btns |= (1 << 0);
    if (HAL_GPIO_ReadPin(BTN_CIRCLE_GPIO_Port, BTN_CIRCLE_Pin) == GPIO_PIN_RESET) btns |= (1 << 1);
    if (HAL_GPIO_ReadPin(BTN_SQUARE_GPIO_Port, BTN_SQUARE_Pin) == GPIO_PIN_RESET) btns |= (1 << 2);
    if (HAL_GPIO_ReadPin(BTN_TRIANGLE_GPIO_Port, BTN_TRIANGLE_Pin) == GPIO_PIN_RESET) btns |= (1 << 3);
    if (HAL_GPIO_ReadPin(BTN_L1_GPIO_Port, BTN_L1_Pin) == GPIO_PIN_RESET) btns |= (1 << 4);
    if (HAL_GPIO_ReadPin(BTN_R1_GPIO_Port, BTN_R1_Pin) == GPIO_PIN_RESET) btns |= (1 << 5);
    if (HAL_GPIO_ReadPin(BTN_L2_GPIO_Port, BTN_L2_Pin) == GPIO_PIN_RESET) btns |= (1 << 6);
    if (HAL_GPIO_ReadPin(BTN_R2_GPIO_Port, BTN_R2_Pin) == GPIO_PIN_RESET) btns |= (1 << 7);

    if (HAL_GPIO_ReadPin(JOY_L_SW_GPIO_Port, JOY_L_SW_Pin) == GPIO_PIN_RESET) btns |= (1 << 8); // L3
    if (HAL_GPIO_ReadPin(JOY_R_SW_GPIO_Port, JOY_R_SW_Pin) == GPIO_PIN_RESET) btns |= (1 << 9); // R3

    g_gamepad_report.buttons = btns;
}

void InputManager_ToggleMode(void) {
    if (g_current_mode == MODE_STANDARD) {
        g_current_mode = MODE_MOTION;
        // Reset góc Gyro về 0 để joystick bắt đầu từ tâm
        L3GD20_ResetAngles();
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    } else {
        g_current_mode = MODE_STANDARD;
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    }
}
