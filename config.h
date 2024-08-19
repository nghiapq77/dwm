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
    /* class              instance     title                    tags mask    isfloating    monitor */
    { "LibreWolf",        NULL,        NULL,                    1,           0,            -1 },
    { "st-256color",      "tm",        NULL,                    1 << 1,      0,            -1 },
    { "Brave-browser",    NULL,        NULL,                    1 << 2,      0,            -1 },
    { "st-256color",      "lf",        NULL,                    1 << 3,      0,            -1 },
    { NULL,               "spterm",    NULL,                    SPTAG(0),    1,            -1 },
    { NULL,               NULL,        "Picture-in-Picture",    0,           1,            -1 },
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

#define KEY(MOD,KEY,FUNC,ARG) \
    &((Keychord){1, {{MOD, KEY}}, FUNC, ARG}),

#define KEYCHORD(CHORD,KEY,FUNC,ARG) \
    &((Keychord){2, {{MODKEY, CHORD}, {0, KEY}}, FUNC, ARG}),

#define TAGKEYS(KEY,TAG) \
    &((Keychord){1, {{MODKEY,             KEY}},    view,          {.ui = 1 << TAG}}), \
    &((Keychord){1, {{MODKEY|ALTKEY,      KEY}},    toggleview,    {.ui = 1 << TAG}}), \
    &((Keychord){1, {{MODKEY|ShiftMask,   KEY}},    tag,           {.ui = 1 << TAG}}), \
    &((Keychord){1, {{MODKEY|ControlMask, KEY}},    toggletag,     {.ui = 1 << TAG}}),

