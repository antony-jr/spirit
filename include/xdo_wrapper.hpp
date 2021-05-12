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
#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif /* __USE_XOPEN */

/**
 * @mainpage
 *
 * libxdo helps you send fake mouse and keyboard input, search for windows,
 * perform various window management tasks such as desktop changes, window
 * movement, etc.
 *
 * For examples on libxdo usage, the xdotool source code is a good reference.
 *
 * @see xdo.h
 * @see xdo_new
 */

/**
 * When issuing a window size change, giving this flag will make the size
 * change be relative to the size hints of the window.  For terminals, this
 * generally means that the window size will be relative to the font size,
 * allowing you to change window sizes based on character rows and columns
 * instead of pixels.
 */
#define SIZE_USEHINTS (1L << 0)
#define SIZE_USEHINTS_X (1L << 1)
#define SIZE_USEHINTS_Y (1L << 2)

/**
 * CURRENTWINDOW is a special identify for xdo input faking (mouse and
 * keyboard) functions like xdo_send_keysequence_window that indicate we should target the
 * current window, not a specific window.
 *
 * Generally, this means we will use XTEST instead of XSendEvent when sending
 * events.
 */
#define CURRENTWINDOW (0)

/**
 * @internal
 * Map character to whatever information we need to be able to send
 * this key (keycode, modifiers, group, etc)
 */
typedef struct charcodemap {
  wchar_t key; /** the letter for this key, like 'a' */
  KeyCode code; /** the keycode that this key is on */
  KeySym symbol; /** the symbol representing this key */
  int group; /** the keyboard group that has this key in it */
  int modmask; /** the modifiers to apply when sending this key */
   /** if this key need to be bound at runtime because it does not
    * exist in the current keymap, this will be set to 1. */
  int needs_binding;
} charcodemap_t;

typedef enum {
  XDO_FEATURE_XTEST, /** Is XTest available? */
} XDO_FEATURES;

/**
 * The main context.
 */
typedef struct xdo {

  /** The Display for Xlib */
  Display *xdpy;

  /** The display name, if any. NULL if not specified. */
  char *display_name;

  /** @internal Array of known keys/characters */
  charcodemap_t *charcodes;

  /** @internal Length of charcodes array */
  int charcodes_len;

  /** @internal highest keycode value */
  int keycode_high; /* highest and lowest keycodes */

  /** @internal lowest keycode value */
  int keycode_low;  /* used by this X server */
  
  /** @internal number of keysyms per keycode */
  int keysyms_per_keycode;

  /** Should we close the display when calling xdo_free? */
  int close_display_when_freed;

  /** Be extra quiet? (omits some error/message output) */
  int quiet;

  /** Enable debug output? */
  int debug;

  /** Feature flags, such as XDO_FEATURE_XTEST, etc... */
  int features_mask;

} xdo_t;


#define XDO_ERROR 1
#define XDO_SUCCESS 0

/**
 * Create a new xdo_t instance.
 *
 * @param display the string display name, such as ":0". If null, uses the
 * environment variable DISPLAY just like XOpenDisplay(NULL).
 *
 * @return Pointer to a new xdo_t or NULL on failure
 */
xdo_t* xdo_new(const char *display);


/**
 * Free and destroy an xdo_t instance.
 *
 * If close_display_when_freed is set, then we will also close the Display.
 */
void xdo_free(xdo_t *xdo);

#define SIZE_TO 0
#define SIZE_FROM 1

/**
 * Get the window currently having focus.
 *
 * @param window_ret Pointer to a window where the currently-focused window
 *   will be stored.
 */
int xdo_get_focused_window(const xdo_t *xdo, Window *window_ret);

/**
 * Get a window's location.
 *
 * @param wid the window to query
 * @param x_ret pointer to int where the X location is stored. If NULL, X is
 *   ignored.
 * @param y_ret pointer to int where the Y location is stored. If NULL, X is
 *   ignored.
 * @param screen_ret Pointer to Screen* where the Screen* the window on is
 *   stored. If NULL, this parameter is ignored.
 */
int xdo_get_window_location(const xdo_t *xdo, Window wid,
                            int *x_ret, int *y_ret, Screen **screen_ret);

/**
 * Get a window's size.
 *
 * @param wid the window to query
 * @param width_ret pointer to unsigned int where the width is stored.
 * @param height_ret pointer to unsigned int where the height is stored.
 */
int xdo_get_window_size(const xdo_t *xdo, Window wid, unsigned int *width_ret,
                        unsigned int *height_ret);

/**
 * Get a window's name, if any.
 *
 * TODO(sissel): Document
 */
int xdo_get_window_name(const xdo_t *xdo, Window window, 
                        unsigned char **name_ret, int *name_len_ret,
                        int *name_type);

 }
} // namespace

#endif // XDO_WRAPPER_HPP_INCLUDED
