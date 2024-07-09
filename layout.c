/* macros */
#define GETINC(X)               ((X) - 2000)
#define INC(X)                  ((X) + 2000)
#define ISINC(X)                ((X) > 1000 && (X) < 3000)
#define PREVSEL                 3000
#define MOD(N,M)                ((N)%(M) < 0 ? (N)%(M) + (M) : (N)%(M))

/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void incrgaps(const Arg *arg);
static void togglegaps(const Arg *arg);
static void focusstack(const Arg *arg);
static void pushstack(const Arg *arg);

/* Layouts */
static void tile(Monitor *);
static void bstack(Monitor *m);

/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static void setgaps(int oh, int ov, int ih, int iv);
static int stackpos(const Arg *arg);

/* Settings */
static int enablegaps = 1;

static void
setgaps(int oh, int ov, int ih, int iv)
{
	if (oh < 0) oh = 0;
	if (ov < 0) ov = 0;
	if (ih < 0) ih = 0;
	if (iv < 0) iv = 0;

	selmon->gappoh = oh;
	selmon->gappov = ov;
	selmon->gappih = ih;
	selmon->gappiv = iv;
	arrange(selmon);
}

static void
togglegaps(const Arg *arg)
{
	enablegaps = !enablegaps;
	arrange(NULL);
}

static void
defaultgaps(const Arg *arg)
{
	setgaps(gappoh, gappov, gappih, gappiv);
}

static void
incrgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov + arg->i,
		selmon->gappih + arg->i,
		selmon->gappiv + arg->i
	);
}

static void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc)
{
	unsigned int n, oe, ie;
	oe = ie = enablegaps;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gappoh*oe; // outer horizontal gap
	*ov = m->gappov*oe; // outer vertical gap
	*ih = m->gappih*ie; // inner horizontal gap
	*iv = m->gappiv*ie; // inner vertical gap
	*nc = n;            // number of clients
}

void
getfacts(Monitor *m, int msize, int ssize, float *mf, float *sf, int *mr, int *sr)
{
	unsigned int n;
	float mfacts, sfacts;
	int mtotal = 0, stotal = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	mfacts = MIN(n, m->nmaster);
	sfacts = n - m->nmaster;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		if (n < m->nmaster)
			mtotal += msize / mfacts;
		else
			stotal += ssize / sfacts;

	*mf = mfacts; // total factor of master area
	*sf = sfacts; // total factor of stack area
	*mr = msize - mtotal; // the remainder (rest) of pixels after an even master split
	*sr = ssize - stotal; // the remainder (rest) of pixels after an even stack split
}

/***
 * Layouts
 */

/*
 * Default tile layout + gaps
 */

static void
tile(Monitor *m)
{
        unsigned int i, n;
        int oh, ov, ih, iv;
        int mx = 0, my = 0, mh = 0, mw = 0;
        int sx = 0, sy = 0, sh = 0, sw = 0;
        float mfacts, sfacts;
        int mrest, srest;
        Client *c;

        getgaps(m, &oh, &ov, &ih, &iv, &n);
        if (n == 0)
                return;

        sx = mx = m->wx + ov;
        sy = my = m->wy + oh;
        mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
        sh = m->wh - 2*oh - ih * (n - m->nmaster - 1);
        sw = mw = m->ww - 2*ov;

        if (m->nmaster && n > m->nmaster) {
                sw = (mw - iv) * (1 - m->mfact);
                mw = mw - iv - sw;
                sx = mx + mw + iv;
        }

        getfacts(m, mh, sh, &mfacts, &sfacts, &mrest, &srest);

        for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
                if (i < m->nmaster) {
                        resize(c, mx, my, mw - (2*c->bw), (mh / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), 0);
                        my += HEIGHT(c) + ih;
                } else {
                        resize(c, sx, sy, sw - (2*c->bw), (sh / sfacts) + ((i - m->nmaster) < srest ? 1 : 0) - (2*c->bw), 0);
                        sy += HEIGHT(c) + ih;
                }
}

/*
 * Bottomstack layout + gaps
 * https://dwm.suckless.org/patches/bottomstack/
 */
static void
bstack(Monitor *m)
{
        unsigned int i, n;
        int oh, ov, ih, iv;
        int mx = 0, my = 0, mh = 0, mw = 0;
        int sx = 0, sy = 0, sh = 0, sw = 0;
        float mfacts, sfacts;
        int mrest, srest;
        Client *c;

        getgaps(m, &oh, &ov, &ih, &iv, &n);
        if (n == 0)
                return;

        sx = mx = m->wx + ov;
        sy = my = m->wy + oh;
        sh = mh = m->wh - 2*oh;
        mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);
        sw = m->ww - 2*ov - iv * (n - m->nmaster - 1);

        if (m->nmaster && n > m->nmaster) {
                sh = (mh - ih) * (1 - m->mfact);
                mh = mh - ih - sh;
                sx = mx;
                sy = my + mh + ih;
        }

        getfacts(m, mw, sw, &mfacts, &sfacts, &mrest, &srest);

        for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
                if (i < m->nmaster) {
                        resize(c, mx, my, (mw / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), mh - (2*c->bw), 0);
                        mx += WIDTH(c) + iv;
                } else {
                        resize(c, sx, sy, (sw / sfacts) + ((i - m->nmaster) < srest ? 1 : 0) - (2*c->bw), sh - (2*c->bw), 0);
                        sx += WIDTH(c) + iv;
                }
        }
}

int
stackpos(const Arg *arg) {
	int n, i;
	Client *c, *l;

	if(!selmon->clients)
		return -1;

	if(arg->i == PREVSEL) {
		for(l = selmon->stack; l && (!ISVISIBLE(l) || l == selmon->sel); l = l->snext);
		if(!l)
			return -1;
		for(i = 0, c = selmon->clients; c != l; i += ISVISIBLE(c) ? 1 : 0, c = c->next);
		return i;
	}
	else if(ISINC(arg->i)) {
		if(!selmon->sel)
			return -1;
		for(i = 0, c = selmon->clients; c != selmon->sel; i += ISVISIBLE(c) ? 1 : 0, c = c->next);
		for(n = i; c; n += ISVISIBLE(c) ? 1 : 0, c = c->next);
		return MOD(i + GETINC(arg->i), n);
	}
	else if(arg->i < 0) {
		for(i = 0, c = selmon->clients; c; i += ISVISIBLE(c) ? 1 : 0, c = c->next);
		return MAX(i + arg->i, 0);
	}
	else
		return arg->i;
}

void
focusstack(const Arg *arg)
{
	int i = stackpos(arg);
	Client *c, *p;

	if(i < 0)
		return;

	for(p = NULL, c = selmon->clients; c && (i || !ISVISIBLE(c));
	    i -= ISVISIBLE(c) ? 1 : 0, p = c, c = c->next);
	focus(c ? c : p);
	restack(selmon);
}

void
pushstack(const Arg *arg) {
	int i = stackpos(arg);
	Client *sel = selmon->sel, *c, *p;

	if(i < 0)
		return;
	else if(i == 0) {
		detach(sel);
		attach(sel);
	}
	else {
		for(p = NULL, c = selmon->clients; c; p = c, c = c->next)
			if(!(i -= (ISVISIBLE(c) && c != sel)))
				break;
		c = c ? c : p;
		detach(sel);
		sel->next = c->next;
		c->next = sel;
	}
	arrange(selmon);
}
