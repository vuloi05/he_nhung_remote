#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    //Hàm nhận sự kiện đổi mode (để trống)
    virtual void modeChanged(int newMode) {}
    virtual void usbStateChanged(int usbState) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
