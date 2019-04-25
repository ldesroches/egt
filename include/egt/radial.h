/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef EGT_RADIAL_H
#define EGT_RADIAL_H

#include <egt/flags.h>
#include <egt/valuewidget.h>

namespace egt
{
inline namespace v1
{

/**
 * @brief Radial dial widget that a user uses to select a value.
 *
 * @image html widget_radial.png
 * @image latex widget_radial.png "widget_radial" width=5cm
 *
 * @ingroup controls
 */
template<class T = int>
class RadialType : public ValueRangeWidget<T>
{
public:

    enum class flag
    {
        primary_value,
        primary_handle,
        secondary_value,
        text,
        text_value
    };

    using flags_type = Flags<flag>;

    /**
     * @param[in] rect Rectangle for the widget.
     * @param[in] min Minimum value for the range.
     * @param[in] max Maximum value in the range.
     * @param[in] value Current value in the range.
     */
    RadialType(const Rect& rect, T min, T max, T value = T())
        : ValueRangeWidget<T>(rect, min, max, value)
    {
        this->set_boxtype(Theme::boxtype::none);
        this->flags().set(Widget::flag::grab_mouse);

        this->radial_flags().set({flag::primary_value,
                                  flag::primary_handle,
                                  flag::secondary_value,
                                  flag::text});
    }

    RadialType(Frame& parent, const Rect& rect, T min, T max, T value = T())
        : RadialType<T>(rect, min, max, value)
    {
        parent.add(*this);
    }

    virtual std::unique_ptr<Widget> clone() override
    {
        return std::unique_ptr<Widget>(make_unique<RadialType<T>>(*this).release());
    }

    /**
     * Get a const ref of the flags.
     */
    inline const flags_type& radial_flags() const { return m_radial_flags; }

    /**
     * Get a modifiable ref of the flags.
     */
    inline flags_type& radial_flags() { return m_radial_flags; }

    inline T value2() const
    {
        return m_value2;
    }

    virtual void set_value2(T v)
    {
        assert(this->m_max > this->m_min);

        if (v > this->m_max)
            v = this->m_max;

        if (v < this->m_min)
            v = this->m_min;

        if (v != this->m_value2)
        {
            this->m_value2 = v;
            this->damage();
            this->invoke_handlers(eventid::property_changed);
        }
    }

    /**
     * Set the center label text of the dial.
     */
    void text(const std::string& text)
    {
        m_text = text;
    }

    virtual int handle(eventid event) override
    {
        auto ret = Widget::handle(event);

        switch (event)
        {
        case eventid::raw_pointer_down:
        {
            this->set_active(true);
            return 1;
        }
        case eventid::raw_pointer_up:
        {
            this->set_active(false);
            return 1;
        }
        case eventid::pointer_click:
        case eventid::pointer_drag:
        {
            auto angle = this->touch_to_degrees(this->from_display(event::pointer().point));
            auto v = this->degrees_to_value(angle);
            auto orig = this->set_value(v);
            if (orig != v)
                this->invoke_handlers(eventid::input_property_changed);

            return 1;
        }
        default:
            break;
        }

        return ret;
    }

    virtual void draw(Painter& painter, const Rect& rect) override
    {
        Drawer<RadialType<T>>::draw(*this, painter, rect);
    }

    static void default_draw(RadialType<T>& widget, Painter& painter, const Rect& rect)
    {
        ignoreparam(rect);

        auto v = widget.value_to_degrees(widget.value());

        auto color1 = widget.palette().color(Palette::ColorId::mid);
        auto color2 = widget.palette().color(Palette::ColorId::highlight);
        auto color3 = widget.palette().color(Palette::ColorId::dark);
        auto color4 = widget.palette().color(Palette::ColorId::highlight);

        float smalldim = std::min(widget.w(), widget.h());
        float linew = smalldim / 10;
        float radius = smalldim / 2 - (linew / 2);
        float angle1 = detail::to_radians<float>(-90, 0);
        float angle2 = detail::to_radians<float>(-90, v);

        auto c = widget.center();

        // bottom full circle
        painter.set(color1);
        painter.set_line_width(linew);
        painter.draw(Arc(c, radius, 0, 2 * M_PI));
        painter.stroke();

        if (widget.radial_flags().is_set(flag::primary_value))
        {
            // value arc
            painter.set(color2);
            painter.set_line_width(linew - (linew / 3));
            painter.draw(Arc(c, radius, angle1, angle2));
            painter.stroke();
        }

        if (widget.radial_flags().is_set(flag::primary_handle))
        {
            // handle
            painter.set(color3);
            painter.set_line_width(linew);
            painter.draw(Arc(c, radius, angle2 - 0.3, angle2));
            painter.stroke();
        }

        if (widget.radial_flags().is_set(flag::secondary_value))
        {
            // secondary value
            float angle3 = detail::to_radians<float>(-90,
                           widget.value_to_degrees(widget.value2()));
            painter.set(color4);
            painter.set_line_width(linew);
            painter.draw(Arc(c, radius, angle3 - 0.01, angle3 + 0.01));
            painter.stroke();
        }

        if (widget.radial_flags().is_set(flag::text))
        {
            if (!widget.m_text.empty())
            {
                painter.set(widget.palette().color(Palette::ColorId::text));
                painter.set(Font(72));

                auto size = painter.text_size(widget.m_text);
                Rect target = detail::align_algorithm(size, widget.box(), alignmask::center, 0);

                painter.draw(target.point());
                painter.draw(widget.m_text);
            }
        }
        else if (widget.radial_flags().is_set(flag::text_value))
        {
            auto txt = std::to_string(widget.value());

            painter.set(widget.palette().color(Palette::ColorId::text));
            painter.set(Font(72));

            auto size = painter.text_size(txt);
            Rect target = detail::align_algorithm(size, widget.box(), alignmask::center, 0);

            painter.draw(target.point());
            painter.draw(txt);
        }
    }

    /**
     * Get the current text of the radial.
     */
    virtual const std::string& text() const { return m_text; }

    float touch_to_degrees(const Point& point)
    {
        const Point c = this->center();
        float radians = c.angle_to<float>(point);
        float angle = detail::to_degrees(radians);
        if (angle < 0)
            angle = angle + 360.;
        return angle;
    }

    /**
     * Normalize a value to degrees.
     */
    float value_to_degrees(T value)
    {
        float n = (static_cast<float>(value) -
                   static_cast<float>(this->m_min)) /
                  (static_cast<float>(this->m_max) - static_cast<float>(this->m_min));
        return n * 360.;
    }

    /**
     * Normalize degrees to a value.
     */
    T degrees_to_value(float degrees)
    {
        float n = degrees / 360.;
        return (n * (this->m_max - this->m_min)) + this->m_min;
    }

    virtual ~RadialType()
    {}

protected:

    std::string m_text;

    T m_value2{};

    RadialType() = delete;

    /**
     * Radial flags.
     */
    flags_type m_radial_flags;
};

using Radial = RadialType<>;

}
}

#endif
