#ifndef __WLF_WINDOW_H
#define __WLF_WINDOW_H

#include <wayland-client.h>

typedef struct wlf_window wlfWindow;

#include "wlfreerdp.h"

struct wlf_buffer
{
	struct wl_buffer* buffer;
	void* shm_data;
	BOOL busy;
};
typedef struct wlf_buffer wlfBuffer;

struct wlf_window
{
	int width;
	int height;
	struct wl_surface* surface;
	struct wl_shell_surface* shell_surface;
	struct wl_callback* callback;
	wlfBuffer buffers[2];
	wlfDisplay* display;
	void* data;
	BOOL fullscreen;
};

wlfWindow* wlf_CreateDesktopWindow(wlfContext* wlfc, char* name, int width, int height, BOOL decorations);
void wlf_ResizeDesktopWindow(wlfContext* wlfc, wlfWindow* window, int width, int height);
void wlf_SetWindowText(wlfContext* wlfc, wlfWindow* window, char* name);
void wlf_SetWindowFullscreen(wlfContext* wlfc, wlfWindow* window, BOOL fullscree);
void wlf_ShowWindow(wlfContext* wlfc, wlfWindow* window, BYTE state);
void wlf_UpdateWindowArea(wlfContext* wlfc, wlfWindow* window, int x, int y, int width, int height);
void wlf_DestroyWindow(wlfContext* wlfc, wlfWindow* window);

#endif /* __WLF_WINDOW_H */
