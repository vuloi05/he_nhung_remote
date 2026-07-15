#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateModeUI(int newMode);
    virtual void updateUsbUI(int usbState);
protected:
    Unicode::UnicodeChar modeBuffer[20]; // Bộ đệm chứa text hiển thị
    Unicode::UnicodeChar usbBuffer[20];
};

#endif // SCREEN1VIEW_HPP
