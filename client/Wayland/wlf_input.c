#include <linux/input.h>

#include "wlf_input.h"

static void wl_pointer_enter(void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t sx_w, wl_fixed_t sy_w)
{

}

static void wl_pointer_leave(void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface)
{
	
}

static void wl_pointer_motion(void* data, struct wl_pointer* pointer, uint32_t time, wl_fixed_t sx_w, wl_fixed_t sy_w)
{
	wlfContext* context = data;
	rdpInput* input;
	UINT16 x;
	UINT16 y;

	input = context->input->input;

	x = wl_fixed_to_int(sx_w);
	y = wl_fixed_to_int(sy_w);

	input->MouseEvent(input, PTR_FLAGS_MOVE, x, y);
}

static void wl_pointer_button(void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	wlfContext* context = data;
	rdpInput* input;
	int flags;

	input = context->input->input;

	if (state == WL_POINTER_BUTTON_STATE_PRESSED)
		flags = PTR_FLAGS_DOWN;

	switch (button)
	{
		case BTN_LEFT:
			flags |= PTR_FLAGS_BUTTON1;
			break;
		case BTN_RIGHT:
			flags |= PTR_FLAGS_BUTTON2;
			break;
		case BTN_MIDDLE:
			flags |= PTR_FLAGS_BUTTON3;
			break;
		default:
			break;
	}

	input->MouseEvent(input, flags, 0, 0);
}

static const struct wl_pointer_listener wl_pointer_listener =
{
	wl_pointer_enter,
	wl_pointer_leave,
	wl_pointer_motion,
	wl_pointer_button,
	NULL
};

static void wl_seat_handle_capabilities(void* data, struct wl_seat* seat, enum wl_seat_capability capabilities)
{
	wlfContext* context = data;
	struct wl_pointer* pointer;
	struct wl_pointer* keyboard;

	if (capabilities & WL_SEAT_CAPABILITY_POINTER)
	{
		pointer = wl_seat_get_pointer(seat);

		context->input->pointer = pointer;
		wl_pointer_add_listener(pointer, &wl_pointer_listener, context);
	}
}

static const struct wl_seat_listener wl_seat_listener = {
	wl_seat_handle_capabilities
};


void wlf_InitInput(wlfContext* wlfc, rdpInput* input)
{
	wlfInput *input_w;
	struct wl_seat *seat;

	if (!wlfc->display->seat)
		return;

	seat = wlfc->display->seat;

	input_w = (wlfInput*) malloc(sizeof(wlfInput));
	ZeroMemory(input_w, sizeof(wlfInput));

	input_w->input = input;
	wlfc->input = input_w;

	wl_seat_add_listener(seat, &wl_seat_listener, wlfc);
}

void wlf_CloseInput(wlfContext* wlfc)
{
	if (!wlfc->input)
		return;

	if (wlfc->input->pointer)
		wl_pointer_release(wlfc->input->pointer);

	free(wlfc->input);
	wlfc->input = NULL;
}
