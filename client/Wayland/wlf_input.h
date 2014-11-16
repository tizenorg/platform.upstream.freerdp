#ifndef __WLF_INPUT_H
#define __WLF_INPUT_H

#include <wayland-client.h>

typedef struct wlf_input wlfInput;

#include "wlfreerdp.h"

struct wlf_input
{
	rdpInput *input;

	struct wl_pointer *pointer;
	struct wl_keyboard *keyboard;
};

wlfInput* wlf_CreateInput(wlfContext* wlfc);
void wlf_DestroyInput(wlfContext* wlfc, wlfInput* input);

#endif /* __WLF_INPUT_H */
