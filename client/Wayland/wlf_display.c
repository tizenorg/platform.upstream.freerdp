#include <stdio.h>

#include "wlf_display.h"

static void wl_registry_handle_global(void* data, struct wl_registry* registry, uint32_t id, const char *interface, uint32_t version)
{
	wlfDisplay* display = data;

	if (strcmp(interface, "wl_compositor") == 0)
		display->compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
	else if (strcmp(interface, "wl_shell") == 0)
		display->shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
	else if (strcmp(interface, "wl_shm") == 0)
		display->shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
	else if (strcmp(interface, "wl_seat") == 0)
		display->seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
}

static void wl_registry_handle_global_remove(void* data, struct wl_registry* registry, uint32_t name)
{

}

static const struct wl_registry_listener wl_registry_listener =
{
	wl_registry_handle_global,
	wl_registry_handle_global_remove
};


wlfDisplay* wlf_CreateDisplay(void)
{
	wlfDisplay* display;

	display = (wlfDisplay*) malloc(sizeof(wlfDisplay));
	ZeroMemory(display, sizeof(wlfDisplay));

	if (display)
	{
		display->display = wl_display_connect(NULL);

		if (!display->display)
		{
			fprintf(stderr, "wl_pre_connect: failed to connect to Wayland compositor\n");
			fprintf(stderr, "Please check that the XDG_RUNTIME_DIR environment variable is properly set.\n");
			free(display);
			return NULL;
		}

		display->registry = wl_display_get_registry(display->display);
		wl_registry_add_listener(display->registry, &wl_registry_listener, display);
		wl_display_roundtrip(display->display);

		if (!display->compositor || !display->shell || !display->shm)
		{
			fprintf(stderr, "wl_pre_connect: failed to find needed compositor interfaces\n");
			free(display);
			return NULL;
		}
	}

	return display;
}

void wlf_RefreshDisplay(wlfDisplay* display)
{
	wl_display_dispatch(display->display);
}

void wlf_DestroyDisplay(wlfContext* wlfc, wlfDisplay* display)
{
	if (display == NULL)
		return;

	if (wlfc->display == display)
		wlfc->display = NULL;

	if (display->seat)
		wl_seat_destroy(display->seat);
	if (display->shm)
		wl_shm_destroy(display->shm);
	if (display->shell)
		wl_shell_destroy(display->shell);
	if (display->compositor)
		wl_compositor_destroy(display->compositor);
	if (display->registry)
		wl_registry_destroy(display->registry);
	wl_display_disconnect(display->display);

	free(display);
}
