#pragma once

#include <iostream>

namespace rvn {
    typedef enum class MouseCode : uint16_t {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,
        ButtonLast = Button7,
        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2,
    } Mouse;
    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
    {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }
}
#define MOUSE_BUTTON_0         ::rvn::MouseCode::Button0
#define MOUSE_BUTTON_1         ::rvn::MouseCode::Button1
#define MOUSE_BUTTON_2         ::rvn::MouseCode::Button2
#define MOUSE_BUTTON_3         ::rvn::MouseCode::Button3
#define MOUSE_BUTTON_4         ::rvn::MouseCode::Button4
#define MOUSE_BUTTON_5         ::rvn::MouseCode::Button5
#define MOUSE_BUTTON_6         ::rvn::MouseCode::Button6
#define MOUSE_BUTTON_7         ::rvn::MouseCode::Button7
#define MOUSE_BUTTON_LAST      ::rvn::MouseCode::ButtonLast
#define MOUSE_BUTTON_LEFT      ::rvn::MouseCode::ButtonLeft
#define MOUSE_BUTTON_RIGHT     ::rvn::MouseCode::ButtonRight
#define MOUSE_BUTTON_MIDDLE    ::rvn::MouseCode::ButtonMiddle