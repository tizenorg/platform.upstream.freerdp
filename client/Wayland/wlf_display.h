#ifndef __WLF_DISPLAY_H
#define __WLF_DISPLAY_H

#include <wayland-client.h>

typedef struct wlf_display wlfDisplay;

#include "wlfreerdp.h"

struct wlf_display
{
	struct wl_display* display;
	struct wl_registry* registry;
	struct wl_compositor* compositor;
	struct wl_shell* shell;
	struct wl_shm* shm;
};

wlfDisplay* wlf_CreateDisplay(void);
void wlf_RefreshDisplay(wlfDisplay* display);
void wlf_DestroyDisplay(wlfContext* wlfc, wlfDisplay* display);

#endif /* __WLF_DISPLAY_H */
