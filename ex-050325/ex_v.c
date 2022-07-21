/*
 * This code contains changes by
 *      Gunnar Ritter, Freiburg i. Br., Germany, 2002. All rights reserved.
 *
 * Conditions 1, 2, and 4 and the no-warranty notice below apply
 * to these changes.
 *
 *
 * Copyright (c) 1980, 1993
 * 	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 * 	This product includes software developed by the University of
 * 	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * Copyright(C) Caldera International Inc. 2001-2002. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   Redistributions of source code and documentation must retain the
 *    above copyright notice, this list of conditions and the following
 *    disclaimer.
 *   Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *   All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed or owned by Caldera
 *      International, Inc.
 *   Neither the name of Caldera International, Inc. nor the names of
 *    other contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * USE OF THE SOFTWARE PROVIDED FOR UNDER THIS LICENSE BY CALDERA
 * INTERNATIONAL, INC. AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CALDERA INTERNATIONAL, INC. BE
 * LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	lint
#ifdef	DOSCCS
static char sccsid[] = "@(#)ex_v.c	1.17 (gritter) 11/27/04";
#endif
#endif

/* from ex_v.c	7.8.1 (2.11BSD GTE) 12/9/94 */

#include "ex.h"
#include "ex_re.h"
#include "ex_tty.h"
#include "ex_vis.h"

/*
 * Entry points to open and visual from command mode processor.
 * The open/visual code breaks down roughly as follows:
 * 从命令模式处理器打开和可视化的入口点。打开/可视化代码大致分解如下：
 *
 * ex_v.c	entry points, checking of terminal characteristics
 *          入口点，检查终端特性
 *
 * ex_vadj.c	logical screen control, use of intelligent operations
 *		insert/delete line and coordination with screen image;
 *		updating of screen after changes.
 *      逻辑画面控制，使用智能操作插入/删除线和与屏幕图像协调； 更改后更新屏幕。
 *		
 *
 * ex_vget.c	input of single keys and reading of input lines
 *		from the echo area, handling of memory for repeated
 *		commands and small saved texts from inserts and partline
 *		deletes, notification of multi line changes in the echo
 *		area.
 *      输入单个键并从回显区域读取输入行，处理重复命令的内存和插入和部分行删除的小保存文本，
 *       回显区域中多行更改的通知。
 *		
 *
 * ex_vmain.c	main command decoding, some command processing.
 *              主要指令解码，部分指令处理。
 *              
 *
 * ex_voperate.c   decoding of operator/operand sequences and
 *		contextual scans, implementation of word motions.
 *      操作符/操作数序列的解码和上下文扫描，单词运动的实现。
 *		
 *
 * ex_vops.c	major operator interfaces, undos, motions, deletes,
 *		changes, opening new lines, shifts, replacements and yanks
 *		coordinating logical and physical changes.
 *		主要的操作员界面、撤消、动作、删除、更改、打开新行、班次、替换和拉动协调逻辑和物理更改。
 *
 * ex_vops2.c	subroutines for operator interfaces in ex_vops.c,
 *		insert mode, read input line processing at lowest level.
 *      ex_vops.c 中操作员界面的子例程，插入模式，在最低级别读取输入行处理。
 *		
 *
 * ex_vops3.c	structured motion definitions of ( ) { } and [ ] operators,
 *		indent for lisp routines, () and {} balancing. 
 *      ( ) { } 和 [ ] 运算符的结构化运动定义，lisp 例程的缩进，() 和 {} 平衡。
 *		
 *
 * ex_vput.c	output routines, clearing, physical mapping of logical cursor
 *		positioning, cursor motions, handling of insert character
 *		and delete character functions of intelligent and unintelligent
 *		terminals, visual mode tracing routines (for debugging),
 *		control of screen image and its updating.
 *		输出例程、清除、逻辑光标定位的物理映射、光标移动、智能和非智能终端的插入字符和删除字符功能的处理，
 *       可视模式跟踪例程（用于调试）、屏幕图像控制及其更新。
 *
 * ex_vwind.c	window level control of display, forward and backward rolls,
 *		absolute motions, contextual displays, line depth determination
 *		显示的窗口水平控制，向前和向后滚动，
 *      绝对运动，上下文显示，线深度确定
 */

JMP_BUF venv;

/*
 * Enter open mode
 */