#define STACKKEYS(MOD,ACTION) \
    &((Keychord){1, {{MOD|ALTKEY, XK_j}},      ACTION##stack,    {.i = INC(+1)}}), \
    &((Keychord){1, {{MOD|ALTKEY, XK_k}},      ACTION##stack,    {.i = INC(-1)}}), \
    &((Keychord){1, {{MOD, XK_semicolon}},     ACTION##stack,    {.i = INC(+1)}}), \
    &((Keychord){1, {{MOD, XK_apostrophe}},    ACTION##stack,    {.i = INC(-1)}}), \
    &((Keychord){1, {{MOD, XK_Tab}},           ACTION##stack,    {.i = PREVSEL}}),

#define KEYEVENT(SRC_MOD,SRC_KEY,DST_MOD,DST_KEY) \
    &((Keychord){1, {{SRC_MOD, SRC_KEY}}, sendkeyevent, {.v = &(const KeyBinding){DST_MOD, DST_KEY}}}),

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

#include <X11/XF86keysym.h>
static Keychord *keychords[] = {
    /* key                                                     function          argument */
    TAGKEYS(XK_1,                                                                0)
    TAGKEYS(XK_2,                                                                1)
    TAGKEYS(XK_3,                                                                2)
    TAGKEYS(XK_4,                                                                3)
    TAGKEYS(XK_5,                                                                4)
    TAGKEYS(XK_6,                                                                5)
    TAGKEYS(XK_7,                                                                6)
    TAGKEYS(XK_8,                                                                7)
    TAGKEYS(XK_9,                                                                8)

    STACKKEYS(MODKEY,                                          focus)
    STACKKEYS(MODKEY|ShiftMask,                                push)

    KEYEVENT(MODKEY,              XK_a,                                          0, XK_Return)
    KEYEVENT(MODKEY,              XK_k,                                          0, XK_Up)
    KEYEVENT(MODKEY,              XK_j,                                          0, XK_Down)
    KEYEVENT(MODKEY,              XK_h,                                          0, XK_Left)
    KEYEVENT(MODKEY,              XK_l,                                          0, XK_Right)
    KEYEVENT(MODKEY,              XK_s,                                          0, XK_Up)
    KEYEVENT(MODKEY,              XK_x,                                          0, XK_Down)
    KEYEVENT(MODKEY,              XK_z,                                          0, XK_Left)
    KEYEVENT(MODKEY,              XK_c,                                          0, XK_Right)

    KEY(MODKEY,                   XK_grave,                    view,             {0})
    KEY(MODKEY,                   XK_0,                        view,             {.ui = ~0})
    KEY(MODKEY|ShiftMask,         XK_0,                        tag,              {.ui = ~0})
    KEY(MODKEY,                   XK_equal,                    spawn,            SPAWN("dmenu_calc"))
    KEY(MODKEY,                   XK_BackSpace,                spawn,            SPAWN("sysact"))
    KEY(MODKEY,                   XK_q,                        spawn,            SPAWN("defapp"))
    KEY(MODKEY|ShiftMask,         XK_q,                        quit,             {0})
    KEY(MODKEY|ALTKEY,            XK_q,                        quit,             {1})
    KEY(MODKEY,                   XK_w,                        spawn,            SPAWN(BROWSER))
    KEY(MODKEY,                   XK_e,                        spawn,            SHCMD("toggle_lang; pkill -39 dwmblocks"))
    KEY(MODKEY,                   XK_r,                        spawn,            SPAWN(TERMINAL, "-n", "lf", "-e", "lftm"))
    KEY(MODKEY,                   XK_t,                        setlayout,        {.v = &layouts[0]})
    KEY(MODKEY|ShiftMask,         XK_t,                        setlayout,        {.v = &layouts[1]})
    KEY(MODKEY|ALTKEY,            XK_t,                        setlayout,        {.v = &layouts[2]})
    KEY(MODKEY,                   XK_p,                        spawn,            SPAWN("dmenu_passmenu"))
    KEY(MODKEY|ShiftMask,         XK_p,                        spawn,            SPAWN("dmenu_passmenu", "--type"))
    KEY(MODKEY,                   XK_bracketleft,              shiftview,        {.i = -1})
    KEY(MODKEY,                   XK_bracketright,             shiftview,        {.i = +1})
    KEY(MODKEY,                   XK_backslash,                view,             {0})
    KEY(MODKEY,                   XK_d,                        spawn,            {.v = dmenucmd})
    KEY(MODKEY,                   XK_f,                        togglefullscr,    {0})
    KEY(MODKEY|ShiftMask,         XK_f,                        togglefloating,   {0})
    KEY(MODKEY,                   XK_g,                        incrgaps,         {.i = +1})
    KEY(MODKEY|ShiftMask,         XK_g,                        incrgaps,         {.i = -1})
    KEY(MODKEY|ALTKEY,            XK_g,                        defaultgaps,      {0})
    KEY(MODKEY|ControlMask,       XK_g,                        togglegaps,       {0})
    KEY(MODKEY,                   XK_Return,                   spawn,            {.v = termcmd})
    KEY(MODKEY|ShiftMask,         XK_Return,                   togglescratch,    {.ui = 0})
    KEY(MODKEY|ShiftMask,         XK_c,                        killclient,       {0})
    KEY(MODKEY,                   XK_b,                        spawn,            SHCMD("refbar"))
    KEY(MODKEY|ShiftMask,         XK_b,                        togglebar,        {0})
    KEY(MODKEY,                   XK_n,                        spawn,            SPAWN("toggle_redshift"))
    KEY(MODKEY|ShiftMask,         XK_space,                    zoom,             {0})
    KEY(ControlMask,              XK_space,                    spawn,            SHCMD("dunstctl close"))
    KEY(ControlMask|ShiftMask,    XK_space,                    spawn,            SHCMD("dunstctl close-all"))
    KEY(MODKEY,                   XK_comma,                    setmfact,         {.f = -0.05})
    KEY(MODKEY,                   XK_period,                   setmfact,         {.f = +0.05})
    KEY(0,                        XK_Print,                    spawn,            SPAWN("dmenu_maimpick"))
    KEY(MODKEY,                   XK_Print,                    spawn,            SHCMD("maim -s | xclip -selection clipboard -t image/png"))
    KEY(ALTKEY,                   XK_Print,                    spawn,            SHCMD("maim ~/pics/pic-full-$(date '+%y%m%d-%H%M-%S').png"))
    KEY(MODKEY,                   XK_Left,                     focusmon,         {.i = -1})
    KEY(MODKEY|ShiftMask,         XK_Left,                     tagmon,           {.i = -1})
    KEY(MODKEY,                   XK_Right,                    focusmon,         {.i = +1})
    KEY(MODKEY|ShiftMask,         XK_Right,                    tagmon,           {.i = +1})
    KEY(MODKEY,                   XK_Escape,                   spawn,            SPAWN("sysact"))
    KEY(0,                        XF86XK_AudioMute,            spawn,            SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; pkill -38 dwmblocks && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:volume -h int:value:$(get-volume percent) \"Audio: $(get-volume)\""))
    KEY(0,                        XF86XK_AudioLowerVolume,     spawn,            SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- -l 1.5; pkill -38 dwmblocks && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:volume -h int:value:$(get-volume percent) \"Audio: $(get-volume)\""))
    KEY(MODKEY,                   XF86XK_AudioLowerVolume,     spawn,            SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 25%- -l 1.5; pkill -38 dwmblocks && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:volume -h int:value:$(get-volume percent) \"Audio: $(get-volume)\""))
    KEY(0,                        XF86XK_AudioRaiseVolume,     spawn,            SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ -l 1.5; pkill -38 dwmblocks && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:volume -h int:value:$(get-volume percent) \"Audio: $(get-volume)\""))
    KEY(MODKEY,                   XF86XK_AudioRaiseVolume,     spawn,            SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 25%+ -l 1.5; pkill -38 dwmblocks && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:volume -h int:value:$(get-volume percent) \"Audio: $(get-volume)\""))
    KEY(0,                        XF86XK_AudioMicMute,         spawn,            SPAWN("bluetooth_quicktoggle"))
    KEY(MODKEY,                   XF86XK_AudioMicMute,         spawn,            SPAWN("dmenu_bluetooth"))
    KEY(0,                        XF86XK_MonBrightnessDown,    spawn,            SHCMD("brightness -dec 10 && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:brightness -h int:value:$(brightness -get) \"Brightness: $(brightness -get)%\""))
    KEY(MODKEY,                   XF86XK_MonBrightnessDown,    spawn,            SHCMD("brightness -dec 25 && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:brightness -h int:value:$(brightness -get) \"Brightness: $(brightness -get)%\""))
    KEY(0,                        XF86XK_MonBrightnessUp,      spawn,            SHCMD("brightness -inc 10 && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:brightness -h int:value:$(brightness -get) \"Brightness: $(brightness -get)%\""))
    KEY(MODKEY,                   XF86XK_MonBrightnessUp,      spawn,            SHCMD("brightness -inc 25 && notify-send -u low -t 1000 -h string:x-dunst-stack-tag:brightness -h int:value:$(brightness -get) \"Brightness: $(brightness -get)%\""))
    KEY(0,                        XF86XK_Display,              spawn,            SPAWN("toggle_hdmi"))
    KEY(0,                        XF86XK_WLAN,                 spawn,            SHCMD("notify-send -h string:x-dunst-stack-tag:wlan 'WLAN' \"$([ \"$(cat /sys/class/net/w*/operstate 2>/dev/null)\" = 'up' ] && echo 'Off' || echo 'On')\"; sleep 2.5s && pkill -44 dwmblocks"))
    KEY(0,                        XF86XK_Tools,                spawn,            SPAWN("toggle_touchpad"))
    KEY(0,                        XF86XK_Bluetooth,            spawn,            SHCMD("notify-send -h string:x-dunst-stack-tag:bluetooth 'Bluetooth' \"$([ -f \"$XDG_CACHE_HOME/.bluetooth_on\" ] && echo 'Turning Off' || echo 'Turning On')\" && bluetooth_refresh"))
    KEY(0,                        XF86XK_Launch1,              spawn,            SHCMD("toggle_lang; pkill -39 dwmblocks"))
    KEY(0,                        XF86XK_Favorites,            togglescratch,    {.ui = 0})
    KEY(MODKEY,                   XF86XK_Favorites,            spawn,            SPAWN("dmenu_battery"))
    KEY(0,                        XK_Insert,                   spawn,            SHCMD("xdotool type $(grep -v '^#' ~/.local/share/bm | dmenu -p \"Bookmarks:\" -i -l 20 | cut -d' ' -f1)"))

    KEYCHORD(XK_space,            XK_w,                        spawn,            SPAWN("dmenu_wifi"))
    KEYCHORD(XK_space,            XK_t,                        spawn,            SPAWN(TERMINAL, "-e", "btop"))
    KEYCHORD(XK_space,            XK_a,                        spawn,            SPAWN("pavucontrol"))
    KEYCHORD(XK_space,            XK_f,                        spawn,            SPAWN(TERMINAL, "-n", "lf", "-e", "lftm"))
    KEYCHORD(XK_space,            XK_c,                        spawn,            SPAWN(TERMINAL, "-e", "calcurse"))
    KEYCHORD(XK_space,            XK_n,                        spawn,            SPAWN("dmenu_noti"))
    KEYCHORD(XK_space,            XK_m,                        spawn,            SPAWN(TERMINAL, "-e", "neomutt"))
    KEYCHORD(XK_space,            XK_space,                    spawn,            SPAWN(TERMINAL, "-n", "tm", "-e", "tm"))
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
