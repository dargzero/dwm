/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>						/* multimedia keys */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { 
	"Noto Sans Display:size=11",
	"Font Awesome 5 Free:size=11",
	"Font Awesome 5 Brands:size=11"
};
static const char dmenufont[]       = "Noto Sans Display:size=11";

static const char col_gray[]  = "#373737";
static const char col_blue[]  = "#005b96";
static const char col_black[] = "#000000";
static const char col_white[] = "#fefefe";

static const char *colors[][3]      = {
	/*                    fg         bg      border   */
	[SchemeNorm]      = { col_white, col_gray, col_black },
    [SchemeSel]       = { col_white, col_gray, col_blue  },
    [SchemeStatus]    = { col_white, col_gray, col_black },	// border unused (cannot be null)
    [SchemeTagsSel]   = { col_white, col_blue, col_black }, // border unused (cannot be null)
    [SchemeTagsNorm]  = { col_white, col_gray, col_black }, // border unused (cannot be null)
    [SchemeInfoSel]   = { col_white, col_gray, col_black }, // border unused (cannot be null)
    [SchemeInfoNorm]  = { col_white, col_gray, col_black }, // border unused (cannot be null)
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance  title               tags mask  isfloating  isterminal  noswallow  monitor */
	{ NULL,         NULL,     "Android Emulator", 0,         1,          0,           0,        -1 },
	{ "alacritty",  NULL,     NULL,               0,         0,          1,           0,        -1 },
	{ "mpv",        NULL,     NULL,               0,         1,          0,           0,        -1 },
	{ NULL,         NULL,     "Event Tester",     0,         1,          0,           1,        -1 }, 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon,
        "-fn", dmenufont, "-nb", col_gray, "-nf", col_white,
        "-sb", col_blue, "-sf", col_white, NULL };

static const char *termcmd[]        = { "alacritty", NULL };
static const char *webcmd[]         = { "librewolf", NULL };
static const char *privwebcmd[]     = { "librewolf", "--private-window", NULL };
static const char *lockcmd[]        = { "slock", NULL };
static const char *steamcmd[]       = { "steam", NULL };

static Key keys[] = {
	/* modifier                     key           function        argument */
	{ MODKEY,                       XK_p,         spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_KP_Enter,  spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_s,         spawn,          {.v = steamcmd } },
	{ MODKEY|ShiftMask,             XK_w,         spawn,          {.v = webcmd } },
	{ MODKEY|ShiftMask|ControlMask, XK_w,         spawn,          {.v = privwebcmd } },
	{ MODKEY|ShiftMask,             XK_l,         spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_k,         spawn,          SHCMD("~/helpers/toggle-keyboard.sh; okki-refresh layout") },
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	{ MODKEY,                       XK_Left,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,         focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,         focusstack,     {.i = -1 } },
	{ MODKEY,						XK_Up,        pushup,         {0} },
	{ MODKEY,						XK_k,         pushup,         {0} },
	{ MODKEY,                       XK_Down,      pushdown,       {0} },
	{ MODKEY,                       XK_j,         pushdown,       {0} },
	{ MODKEY|ControlMask,           XK_Up,        incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,         incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_Down,      incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,         incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Left,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_Right,     setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_KP_Enter,  zoom,           {0} },
	{ MODKEY,                       XK_Return,    zoom,           {0} },
	{ MODKEY,                       XK_Tab,       view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,         killclient,     {0} },
	{ MODKEY,                       XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,     setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,     togglefloating, {0} },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	/* Media keys */
	{ 0,           XF86XK_AudioRaiseVolume,      spawn,          SHCMD("pamixer --allow-boost --increase 1; okki-refresh volume") },
	{ 0,           XF86XK_AudioLowerVolume,      spawn,          SHCMD("pamixer --allow-boost --decrease 1; okki-refresh volume") },
	{ ShiftMask,   XF86XK_AudioRaiseVolume,      spawn,          SHCMD("pamixer --allow-boost --increase 10; okki-refresh volume") },
	{ ShiftMask,   XF86XK_AudioLowerVolume,      spawn,          SHCMD("pamixer --allow-boost --decrease 10; okki-refresh volume") },
	{ 0,           XF86XK_AudioMute,             spawn,          SHCMD("pamixer --toggle-mute; okki-refresh volume") },
	{ 0,           XF86XK_MonBrightnessUp,       spawn,          SHCMD("pkexec brillo -A 1; okki-refresh brightness") },
	{ 0,           XF86XK_MonBrightnessDown,     spawn,          SHCMD("pkexec brillo -U 1; okki-refresh brightness") },
	{ ShiftMask,   XF86XK_MonBrightnessUp,       spawn,          SHCMD("pkexec brillo -A 10 -u 500; okki-refresh brightness") },
	{ ShiftMask,   XF86XK_MonBrightnessDown,     spawn,          SHCMD("pkexec brillo -U 10 -u 500; okki-refresh brightness") },
	{ 0,           XK_Print,                     spawn,          SHCMD("sleep 0.2s; scrot --quality 100 -e 'mv $f ~/Screenshots'") },
	{ ShiftMask,   XK_Print,                     spawn,          SHCMD("sleep 0.2s; scrot --focused --quality 100 -e 'mv $f ~/Screenshots'") },
	{ ControlMask, XK_Print,                     spawn,          SHCMD("sleep 0.2s; scrot --select --quality 100 -e 'mv $f ~/Screenshots'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

