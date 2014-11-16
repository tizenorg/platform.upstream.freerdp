#ifndef __WLFREERDP_H
#define __WLFREERDP_H

#include <freerdp/freerdp.h>
#include <winpr/wtypes.h>

typedef struct wlf_context wlfContext;

#include "wlf_display.h"
#include "wlf_window.h"
//#include "wlf_input.h"

struct wlf_context
{
	rdpContext context;

	wlfDisplay* display;
	wlfWindow* window;
	//wlfPointer* pointer;
};

#endif /* __WLFREERDP_H */

