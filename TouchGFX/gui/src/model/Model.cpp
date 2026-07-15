#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// THÊM ĐOẠN NÀY ĐỂ KẾT NỐI VỚI C-CODE
extern "C" {
    #include "input_manager.h"
	extern volatile uint8_t g_usb_connected;
}

Model::Model() : modelListener(0), previousMode(-1), previousUsbState(-1) // Khởi tạo mode cũ khác 0 và 1
{
}

void Model::tick()
{
	// 1. CẬP NHẬT MODE
    // Đọc biến toàn cục (chạy ở Input_Task)
    int currentMode = (int)g_current_mode;

    // Nếu mode thay đổi do người dùng bấm nút USER
    if (currentMode != previousMode)
    {
        previousMode = currentMode;

        // Báo cho Presenter biết có sự thay đổi
        if (modelListener != 0)
        {
            modelListener->modeChanged(currentMode);
        }
    }

    // 2. CẬP NHẬT TRẠNG THÁI USB
    int currentUsb = (int)g_usb_connected;
    if (currentUsb != previousUsbState)
    {
    	previousUsbState = currentUsb;
    	if (modelListener != 0)
    	{
    		modelListener->usbStateChanged(currentUsb);
    	}
    }
}
