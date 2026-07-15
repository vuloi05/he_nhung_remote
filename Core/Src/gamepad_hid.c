#include "gamepad_hid.h"
#include "usbd_customhid.h"

// Lấy biến hUsbDeviceHS (do CubeMX sinh ra trong usb_device.c)
extern USBD_HandleTypeDef hUsbDeviceHS;

void Gamepad_SendReport(GamepadReport_t *report)
{
    // Kiểm tra xem USB có đang kết nối và sẵn sàng gửi không
    if (hUsbDeviceHS.dev_state == USBD_STATE_CONFIGURED) {
        USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, (uint8_t*)report, sizeof(GamepadReport_t));
    }
}