#ifdef u370
cell	atube[TUBESIZE+LBSIZE];
#endif
void 
oop(void)
{
	register char *ic;
#ifndef u370
	cell atube[TUBESIZE + LBSIZE];
#endif
	struct termios f;	/* mjm: was register */
	int resize;

	resize = SETJMP(venv);
	if (resize) {
		setsize();
		initev = (char *)0;
		inopen = 0;
		addr1 = addr2 = dot;
	}
#ifdef	SIGWINCH
	signal(SIGWINCH, onwinch);
#endif
	ovbeg();
	if (peekchar() == '/') {
		ignore(compile(getchar(), 1));
		savere(&scanre);
		if (execute(0, dot) == 0)
			error(catgets(catd, 1, 207,
				"Fail|Pattern not found on addressed line"));
		ic = loc1;
		if (ic > linebuf && *ic == 0)
			ic--;
	} else {
		getDOT();
		ic = vskipwh(linebuf);
	}
	newline();

	/*
	 * If overstrike then have to HARDOPEN
	 * else if can move cursor up off current line can use CRTOPEN (~~vi1)
	 * otherwise (ugh) have to use ONEOPEN (like adm3)
	 */
	if (OS && !EO)
		bastate = HARDOPEN;
	else if (CA || UP)
		bastate = CRTOPEN;
	else
		bastate = ONEOPEN;
	setwind();

	/*
	 * To avoid bombing on glass-crt's when the line is too long
	 * pretend that such terminals are 160 columns wide.
	 * If a line is too wide for display, we will dynamically
	 * switch to hardcopy open mode.
	 */
	if (state != CRTOPEN)
		WCOLS = TUBECOLS;
	if (!inglobal)
		savevis();
	vok(atube);
	if (state != CRTOPEN)
		TCOLUMNS = WCOLS;
	Outchar = vputchar;
	f = ostart();
	if (state == CRTOPEN) {
		if (outcol == UKCOL)
			outcol = 0;
		vmoveitup(1, 1);
	} else
		outline = destline = WBOT;
	vshow(dot, NOLINE);
	vnline(ic);
	vmain();
	if (state != CRTOPEN)
		vclean();
	Command = "open";
	ovend(f);
#ifdef	SIGWINCH
	signal(SIGWINCH, SIG_DFL);
#endif
}

void 
ovbeg(void)
{

	if (!value(OPEN))
		error(catgets(catd, 1, 208,
			"Can't use open/visual unless open option is set"));
	if (inopen)
		error(catgets(catd, 1, 209,
					"Recursive open/visual not allowed"));
	Vlines = lineDOL();
	fixzero();
	setdot();
	pastwh();
	dot = addr2;
}

void 
ovend(struct termios f)
{

	splitw++;
	vgoto(WECHO, 0);
	vclreol();
	vgoto(WECHO, 0);
	holdcm = 0;
	splitw = 0;
	ostop(f);
	setoutt();
	undvis();
	TCOLUMNS = OCOLUMNS;
	inopen = 0;
	flusho();
	netchHAD(Vlines);
}

/*
 * Enter visual mode
 */
void 
vop(void)
{
	register int c;
#ifndef u370
	cell atube[TUBESIZE + LBSIZE];
#endif
	struct termios f;	/* mjm: was register */
	int resize;

	if (!CA && UP == NOSTR) {
		if (initev) {
toopen:
			merror(catgets(catd, 1, 210, "[Using open mode]"));
			putNFL();
			oop();
			return;
		}
		error(catgets(catd, 1, 211,
		"Visual needs addressible cursor or upline capability"));
	}
	if (OS && !EO) {
		if (initev)
			goto toopen;
		error(catgets(catd, 1, 212,
			"Can't use visual on a terminal which overstrikes"));
	}
	if (!CL) {
		if (initev)
			goto toopen;
		error(catgets(catd, 1, 213,
			"Visual requires clear screen capability"));
	}
	if (NS && !SF) {
		if (initev)
			goto toopen;
		error(catgets(catd, 1, 214, "Visual requires scrolling"));
	}
	resize = SETJMP(venv);
	if (resize) {
		setsize();
		initev = (char *)0;
		inopen = 0;
		addr1 = addr2 = dot;
	}
#ifdef	SIGWINCH
	signal(SIGWINCH, onwinch);
#endif
	ovbeg();
	bastate = VISUAL;
	c = 0;
	if (any(peekchar(), "+-^."))
		c = getchar();
	pastwh();
	vsetsiz(isdigit(peekchar()) ? getnum() : value(WINDOW));
	setwind();
	newline();
	vok(atube);
	if (!inglobal)
		savevis();
	Outchar = vputchar;
	vmoving = 0;
	f = ostart();
	if (initev == 0) {
		vcontext(dot, c);
		vnline(NOSTR);
	}
	vmain();
	Command = "visual";
	ovend(f);
#ifdef	SIGWINCH
	signal(SIGWINCH, SIG_DFL);
#endif
}

/*
 * Hack to allow entry to visual with
 * empty buffer since routines internally
 * demand at least one line.
 */
