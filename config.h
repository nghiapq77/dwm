/* constants */
#define TERMINAL "st"
#define BROWSER "librewolf"

/* appearance */
static const unsigned int borderpx  = 1;  // border pixel of windows
static const unsigned int snap      = 32; // snap pixel
static const unsigned int gappih    = 30; // horiz inner gap between windows
static const unsigned int gappiv    = 30; // vert inner gap between windows
static const unsigned int gappoh    = 30; // horiz outer gap between windows and screen edge
static const unsigned int gappov    = 30; // vert outer gap between windows and screen edge
static int smartgaps                = 1;  // 1 means no outer gap when there is only one window
static const int showbar            = 1;  // 0 means no bar
static const int topbar             = 1;  // 0 means bottom bar
static const char *fonts[]          = { "IBM Plex Mono:pixelsize=18", "siji:size=12", "Symbols Nerd Font:size=14" };
static const char *colors[][3]      = {
    /*                   fg            bg            border    */
    [SchemeNorm]     = { "#ebdbb2",    "#1d2021",    "#444444" },
    [SchemeSel]      = { "#eeeeee",    "#00afff",    "#8ec07c" },
    [SchemeStatus]   = { "#ebdbb2",    "#1d2021",    "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsNorm] = { "#bbbbbb",    "#1d2021",    "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = { "#1d2021",    "#8ec07c",    "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm] = { "#bbbbbb",    "#1d2021",    "#000000" }, // Infobar middle unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { "#f9f5d7",    "#1d2021",    "#000000" }, // Infobar middle selected {text,background,not used but cannot be empty}
};

/* scratchpads */
typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = { TERMINAL, "-n", "spterm", "-g", "80x24", "-f", "Monaco:size=14:antialias=true:autohint=true", NULL };
static Sp scratchpads[] = {
    /* name       cmd */
    {"spterm",    spcmd1},
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
    /* class    instance     title                    tags mask    isfloating    monitor */
    { NULL,     "spterm",    NULL,                    SPTAG(0),    1,            -1 },
    { NULL,     NULL,        "Picture-in-Picture",    0,           1,            -1 },
};

/* layout(s) */
#include "layout.c"
static const float mfact        = 0.55; // factor of master area size [0.05..0.95]
static const int nmaster        = 1;    // number of clients in master area
static const int resizehints    = 0;    // 1 means respect size hints in tiled resizals
static const int refreshrate    = 120;  // refresh rate (per second) for client move/resize

static const Layout layouts[] = {
    /* symbol    arrange function    */
    { "",       tile   },           // Default: Master on left, slaves on right
    { "",       bstack },           // Master on top, slaves on bottom
    { "",       NULL   },           // No layout function means floating behavior
};

/* key definitions */
#define MODKEY Mod4Mask // Super key
#define ALTKEY Mod1Mask // Alt key

#define TAGKEYS(KEY,TAG) \
    { MODKEY,                KEY,    view,          {.ui = 1 << TAG} }, \
    { MODKEY|ALTKEY,         KEY,    toggleview,    {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,      KEY,    tag,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,    KEY,    toggletag,     {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
    { MOD,    XK_j,      ACTION##stack,    {.i = INC(+1)} }, \
    { MOD,    XK_k,      ACTION##stack,    {.i = INC(-1)} }, \
    { MOD,    XK_Tab,    ACTION##stack,    {.i = PREVSEL} }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for spawning shell-less commands */
#define SPAWN(...) { .v = (const char*[]){ __VA_ARGS__, NULL } }

/* define status bar */
#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static const Key keys[] = {
    /* modifier                    key              function           argument */
    STACKKEYS(MODKEY,                               focus)
    STACKKEYS(MODKEY|ShiftMask,                     push)
    TAGKEYS(                       XK_1,                               0)
    TAGKEYS(                       XK_2,                               1)
    TAGKEYS(                       XK_3,                               2)
    TAGKEYS(                       XK_4,                               3)
    TAGKEYS(                       XK_5,                               4)
    TAGKEYS(                       XK_6,                               5)
    TAGKEYS(                       XK_7,                               6)
    TAGKEYS(                       XK_8,                               7)
    TAGKEYS(                       XK_9,                               8)
    { MODKEY,                      XK_0,            view,              {.ui = ~0} },
    { MODKEY|ShiftMask,            XK_0,            tag,               {.ui = ~0} },
    { MODKEY,                      XK_Escape,       spawn,             SPAWN("sysact") },
    { MODKEY,                      XK_BackSpace,    spawn,             SPAWN("sysact") },
    { MODKEY,                      XK_Return,       spawn,             {.v = termcmd} },
    { MODKEY|ShiftMask,            XK_Return,       togglescratch,     {.ui = 0} },
    { MODKEY|ShiftMask,            XK_q,            quit,              {0} },
    { MODKEY,                      XK_w,            spawn,             SPAWN(BROWSER) },
    { MODKEY,                      XK_t,            setlayout,         {.v = &layouts[0]} },
    { MODKEY|ShiftMask,            XK_t,            setlayout,         {.v = &layouts[1]} },
    { MODKEY|ALTKEY,               XK_t,            setlayout,         {.v = &layouts[2]} },
    { MODKEY,                      XK_d,            spawn,             {.v = dmenucmd} },
    { MODKEY,                      XK_f,            togglefullscr,     {0} },
    { MODKEY|ShiftMask,            XK_f,            togglefloating,    {0} },
    { MODKEY,                      XK_g,            incrgaps,          {.i = +1} },
    { MODKEY|ShiftMask,            XK_g,            incrgaps,          {.i = -1} },
    { MODKEY|ALTKEY,               XK_g,            defaultgaps,       {0} },
    { MODKEY|ControlMask,          XK_g,            togglegaps,        {0} },
    /* j and k are automatically bound above in STACKEYS */
    { MODKEY|ShiftMask,            XK_c,            killclient,        {0} },
    { MODKEY|ShiftMask,            XK_b,            togglebar,         {0} },
    { MODKEY|ShiftMask,            XK_space,        zoom,              {0} },
    { MODKEY,                      XK_comma,        setmfact,          {.f = -0.05} },
    { MODKEY,                      XK_period,       setmfact,          {.f = +0.05} },
    { MODKEY,                      XK_Left,         focusmon,          {.i = -1} },
    { MODKEY|ShiftMask,            XK_Left,         tagmon,            {.i = -1} },
    { MODKEY,                      XK_Right,        focusmon,          {.i = +1} },
    { MODKEY|ShiftMask,            XK_Right,        tagmon,            {.i = +1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click            event mask           button      function         argument */
    { ClkTagBar,        0,                   Button1,    view,            {0} },
    { ClkTagBar,        0,                   Button3,    toggleview,      {0} },
    { ClkTagBar,        MODKEY,              Button1,    tag,             {0} },
    { ClkTagBar,        MODKEY,              Button3,    toggletag,       {0} },
    { ClkLtSymbol,      0,                   Button1,    setlayout,       {0} },
    { ClkStatusText,    0,                   Button1,    sigstatusbar,    {.i = 1} },
    { ClkStatusText,    0,                   Button2,    sigstatusbar,    {.i = 2} },
    { ClkStatusText,    0,                   Button3,    sigstatusbar,    {.i = 3} },
    { ClkStatusText,    MODKEY,              Button1,    sigstatusbar,    {.i = 4} },
    { ClkStatusText,    MODKEY,              Button2,    sigstatusbar,    {.i = 5} },
    { ClkStatusText,    MODKEY,              Button3,    sigstatusbar,    {.i = 6} },
    { ClkWinTitle,      0,                   Button3,    togglebar,       {0} },
    { ClkClientWin,     MODKEY,              Button1,    movemouse,       {0} },
    { ClkClientWin,     MODKEY|ShiftMask,    Button1,    resizemouse,     {0} },
    { ClkRootWin,       0,                   Button2,    spawn,           {.v = termcmd} },
    { ClkRootWin,       0,                   Button3,    togglebar,       {0} },
};
