#include "input_manager.h"
#include "main.h"
#include "adc.h" // Để gọi hàm HAL_ADC_Start_DMA
volatile GamepadMode_t g_current_mode = MODE_STANDARD;
volatile GamepadReport_t g_gamepad_report = {0};

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
    
    // Set joystick nằm ở giữa lúc khởi động
    g_gamepad_report.left_x = 128;
    g_gamepad_report.left_y = 128;
    g_gamepad_report.right_x = 128;
    g_gamepad_report.right_y = 128;
    g_gamepad_report.buttons = 0;
}

void InputManager_Update(void) {
    // --- 1. CẬP NHẬT JOYSTICK ---
    // ADC1_Rank1: PA5 (Left X) -> adc1_buf[0]
    // ADC1_Rank2: PA7 (Right X) -> adc1_buf[1]
    // ADC1_Rank3: PC3 (Left Y) -> adc1_buf[2]
    // ADC3_Rank1: PF6 (Right Y) -> adc3_buf[0]
    
    g_gamepad_report.left_x  = Process_ADC_Axis(adc1_buf[0]);
    g_gamepad_report.right_x = Process_ADC_Axis(adc1_buf[1]);
    g_gamepad_report.left_y  = Process_ADC_Axis(adc1_buf[2]);
    g_gamepad_report.right_y = Process_ADC_Axis(adc3_buf[0]);

    // --- 2. CẬP NHẬT NÚT BẤM ---
    uint16_t btns = 0;
    
    // Chân GPIO có Pull-up -> Không nhấn = 1 (SET), Nhấn = 0 (RESET)
    // Map từng nút vào 1 bit tương ứng
    if (HAL_GPIO_ReadPin(BTN_CROSS_GPIO_Port, BTN_CROSS_Pin) == GPIO_PIN_RESET) btns |= (1 << 0);
    if (HAL_GPIO_ReadPin(BTN_CIRCLE_GPIO_Port, BTN_CIRCLE_Pin) == GPIO_PIN_RESET) btns |= (1 << 1);
    if (HAL_GPIO_ReadPin(BTN_SQUARE_GPIO_Port, BTN_SQUARE_Pin) == GPIO_PIN_RESET) btns |= (1 << 2);
    if (HAL_GPIO_ReadPin(BTN_TRIANGLE_GPIO_Port, BTN_TRIANGLE_Pin) == GPIO_PIN_RESET) btns |= (1 << 3);
    if (HAL_GPIO_ReadPin(BTN_L1_GPIO_Port, BTN_L1_Pin) == GPIO_PIN_RESET) btns |= (1 << 4);
    if (HAL_GPIO_ReadPin(BTN_R1_GPIO_Port, BTN_R1_Pin) == GPIO_PIN_RESET) btns |= (1 << 5);
    if (HAL_GPIO_ReadPin(BTN_L2_GPIO_Port, BTN_L2_Pin) == GPIO_PIN_RESET) btns |= (1 << 6);
    if (HAL_GPIO_ReadPin(BTN_R2_GPIO_Port, BTN_R2_Pin) == GPIO_PIN_RESET) btns |= (1 << 7);
    
    // Hai nút joystick SW
    if (HAL_GPIO_ReadPin(JOY_L_SW_GPIO_Port, JOY_L_SW_Pin) == GPIO_PIN_RESET) btns |= (1 << 8); // L3
    if (HAL_GPIO_ReadPin(JOY_R_SW_GPIO_Port, JOY_R_SW_Pin) == GPIO_PIN_RESET) btns |= (1 << 9); // R3

    g_gamepad_report.buttons = btns;
}
