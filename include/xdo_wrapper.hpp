#ifndef XDO_WRAPPER_HPP_INCLUDED
#define XDO_WRAPPER_HPP_INCLUDED
extern "C" {
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <unistd.h>
#include <wchar.h>
}

//// For Sake of Qt
#undef Bool
#undef CursorShape
#undef Expose
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef None
#undef Status
#undef Unsorted

namespace XDOWrapper {
 extern "C" {
   #include <xdo.h>
 }
} // namespace

#endif // XDO_WRAPPER_HPP_INCLUDED
