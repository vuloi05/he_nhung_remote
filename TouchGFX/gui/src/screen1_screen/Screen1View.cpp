#include <gui/screen1_screen/Screen1View.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
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


void Screen1View::updateModeUI(int newMode)
{
    if (newMode == 0) // MODE_STANDARD
    {
        Unicode::snprintf(modeBuffer, 20, "MODE\n STANDARD");
        imageModeIcon.setBitmap(touchgfx::Bitmap(BITMAP_ICON_GAMEPAD_ID));
    }
    else // MODE_MOTION
    {
        Unicode::snprintf(modeBuffer, 20, "MODE\n GYRO MOTION");
        imageModeIcon.setBitmap(touchgfx::Bitmap(BITMAP_ICON_GYRO_ID));
    }
    // Gán buffer vào textAreaMode
    textAreaMode.setWildcard(modeBuffer);

    // Ra lệnh vẽ lại phần màn hình chứa text này
    textAreaMode.invalidate();

    // Ra lệnh vẽ lại ảnh
    imageModeIcon.invalidate();

}

void Screen1View::updateUsbUI(int usbState)
{
    if (usbState == 1)
    {
        Unicode::snprintf(usbBuffer, 20, "USB \nREADY");
        textAreaUsbStatus.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0)); // Màu xanh lá
    }
    else
    {
        Unicode::snprintf(usbBuffer, 20, "USB \nDISCONNECTED");
        textAreaUsbStatus.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0)); // Màu đỏ
    }
    textAreaUsbStatus.setWildcard(usbBuffer);
    textAreaUsbStatus.invalidate();
}
