/* See LICENSE file for copyright and license details. */
#include "push.c"

/* appearance */
static const char font[]            = "sans:size=9:antialias=true:hinting=true";
static const char normbordercolor[] = "#333333";
static const char normbgcolor[]     = "#223355";
static const char normfgcolor[]     = "#cccccc";
static const char selbordercolor[]  = "#116699";
static const char selbgcolor[]      = "#005588";
static const char selfgcolor[]      = "#aaddee";
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
//static const char *tags[] = { "dev", "web", "misc", "logs" };
static const char *tags[] = { "o" }; // Single view - no tags

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	{ "Chromium-browser",NULL,NULL,       2,            False,       -1 },
	{ "Firefox",  NULL,       NULL,       2,            False,       -1 },
	{ "Inkscape",  NULL,       NULL,      3,            False,       -1 },
	{ "Pcmanfm",  NULL,       NULL,       3,            False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.66667; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 2;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|||",      col },
//	{ "TTT",      bstack },
//	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
//	{ "||=",      bstackhoriz },
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
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "rxvt", NULL };
static const char *killcmd[]  = { "pkill", "-f", "startdwm", NULL };
static const char *sublcmd[]  = { "subl", NULL };
static const char *chromcmd[] = { "google-chrome", "--disable-accelerated-compositing", NULL };
//static const char *foxcmd[] = { "firefox", NULL };
static const char *inkcmd[] = { "inkscape", NULL };
static const char *gimpcmd[] = { "gimp", NULL };
static const char *pcmanfmcmd[] = { "pcmanfm", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = killcmd } },

// APPS
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },    // Terminal
	{ MODKEY,                       XK_f,      spawn,          {.v = pcmanfmcmd } }, // File Browser
	{ MODKEY,                       XK_t,      spawn,          {.v = sublcmd } },    // Text Editor
	{ MODKEY,                       XK_w,      spawn,          {.v = chromcmd } },   // Web browser
	{ MODKEY,                       XK_v,      spawn,          {.v = inkcmd } },     // Vector graphics
	{ MODKEY,                       XK_r,      spawn,          {.v = gimpcmd } },    // Raster graphics

// CLIENT MANIPULATION
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,           	XK_Down,   pushdown,       {0} },
  { MODKEY|ShiftMask,           	XK_Up,     pushup,         {0} },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ControlMask,           XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

// LAYOUTS
	{ MODKEY|ShiftMask,             XK_c,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[3]} },
//	{ MODKEY|ShiftMask,             XK_h,      setlayout,      {.v = &layouts[4]} },
//	{ MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[5]} },

	{ MODKEY,                       XK_space,  setlayout,      {0} },

// TAGS / MONITORS
	// { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	// { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	// { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	// TAGKEYS(                        XK_1,                      0)
	// TAGKEYS(                        XK_2,                      1)
	// TAGKEYS(                        XK_3,                      2)
	// TAGKEYS(                        XK_4,                      3)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

