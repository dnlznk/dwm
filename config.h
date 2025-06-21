/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 29;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "CodeNewRomanNerdFont:size=11" };
static const char dmenufont[]       = "CodeNewRomanNerdFont:size=11";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeStatus]  = { normfgcolor, normbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
       [SchemeTagsSel]  = { selfgcolor, selbgcolor,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
       [SchemeTagsNorm]  = { normfgcolor, normbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
       [SchemeInfoSel]  = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
       [SchemeInfoNorm]  = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};
static const unsigned int baralpha = 0xbe;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus]  = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsSel]  = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeInfoSel]  = { OPAQUE, baralpha, borderalpha },
    [SchemeInfoNorm] = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};

/* key definitions */
#include <X11/XF86keysym.h>
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", "dmenufont", "-nb", "normbgcolor", "-nf", "normfgcolor", "-sb", "selbordercolor", "-sf", "selfgcolor", NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", NULL };
static const char *termcmd[] = { "st", NULL };

static const char *rofiwifimenucmd[] = { "rofi-wifi-menu", NULL };
static const char *rofipasswordmenucmd[] = { "rofi-password-menu", NULL };

static const char *audiomutecmd[] = { "audio-volume-toggle", NULL };
static const char *audiodecreasecmd[] = { "audio-volume", "-2", NULL };
static const char *audioincreasecmd[] = { "audio-volume", "+2", NULL };
static const char *inputmutecmd[] = { "mic-toggle", NULL };

static const char *keyboarduscmd[] = { "keyboard-set", "us", NULL };
static const char *keyboarddecmd[] = { "keyboard-set", "de", NULL };

static const char screenshotselectioncmd[] = "maim -s -u -o | xclip -selection clipboard -t image/png -i";
static const char screenshotselectionfilecmd[] = "maim -s -u -o $HOME/Pictures/Screenshots/$(date '+%Y-%m-%d_%H:%M:%S').png";
static const char screenshotwindowcmd[] = "maim -i $(xdotool getactivewindow) -u -o | xclip -selection clipboard -t image/png -i";
static const char screenshotscreencmd[] = "maim -u -o | xclip -selection clipboard -t image/png -i";


static const Key keys[] = {
	/* modifier                     key                         function        argument */
    
    // applications
	// { MODKEY,                       XK_p,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_p,                       spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Return,                  spawn,          {.v = termcmd } },

    // rofi scripts
    { MODKEY|ShiftMask,             XK_w,                       spawn,          {.v = rofiwifimenucmd } },
    { MODKEY|ShiftMask,             XK_p,                       spawn,          {.v = rofipasswordmenucmd } },

    // audio
    { 0,                            XF86XK_AudioMute,           spawn,          {.v = audiomutecmd} },
    { 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = audiodecreasecmd} },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = audioincreasecmd } },
    { 0,                            XF86XK_AudioMicMute,        spawn,          {.v = inputmutecmd } },

    // keyboard
    { MODKEY|ControlMask,           XK_u,                       spawn,          {.v = keyboarduscmd } },
    { MODKEY|ControlMask,           XK_d,                       spawn,          {.v = keyboarddecmd } },

    // screenshot
    { MODKEY,                       XK_s,                       spawn,          SHCMD(screenshotselectioncmd) },
    { MODKEY|Mod1Mask,              XK_s,                       spawn,          SHCMD(screenshotselectionfilecmd) },
    { MODKEY|ShiftMask,             XK_s,                       spawn,          SHCMD(screenshotwindowcmd) },
    { MODKEY|ControlMask,           XK_s,                       spawn,          SHCMD(screenshotscreencmd) },

    // navigation & appearance
	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                  zoom,           {0} }, // move current window to top of the stack
	{ MODKEY,                       XK_Tab,                     view,           {0} }, // view last selected tags
    { MODKEY|ControlMask,           XK_x,                       xrdb,           {.v = NULL } },

    // layout
	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,                       setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,                       setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,                   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,                       togglefullscr,  {0} },

    // vanitygaps
	{ MODKEY|Mod1Mask,              XK_h,                       incrgaps,       {.i = +1 } }, // all
	{ MODKEY|Mod1Mask,              XK_l,                       incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,                       incrogaps,      {.i = +1 } }, // outer
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,                       incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,                       incrigaps,      {.i = +1 } }, // inner
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,                       incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,                       togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,                       defaultgaps,    {0} },
	{ MODKEY,                       XK_y,                       incrihgaps,     {.i = +1 } }, // inner horizontal
	{ MODKEY,                       XK_o,                       incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,                       incrivgaps,     {.i = +1 } }, // inner vertical
	{ MODKEY|ControlMask,           XK_o,                       incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,                       incrohgaps,     {.i = +1 } }, // outer horizontal
	{ MODKEY|Mod1Mask,              XK_o,                       incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,                       incrovgaps,     {.i = +1 } }, // outer vertical
	{ MODKEY|ShiftMask,             XK_o,                       incrovgaps,     {.i = -1 } },

    // monitor
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,         {.i = +1 } },

    // tags
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY,                       XK_0,                       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },

    // close window, restart & quit dwm
	{ MODKEY|ShiftMask,             XK_q,                       killclient,     {0} },
    { MODKEY|ControlMask|ShiftMask, XK_r,                       quit,           {1} }, // restart
	{ MODKEY|ControlMask|ShiftMask, XK_e,                       quit,           {0} }, // quit
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
