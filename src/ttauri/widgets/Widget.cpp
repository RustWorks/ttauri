// Copyright 2019 Pokitec
// All rights reserved.

#include "Widget.hpp"
#include "../GUI/utils.hpp"

namespace tt {

Widget::Widget(Window &_window, Widget *_parent, float _width, float _height) noexcept :
    _minimumWidth(_width),
    _preferredWidth(_width),
    _maximumWidth(_width),
    _minimumHeight(_height),
    _preferredHeight(_height),
    _maximumHeight(_height),
    window(_window),
    parent(_parent),
    elevation(_parent ? _parent->elevation + 1.0f : 0.0f),
    enabled(true)
    
{
    [[maybe_unused]] ttlet enabled_cbid = enabled.add_callback([this](auto...){
        window.requestRedraw = true;
    });

    minimumWidthConstraint = window.addConstraint(width >= _minimumWidth, rhea::strength::strong());
    minimumHeightConstraint = window.addConstraint(height >= _minimumHeight, rhea::strength::strong());
    preferredWidthConstraint = window.addConstraint(width >= _preferredWidth, rhea::strength::medium());
    preferredHeightConstraint = window.addConstraint(height >= _preferredHeight, rhea::strength::medium());
    maximumWidthConstraint = window.addConstraint(width <= _maximumWidth, rhea::strength::weak());
    maximumHeightConstraint = window.addConstraint(height <= _maximumHeight, rhea::strength::weak());
    baseConstraint =  window.addConstraint(base == middle, rhea::strength::weak());
}

Widget::~Widget()
{
    window.removeConstraint(minimumWidthConstraint);
    window.removeConstraint(minimumHeightConstraint);
    window.removeConstraint(preferredWidthConstraint);
    window.removeConstraint(preferredHeightConstraint);
    window.removeConstraint(maximumWidthConstraint);
    window.removeConstraint(maximumHeightConstraint);
}

GUIDevice *Widget::device() const noexcept
{
    auto device = window.device;
    tt_assert(device);
    return device;
}

[[nodiscard]] float Widget::baseHeight() const noexcept {
    ttlet lock = std::scoped_lock(window.widgetSolverMutex);
    return numeric_cast<float>(base.value() - bottom.value());
}

void Widget::setMinimumWidth(float _width) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_width != _minimumWidth) {
        _minimumWidth = _width;

        minimumWidthConstraint = window.replaceConstraint(
            minimumWidthConstraint,
            width >= _minimumWidth,
            rhea::strength::required()
        );
    }
}

void Widget::setMinimumHeight(float _height) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_height != _minimumHeight) {
        _minimumHeight = _height;

        minimumHeightConstraint = window.replaceConstraint(
            minimumHeightConstraint,
            height >= _minimumHeight,
            rhea::strength::required()
        );
    }
}

void Widget::setMinimumExtent(float _width, float _height) noexcept {
    setMinimumWidth(_width);
    setMinimumHeight(_height);
}

void Widget::setMinimumExtent(vec extent) noexcept {
    setMinimumExtent(extent.width(), extent.height());
}

void Widget::setPreferredWidth(float _width) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_width != _preferredWidth) {
        _preferredWidth = _width;

        preferredWidthConstraint = window.replaceConstraint(
            preferredWidthConstraint,
            width >= _preferredWidth,
            rhea::strength::medium()
        );
    }
}

void Widget::setPreferredHeight(float _height) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_height != _preferredHeight) {
        _preferredHeight = _height;

        preferredHeightConstraint = window.replaceConstraint(
            preferredHeightConstraint,
            height >= _preferredHeight,
            rhea::strength::medium()
        );
    }
}

void Widget::setPreferredExtent(float _width, float _height) noexcept {
    setPreferredWidth(_width);
    setPreferredHeight(_height);
}

void Widget::setPreferredExtent(vec extent) noexcept {
    setPreferredExtent(extent.width(), extent.height());
}

void Widget::setMaximumWidth(float _width) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_width != _maximumWidth) {
        _maximumWidth = _width;

        maximumWidthConstraint = window.replaceConstraint(
            maximumWidthConstraint,
            width >= _maximumWidth,
            rhea::strength::weak()
        );
    }
}

void Widget::setMaximumHeight(float _height) noexcept
{
    auto lock = std::scoped_lock(mutex);
    if (_height != _maximumHeight) {
        _maximumHeight = _height;

        maximumHeightConstraint = window.replaceConstraint(
            maximumHeightConstraint,
            height >= _maximumHeight,
            rhea::strength::weak()
        );
    }
}

