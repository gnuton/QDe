
#include "defs.h"
#include "adx.h"

#include <stdio.h>
#include <stdlib.h>

extern Adx *deluxe;
static bool debugMode = false;

void anticoMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg:
	if (!debugMode) return;
	fprintf(stderr, "Debug: %s\n", msg);
	break;
    case QtWarningMsg:
	fprintf(stderr, "Warning: %s\n", msg);
	break;
    case QtCriticalMsg:
	fprintf(stderr, "Critical: %s\n", msg);
	break;
    case QtFatalMsg:
	fprintf(stderr, "Fatal: %s\n", msg);
	abort();
    }
}

int main(int argc, char **argv)
{
	debugMode = !qgetenv("ANTICO_DEBUG").isEmpty();
	qInstallMsgHandler(anticoMessageOutput);

        Adx a(argc, argv);

        deluxe = &a;

        XSelectInput(display(), rootWindow(), KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask |
                 PropertyChangeMask);

        XClearWindow(display(), rootWindow());


        XSync(display(), FALSE);

    return a.exec();
}

