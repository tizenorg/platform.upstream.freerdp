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

void wlf_InitInput(wlfContext* wlfc, rdpInput* input);
void wlf_CloseInput(wlfContext* wlfc);

#endif /* __WLF_INPUT_H */