void Widget::setMaximumExtent(float _width, float _height) noexcept {
    setMaximumWidth(_width);
    setMaximumHeight(_height);
}

void Widget::setMaximumExtent(vec extent) noexcept {
    setMaximumExtent(extent.width(), extent.height());
}

rhea::constraint Widget::placeBelow(Widget const &rhs, float margin) const noexcept {
    return window.addConstraint(this->top + margin == rhs.bottom);
}

rhea::constraint Widget::placeAbove(Widget const &rhs, float margin) const noexcept {
    return window.addConstraint(this->bottom == rhs.top + margin);
}

rhea::constraint Widget::placeLeftOf(Widget const &rhs, float margin) const noexcept {
    return window.addConstraint(this->right + margin == rhs.left);
}

rhea::constraint Widget::placeRightOf(Widget const &rhs, float margin) const noexcept {
    return window.addConstraint(this->left == rhs.right + margin);
}

rhea::constraint Widget::placeAtTop(float margin) const noexcept {
    return window.addConstraint(this->top + margin == parent->top);
}

rhea::constraint Widget::placeAtBottom(float margin) const noexcept {
    return window.addConstraint(this->bottom - margin == parent->bottom);
}

rhea::constraint Widget::placeLeft(float margin) const noexcept {
    return window.addConstraint(this->left - margin == parent->left);
}

rhea::constraint Widget::placeRight(float margin) const noexcept {
    return window.addConstraint(this->right + margin == parent->right);
}

bool Widget::needLayout(hires_utc_clock::time_point displayTimePoint) noexcept
{
    //
    // Thread safety: All reads and stores are done on atomic variables.
    //

    auto needLayout = requestLayout.exchange(false, std::memory_order::memory_order_relaxed);

    auto newExtent = round(vec{width.value(), height.value()});
    needLayout |= newExtent != extent();
    setExtent(newExtent);

    auto newOffsetFromWindow = round(vec{left.value(), bottom.value()});
    needLayout |= newOffsetFromWindow != offsetFromWindow();
    setOffsetFromWindow(newOffsetFromWindow);

    return needLayout;
}

bool Widget::layout(hires_utc_clock::time_point displayTimePoint, bool forceLayout) noexcept
{
    if (!(needLayout(displayTimePoint) || forceLayout)) {
        return false;
    }

    auto lock = std::scoped_lock(mutex);

    setOffsetFromParent(
        parent ?
            offsetFromWindow() - parent->offsetFromWindow():
            offsetFromWindow()
    );
        
    fromWindowTransform = mat::T(-offsetFromWindow().x(), -offsetFromWindow().y(), -z());
    toWindowTransform = mat::T(offsetFromWindow().x(), offsetFromWindow().y(), z());

    return true;
}

void Widget::handleCommand(command command) noexcept {
    auto lock = std::scoped_lock(mutex);

    switch (command) {
    case command::gui_widget_next:
        window.updateToNextKeyboardTarget(this);
        break;
    case command::gui_widget_prev:
        window.updateToPrevKeyboardTarget(this);
        break;
    default:;
    }
}

void Widget::handleMouseEvent(MouseEvent const &event) noexcept {
    auto lock = std::scoped_lock(mutex);

    if (event.type == MouseEvent::Type::Entered) {
        hover = true;
        window.requestRedraw = true;
    } else if (event.type == MouseEvent::Type::Exited) {
        hover = false;
        window.requestRedraw = true;
    }
}

void Widget::handleKeyboardEvent(KeyboardEvent const &event) noexcept {
    auto lock = std::scoped_lock(mutex);

    switch (event.type) {
    case KeyboardEvent::Type::Entered:
        focus = true;
        window.requestRedraw = true;
        break;

    case KeyboardEvent::Type::Exited:
        focus = false;
        window.requestRedraw = true;
        break;

    case KeyboardEvent::Type::Key:
        for (ttlet command : event.getCommands()) {
            handleCommand(command);
        }
        break;

    default:;
    }
}

Widget *Widget::nextKeyboardWidget(Widget const *currentKeyboardWidget, bool reverse) const noexcept
{
    if (currentKeyboardWidget == nullptr && acceptsFocus()) {
        // The first widget that accepts focus.
        return const_cast<Widget *>(this);

    } else {
        return nullptr;
    }
}

}
