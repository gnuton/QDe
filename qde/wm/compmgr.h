#ifndef COMPMGR_H
#define COMPMGR_H

#include <QObject>

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>

typedef struct _win {
    struct _win		*next;
    Window		id;
#if HAS_NAME_WINDOW_PIXMAP
    Pixmap		pixmap;
#endif
    XWindowAttributes	a;
#if CAN_DO_USABLE
    Bool		usable;		    /* mapped and all damaged at one point */
    XRectangle		damage_bounds;	    /* bounds of damage */
#endif
    int			mode;
    int			damaged;
    Damage		damage;
    Picture		picture;
    Picture		alphaPict;
    Picture		shadowPict;
    XserverRegion	borderSize;
    XserverRegion	extents;
    Picture		shadow;
    int			shadow_dx;
    int			shadow_dy;
    int			shadow_width;
    int			shadow_height;
    unsigned int	opacity;
    Atom                windowType;
    unsigned long	damage_sequence;    /* sequence when damage was created */

    /* for drawing translucent windows */
    XserverRegion	borderClip;
    struct _win		*prev_trans;
} win;

typedef struct _fade {
    struct _fade	*next;
    win			*w;
    double		cur;
    double		finish;
    double		step;
    void		(*callback) (Display *dpy, win *w, Bool gone);
    Display		*dpy;
    Bool		gone;
} fade;

typedef enum _compMode {
    CompSimple,		/* looks like a regular X server */
    CompServerShadows,	/* use window alpha for shadow; sharp, but precise */
    CompClientShadows,	/* use window extents for shadow, blurred */
} CompMode;

typedef struct _conv {
    int	    size;
    double  *data;
} conv;

typedef struct _ignore {
    struct _ignore	*next;
    unsigned long	sequence;
} ignore;

class compmgr : public QObject
{
    Q_OBJECT

public:
    explicit compmgr(QObject *parent = 0);

private:
    void init();
    bool eventFilter();
    int get_time_in_milliseconds();
    fade* find_fade(win *w);
    void dequeue_fade(Display *dpy, fade *f);
    void cleanup_fade(Display *dpy, win *w);
    void enqueue_fade (Display *dpy, fade *f);
    void set_fade(Display *dpy, win *w, double start, double finish, double step,
		  void (*callback) (Display *dpy, win *w, Bool gone),
		  Bool gone, Bool exec_callback, Bool override);
    void determine_mode(Display *dpy, win *w);
    XserverRegion win_extents(Display *dpy, win *w);
    void add_damage (Display *dpy, XserverRegion damage);
    Picture shadow_picture (Display *dpy, double opacity, Picture alpha_pict, int width, int height, int *wp, int *hp);
    XImage* make_shadow(Display *dpy, double opacity, int width, int height);
    unsigned char sum_gaussian(conv *map, double opacity, int x, int y, int width, int height);
    int fade_timeout(void);
    void run_fades(Display *dpy);
    double gaussian (double r, double x, double y);
    conv* make_gaussian_map(Display *dpy, double r);
    void presum_gaussian(conv *map);
    Picture solid_picture(Display *dpy, Bool argb, double a, double r, double g, double b);
    void discard_ignore(Display *dpy, unsigned long sequence);
    void set_ignore(Display *dpy, unsigned long sequence);
    int should_ignore(Display *dpy, unsigned long sequence);
    win* find_win(Display *dpy, Window id);
    Picture root_tile(Display *dpy);
    void paint_root (Display *dpy);
    XserverRegion border_size(Display *dpy, win *w);
    void paint_all(Display *dpy, XserverRegion region);
    void repair_win(Display *dpy, win *w);
    void map_win(Display *dpy, Window id, unsigned long sequence, Bool fade);
    void finish_unmap_win(Display *dpy, win *w);
    unsigned int get_opacity_prop(Display *dpy, win *w, unsigned int def);
    double get_opacity_percent(Display *dpy, win *w, double def);
    Atom get_wintype_prop(Display * dpy, Window w);
    Atom determine_wintype(Display *dpy, Window w);
    void add_win(Display *dpy, Window id, Window prev);
    void restack_win(Display *dpy, win *w, Window new_above);
    void configure_win(Display *dpy, XConfigureEvent *ce);
    void circulate_win(Display *dpy, XCirculateEvent *ce);
    void finish_destroy_win(Display *dpy, Window id, Bool gone);
    void destroy_win(Display *dpy, Window id, Bool gone, Bool fade);
    void damage_win(Display *dpy, XDamageNotifyEvent *de);
    int error(Display *dpy, XErrorEvent *ev);
    void expose_root(Display *dpy, Window root, XRectangle *rects, int nrects);
    int ev_serial(XEvent *ev);
    char* ev_name(XEvent *ev);
    Window ev_window(XEvent *ev);
    void register_cm(void);

public slots:

private:
    fade *fades;
    double fade_in_step;
    double fade_out_step;
    int	fade_time;
    int	fade_delta;
    CompMode compMode;

    Atom opacityAtom;
    Atom winTypeAtom;
    Atom winDesktopAtom;
    Atom winDockAtom;
    Atom winToolbarAtom;
    Atom winMenuAtom;
    Atom winUtilAtom;
    Atom winSplashAtom;
    Atom winDialogAtom;
    Atom winNormalAtom;

    bool excludeDockShadows;

    int	shadowRadius;
    int shadowOffsetX;
    int shadowOffsetY;
    double shadowOpacity;

    XserverRegion allDamage;
    Window root;
    conv *gaussianMap;

    /* For shadow precomputation */
    int Gsize;
    unsigned char *shadowCorner;
    unsigned char *shadowTop;
    ignore *ignore_head, **ignore_tail;
    win *list;
    int	scr;
    Picture rootTile;
    Picture rootPicture;
    Picture rootBuffer;
    int	root_height, root_width;
    bool clipChanged;
    Picture blackPicture;
    Picture transBlackPicture;
    bool fadeWindows;
    int	composite_opcode;
    int xfixes_event, xfixes_error;
    int	damage_event, damage_error;
    int	composite_event, composite_error;
    int	render_event, render_error;
    Display *dpy;
    bool autoRedirect;
};

#endif // COMPMGR_H