void 
fixzero(void)
{

	if (dol == zero) {
		register bool ochng = chng;

		vdoappend("");
		if (!ochng)
			synced();
		fixedzero++;
		addr1 = addr2 = one;
	} else if (addr2 == zero)
		addr2 = one;
}

/*
 * Save lines before visual between unddol and truedol.
 * Accomplish this by throwing away current [unddol,truedol]
 * and then saving all the lines in the buffer and moving
 * unddol back to dol.  Don't do this if in a global.
 *
 * If you do
 *	g/xxx/vi.
 * and then do a
 *	:e xxxx
 * at some point, and then quit from the visual and undo
 * you get the old file back.  Somewhat weird.
 */
void 
savevis(void)
{

	if (inglobal)
		return;
	truedol = unddol;
	saveall();
	unddol = dol;
	undkind = UNDNONE;
}

/*
 * Restore a sensible state after a visual/open, moving the saved
 * stuff back to [unddol,dol], and killing the partial line kill indicators.
 */
void 
undvis(void)
{

	if (ruptible)
		signal(SIGINT, onintr);
	squish();
	pkill[0] = pkill[1] = 0;
	unddol = truedol;
	unddel = zero;
	undap1 = one;
	undap2 = dol + 1;
	undkind = UNDALL;
	if (undadot <= zero || undadot > dol)
		undadot = zero+1;
}

/*
 * Set the window parameters based on the base state bastate
 * and the available buffer space.
 */
void 
setwind(void)
{

	WCOLS = TCOLUMNS;
	switch (bastate) {

	case ONEOPEN:
		if (AM)
			WCOLS--;
		/* fall into ... */

	case HARDOPEN:
		basWTOP = WTOP = WBOT = WECHO = 0;
		ZERO = 0;
		holdcm++;
		break;

	case CRTOPEN:
		basWTOP = TLINES - 2;
		/* fall into */

	case VISUAL:
		ZERO = TLINES - TUBESIZE / WCOLS;
		if (ZERO < 0)
			ZERO = 0;
		if (ZERO > basWTOP)
			error(catgets(catd, 1, 215,
				"Screen too large for internal buffer"));
		WTOP = basWTOP; WBOT = TLINES - 2; WECHO = TLINES - 1;
		break;
	}
	state = bastate;
	basWLINES = WLINES = WBOT - WTOP + 1;
}

/*
 * Can we hack an open/visual on this terminal?
 * If so, then divide the screen buffer up into lines,
 * and initialize a bunch of state variables before we start.
 */
void 
vok(register cell *atube)
{
	register int i;

	if (WCOLS == 1000)
		serror(catgets(catd, 1, 216,
		"Don't know enough about your terminal to use %s"), Command);
	if (WCOLS > TUBECOLS)
		error(catgets(catd, 1, 217, "Terminal too wide"));
	if (WLINES >= TUBELINES || WCOLS * (WECHO - ZERO + 1) > TUBESIZE)
		error(catgets(catd, 1, 218, "Screen too large"));

	vtube0 = atube;
	vclrcell(atube, WCOLS * (WECHO - ZERO + 1));
	for (i = 0; i < ZERO; i++)
		vtube[i] = (cell *) 0;
	for (; i <= WECHO; i++)
		vtube[i] = atube, atube += WCOLS;
	for (; i < TUBELINES; i++)
		vtube[i] = (cell *) 0;
	vutmp = (char *)atube;
	vundkind = VNONE;
	vUNDdot = 0;
	OCOLUMNS = TCOLUMNS;
	inopen = 1;
	signal(SIGINT, vintr);
	vmoving = 0;
	splitw = 0;
	doomed = 0;
	holdupd = 0;
	Peekkey = 0;
	vcnt = vcline = 0;
	if (vSCROLL == 0)
		vSCROLL = value(SCROLL);
		/*old vSCROLL = (value(WINDOW)+1)/2;*//* round up so dft=6,11 */
}

void 
vintr(int signum)
{
	extern JMP_BUF readbuf;
	extern int doingread;

	signal(SIGINT, vintr);
	if (vcatch)
		onintr(SIGINT);
	ungetkey(ATTN);
	draino();
	if (doingread) {
		doingread = 0;
		LONGJMP(readbuf, 1);
	}
}

/*
 * Set the size of the screen to size lines, to take effect the
 * next time the screen is redrawn.
 */
void
vsetsiz(int size)
{
	register int b;

	if (bastate != VISUAL)
		return;
	b = TLINES - 1 - size;
	if (b >= TLINES - 1)
		b = TLINES - 2;
	if (b < 0)
		b = 0;
	basWTOP = b;
	basWLINES = WBOT - b + 1;
}

#ifdef	SIGWINCH
void 
onwinch(int signum)
{
	vsave();
	setty(normf);
	LONGJMP(venv, 1);
}
#endif
