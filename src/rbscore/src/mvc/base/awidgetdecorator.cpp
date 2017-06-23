#include "awidgetdecorator.h"

AWidgetDecorator::AWidgetDecorator(QWidget *_view) :
    QObject(_view),
    view_(_view),
    device_(0)
{

}

void AWidgetDecorator::onWidgetGotFocus(Device *_device)
{
    _do_on_widget_got_focus(_device);
    emit activated();
}

void AWidgetDecorator::onWidgetLostFocus(Device *_device)
{
    emit deactivated();
    _do_on_widget_lost_focus(_device);
}

void AWidgetDecorator::decorate(Device *_device)
{
    if (device_ != _device)
    {
        device_ = _device;

    }

    _set_device_info(_device);
}

QWidget *AWidgetDecorator::getView() const
{
    return view_;
}

void AWidgetDecorator::_reset()
{
    _set_device_info(device_);
}
