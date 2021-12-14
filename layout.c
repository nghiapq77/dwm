/* Key binding functions */
static void defaultgaps(const Arg *arg);
/*static void incnmaster(const Arg *arg);*/
static void incrgaps(const Arg *arg);
/* static void incrigaps(const Arg *arg); */
/* static void incrogaps(const Arg *arg); */
/* static void incrohgaps(const Arg *arg); */
/* static void incrovgaps(const Arg *arg); */
/* static void incrihgaps(const Arg *arg); */
/* static void incrivgaps(const Arg *arg); */
static void togglegaps(const Arg *arg);

/* Layouts */
static void deck(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void monocle(Monitor *m);
static void spiral(Monitor *m);
static void tile(Monitor *);

/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static void setgaps(int oh, int ov, int ih, int iv);

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

/*void*/
/*incnmaster(const Arg *arg)*/
/*{*/
	/*selmon->nmaster = MAX(selmon->nmaster + arg->i, 0);*/
	/*arrange(selmon);*/
/*}*/

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

/* static void */
/* incrigaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh, */
/* 		selmon->gappov, */
/* 		selmon->gappih + arg->i, */
/* 		selmon->gappiv + arg->i */
/* 	); */
/* } */

/* static void */
/* incrogaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh + arg->i, */
/* 		selmon->gappov + arg->i, */
/* 		selmon->gappih, */
/* 		selmon->gappiv */
/* 	); */
/* } */

/* static void */
/* incrohgaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh + arg->i, */
/* 		selmon->gappov, */
/* 		selmon->gappih, */
/* 		selmon->gappiv */
/* 	); */
/* } */

/* static void */
/* incrovgaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh, */
/* 		selmon->gappov + arg->i, */
/* 		selmon->gappih, */
/* 		selmon->gappiv */
/* 	); */
/* } */

/* static void */
/* incrihgaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh, */
/* 		selmon->gappov, */
/* 		selmon->gappih + arg->i, */
/* 		selmon->gappiv */
/* 	); */
/* } */

/* static void */
/* incrivgaps(const Arg *arg) */
/* { */
/* 	setgaps( */
/* 		selmon->gappoh, */
/* 		selmon->gappov, */
/* 		selmon->gappih, */
/* 		selmon->gappiv + arg->i */
/* 	); */
/* } */

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
 * Deck layout + gaps
 * https://dwm.suckless.org/patches/deck/
 */

static void
deck(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	float mfacts, sfacts;
	int mrest, srest;
	Client *c;

	int oh, ov, ih, iv;
	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sh = m->wh - 2*oh;
	}

	getfacts(m, mh, sh, &mfacts, &sfacts, &mrest, &srest);

	if (n - m->nmaster > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "D %d", n - m->nmaster);

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), (mh / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh - (2*c->bw), 0);
		}
}

/*
 * Fibonacci layout + gaps
 * https://dwm.suckless.org/patches/fibonacci/
 */

static void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n;
	int nx, ny, nw, nh;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	nx = m->wx + ov;
	ny = oh;
	nw = m->ww - 2*ov;
	nh = m->wh - 2*oh;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2*c->bw)
		   || (!(i % 2) && nw / 2 > 2*c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh = (nh - ih) / 2;
				else
					nw = (nw - iv) / 2;

				if ((i % 4) == 2 && !s)
					nx += nw + iv;
				else if ((i % 4) == 3 && !s)
					ny += nh + ih;
			}
			if ((i % 4) == 0) {
				if (s)
					ny += nh + ih;
				else
					ny -= nh + ih;
			}
			else if ((i % 4) == 1)
				nx += nw + iv;
			else if ((i % 4) == 2)
				ny += nh + ih;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw + iv;
				else
					nx -= nw + iv;
			}
			if (i == 0)	{
				if (n != 1)
					nw = (m->ww - 2*ov - iv) * m->mfact;
				ny = m->wy + oh;
			}
			else if (i == 1)
				nw = m->ww - nw - iv - 2*ov;
			i++;
		}

		resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
	}
}

/*
 * Default monocle layout
 */
void
monocle(Monitor *m)
{
        unsigned int n = 0;
        Client *c;

        for (c = m->clients; c; c = c->next)
                if (ISVISIBLE(c))
                        n++;
        if (n > 0) /* override layout symbol */
                snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
        for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
                resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

static void
spiral(Monitor *m)
{
	fibonacci(m, 0);
}

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
