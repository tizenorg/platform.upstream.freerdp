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
	wlfInput* input_w = data;
	rdpInput* input;
	UINT16 x;
	UINT16 y;

	input = input_w->input;

	x = wl_fixed_to_int(sx_w);
	y = wl_fixed_to_int(sy_w);

	input->MouseEvent(input, PTR_FLAGS_MOVE, x, y);
}

static void wl_pointer_button(void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	wlfInput* input_w = data;
	rdpInput* input;
	int flags;

	input = input_w->input;

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
	wlfInput* input = data;
	struct wl_pointer* pointer;

	if (capabilities & WL_SEAT_CAPABILITY_POINTER)
	{
		pointer = wl_seat_get_pointer(seat);

		input->pointer = pointer;
		wl_pointer_add_listener(pointer, &wl_pointer_listener, input);
	}
}

static const struct wl_seat_listener wl_seat_listener = {
	wl_seat_handle_capabilities
};


wlfInput* wlf_CreateInput(wlfContext* wlfc)
{
	wlfInput* input;
	struct wl_seat* seat;

	if (!wlfc->display)
		return NULL;
	if (!wlfc->display->seat)
		return NULL;
	seat = wlfc->display->seat;

	input = (wlfInput*) malloc(sizeof(wlfInput));
	ZeroMemory(input, sizeof(wlfInput));

	if (input)
	{
		input->input = wlfc->context.input;

		wl_seat_add_listener(seat, &wl_seat_listener, input);
	}

	return input;
}

void wlf_DestroyInput(wlfContext* wlfc, wlfInput* input)
{
	if (input == NULL)
		return;

	if (wlfc->input == input)
		wlfc->input = NULL;

	if (input->pointer)
		wl_pointer_release(input->pointer);
	if (input->keyboard)
		wl_keyboard_release(input->keyboard);

	free(input);
}
