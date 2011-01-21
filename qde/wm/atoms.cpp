#include "atoms.h"

namespace Atoms {
static Atom atoms[ATOM_COUNT];
static bool atomsCreated = false;
};

bool Atoms::createNetWMAtoms(Display *d)
{
	if (atomsCreated)
		return true;
	
    static const char *atomNames[] = {
        "COMPOSITING_MANAGER",
        "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
        "KWM_WIN_ICON",
        "_MOTIF_WM_HINTS",
        "_MOTIF_WM_INFO",
        "_NET_ACTIVE_WINDOW",
        "_NET_CLIENT_LIST",
        "_NET_CLIENT_LIST_STACKING",
        "_NET_CLOSE_WINDOW",
        "_NET_CURRENT_DESKTOP",
        "_NET_DESKTOP_GEOMETRY",
        "_NET_DESKTOP_LAYOUT",
        "_NET_DESKTOP_NAMES",
        "_NET_DESKTOP_VIEWPORT",
        "_NET_FRAME_EXTENTS",
        "_NET_MOVERESIZE_WINDOW",
        "_NET_NUMBER_OF_DESKTOPS",
        "_NET_REQUEST_FRAME_EXTENTS",
        "_NET_SHOWING_DESKTOP",
        "_NET_STARTUP_ID",
        "_NET_SUPPORTED",
        "_NET_SUPPORTING_WM_CHECK",
        "_NET_SYSTEM_TRAY_OPCODE",
        "_NET_SYSTEM_TRAY_MESSAGE_DATA",
        "_NET_WM_ACTION_ABOVE",
        "_NET_WM_ACTION_BELOW",
        "_NET_WM_ACTION_CHANGE_DESKTOP",
        "_NET_WM_ACTION_CLOSE",
        "_NET_WM_ACTION_FULLSCREEN",
        "_NET_WM_ACTION_MAXIMIZE_HORZ",
        "_NET_WM_ACTION_MAXIMIZE_VERT",
        "_NET_WM_ACTION_MINIMIZE",
        "_NET_WM_ACTION_MOVE",
        "_NET_WM_ACTION_RESIZE",
        "_NET_WM_ACTION_SHADE",
        "_NET_WM_ACTION_STICK",
        "_NET_WM_ALLOWED_ACTIONS",
        "_NET_WM_CONTEXT_HELP",
        "_NET_WM_DESKTOP",
        "_NET_WM_ICON",
        "_NET_WM_ICON_GEOMETRY",
        "_NET_WM_ICON_NAME",
        "_NET_WM_MOVERESIZE",
        "_NET_WM_NAME",
        "_NET_WM_PID",
        "_NET_WM_PING",
        "_NET_WM_WINDOW_OPACITY",
        "_NET_WM_WINDOW_OPACITY_LOCKED",
        "_NET_WM_STATE",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STRUT",
        "_NET_WM_STRUT_PARTIAL",
        "_NET_WM_SYNC_REQUEST",
        "_NET_WM_SYNC_REQUEST_COUNTER",
        "_NET_WM_USER_TIME",
        "_NET_WM_USER_TIME_WINDOW",
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP",
        "_NET_WM_WINDOW_TYPE_DIALOG",
        "_NET_WM_WINDOW_TYPE_DOCK",
        "_NET_WM_WINDOW_TYPE_MENU",
        "_NET_WM_WINDOW_TYPE_NORMAL",
        "_NET_WM_WINDOW_TYPE_SPLASH",
        "_NET_WM_WINDOW_TYPE_TOOLBAR",
        "_NET_WM_WINDOW_TYPE_UTILITY",
        "_NET_WORKAREA",
        "SM_CLIENT_ID",
        "UTF8_STRING",
        "WM_CHANGE_STATE",
        "WM_CLIENT_LEADER",
        "WM_CLIENT_MACHINE",
        "WM_COLORMAP_WINDOWS",
        "WM_DELETE_WINDOW",
        "WM_HINTS",
        "WM_NORMAL_HINTS",
        "WM_PROTOCOLS",
        "WM_STATE",
        "WM_TAKE_FOCUS",
        "WM_TRANSIENT_FOR",
        "WM_WINDOW_ROLE",
        "XFWM4_COMPOSITING_MANAGER",
        "XFWM4_TIMESTAMP_PROP",
        "_XROOTPMAP_ID",
        "_XSETROOT_ID",
        "MANAGER"
    };

    return (XInternAtoms (d, (char **) atomNames, ATOM_COUNT, FALSE, atoms) != 0);
}

Atom Atoms::atom(int a)
{
	if ((a >= 0) || (a <= ATOM_COUNT)) 
		return atoms[a];
	else
		return None;
}

int Atoms::windowType(Window w)
{
	Atom *state;
	int num3;
	int t = Unknown;
	
	if (!(state = (Atom *) getXAProperty(w, atoms[NET_WM_WINDOW_TYPE], XA_ATOM, &num3)))
		return t;
	while (--num3 >= 0) {
		if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DESKTOP]) t = Desktop;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DOCK]) t = Dock;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_TOOLBAR]) t = Toolbar;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_MENU]) t = Menu;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_UTILITY]) t = Utility;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_SPLASH]) t = Splash;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DIALOG]) t = Dialog;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_NORMAL]) t = Normal;
	}
	XFree(state);
	return t;
}


void *Atoms::getXAProperty(Window win, Atom prop, Atom type, int *nitems)
{
	Atom typeRet;
	int formatRet;
	unsigned long itemsRet;
	unsigned long afterRet;
	unsigned char *propData;

	propData = NULL;

	if (XGetWindowProperty(display(), win, prop, 0, 0x7fffffff, False, 
			type, &typeRet, &formatRet, &itemsRet,
			&afterRet, &propData) != Success)
		return NULL;
	
	if (nitems)
		*nitems = itemsRet;
	return propData;
}

// send protocol message to child window
void Atoms::sendWMProtocols(Window w, long data0, long data1)  
{
    XClientMessageEvent ev;

    //memset(&ev, 0, sizeof(ev));
    ev.type = ClientMessage;
    ev.window = w;
    ev.message_type = atom(WM_PROTOCOLS);
    ev.format = 32;
	ev.send_event = TRUE;
    ev.data.l[0] = data0;
    ev.data.l[1] = data1;

    XSendEvent(display(), w, FALSE, 0L, (XEvent*) &ev);
}
