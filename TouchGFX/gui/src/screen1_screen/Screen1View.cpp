#include <gui/screen1_screen/Screen1View.hpp>

extern "C" {
    // 0 = MODE_STANDARD, 1 = MODE_MOTION
    extern volatile int g_current_mode;
}


Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
    // Biến static để ghi nhớ trạng thái mode của khung hình trước
    static int last_mode = -1;

    // Chỉ xử lý cập nhật màn hình nếu có sự thay đổi mode (để tiết kiệm CPU)
    if (g_current_mode != last_mode) {
        last_mode = g_current_mode; // Cập nhật lại

        if (g_current_mode == 0) { // Đang ở MODE_STANDARD
            textStandard.setVisible(true);   // Hiện chữ Xanh
            textMotion.setVisible(false);    // Ẩn chữ Đỏ
        }
        else { // Đang ở MODE_MOTION
            textStandard.setVisible(false);  // Ẩn chữ Xanh
            textMotion.setVisible(true);     // Hiện chữ Đỏ
        }

        // Bắt buộc gọi invalidate() để TouchGFX biết thành phần này cần được vẽ lại
        textStandard.invalidate();
        textMotion.invalidate();
    }
}
