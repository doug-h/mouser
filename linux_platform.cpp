#include "input.h"
#include "platform.h"
#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>

#include "linux_scancode_table.h"


namespace Platform {
// Taken from https://www.kernel.org/doc/html/v4.12/input/uinput.html
void emit(int fd, int type, int code, int val)
{
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

struct VirtualInputDevice {
  VirtualInputDevice()
  {
    struct uinput_setup usetup;
    m_handle = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(m_handle, UI_SET_EVBIT, EV_KEY);
    // Mouse buttons
    ioctl(m_handle, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(m_handle, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(m_handle, UI_SET_KEYBIT, BTN_RIGHT);
    // Keyboard
    ioctl(m_handle, UI_SET_KEYBIT, KEY_D);
    ioctl(m_handle, UI_SET_KEYBIT, KEY_S);
    ioctl(m_handle, UI_SET_KEYBIT, KEY_A);
    ioctl(m_handle, UI_SET_KEYBIT, KEY_W);

    ioctl(m_handle, UI_SET_EVBIT, EV_REL);
    ioctl(m_handle, UI_SET_RELBIT, REL_X);
    ioctl(m_handle, UI_SET_RELBIT, REL_Y);

    ioctl(m_handle, UI_SET_EVBIT, EV_SYN);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_VIRTUAL;
    strcpy(usetup.name, "Mouser virtual mouse");

    ioctl(m_handle, UI_DEV_SETUP, &usetup);
    ioctl(m_handle, UI_DEV_CREATE);
  }

  ~VirtualInputDevice()
  {
    ioctl(m_handle, UI_DEV_DESTROY);
    close(m_handle);
  }

  void MoveCursor(int dx, int dy)
  {
    emit(m_handle, EV_REL, REL_X, dx);
    emit(m_handle, EV_REL, REL_Y, dy);
  }

  void SetKey(int KEY_ID, int state)
  {
    emit(m_handle, EV_KEY, KEY_ID, 0);
    printf("Setting %d to %d\n", KEY_ID, state);
  }

  void SetMouseButtons(bool L_state, bool L_changed, bool M_state,
                       bool M_changed, bool R_state, bool R_changed)
  {
    if (L_changed) { emit(m_handle, EV_KEY, BTN_LEFT, L_state); }
    if (M_changed) { emit(m_handle, EV_KEY, BTN_MIDDLE, M_state); }
    if (R_changed) { emit(m_handle, EV_KEY, BTN_RIGHT, R_state); }
  }

  void FinishInput() { emit(m_handle, EV_SYN, SYN_REPORT, 0); }

  int m_handle;
};

VirtualInputDevice &GetVirtualInputDevice()
{
  static VirtualInputDevice _vid;
  return _vid;
}

void SetMouse(const MouseData &md)
{
  VirtualInputDevice &vid = GetVirtualInputDevice();
  static MouseData previous_state;

  // You can do better than this...
  bool L_state, L_changed, M_state, M_changed, R_state, R_changed;


  L_state = IsButtonPressed(md, SDL_BUTTON_LEFT);
  M_state = IsButtonPressed(md, SDL_BUTTON_MIDDLE);
  R_state = IsButtonPressed(md, SDL_BUTTON_RIGHT);
  previous_state.button ^= md.button;
  L_changed = IsButtonPressed(previous_state, SDL_BUTTON_LEFT);
  M_changed = IsButtonPressed(previous_state, SDL_BUTTON_MIDDLE);
  R_changed = IsButtonPressed(previous_state, SDL_BUTTON_RIGHT);


  vid.MoveCursor(md.dx, md.dy);
  vid.SetMouseButtons(L_state, L_changed, M_state, M_changed, R_state,
                      R_changed);
  vid.FinishInput();

  previous_state = md;
}

void SetKeys(SDL_Scancode *keys_to_press, SDL_Scancode *keys_to_release,
             int num_to_press, int num_to_release)
{
  VirtualInputDevice &vid = GetVirtualInputDevice();
  for (int i = 0; i < num_to_press; ++i) {
    SDL_Scancode s = keys_to_press[i];
    int scancode = GetLinuxScancode(s);

    vid.SetKey(scancode, 1);
  }
  for (int i = 0; i < num_to_release; ++i) {
    SDL_Scancode s = keys_to_release[i];
    int scancode = GetLinuxScancode(s);

    vid.SetKey(scancode, 0);
  }

  vid.FinishInput();
}

} // namespace Platform