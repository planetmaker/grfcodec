
#include "pcxsprit.h"
#include "ttdpal.h"
#include "grfcomm.h"

extern bool _mapAll,_hexspritenums;

/***********************\
*						*
* class pcxwrite		*
*						*
\***********************/

// to make this a bit more readable, use binary digits instead of hex
#define b____ 0
#define b___O 1
#define b__O_ 2
#define b__OO 3
#define b_O__ 4
#define b_O_O 5
#define b_OO_ 6
#define b_OOO 7
#define bO___ 8
#define bO__O 9
#define bO_O_ 10
#define bO_OO 11
#define bOO__ 12
#define bOO_O 13
#define bOOO_ 14
#define bOOOO 15


#define _(o,t) ((b ## t << 4) + b ## o)

#define DIGITHEIGHT 5
#define DIGITWIDTH 4
// DIGITHEIGHT*DIGITWIDTH*16(digits)/8(bitsperbyte) = 40 bytes
#define DIGITIND(digit,line) (digit/2+8*line)
#define DIGITSHR(digit) (4*(digit&1))
static unsigned char digitlines[40] = {

_( _OO_, __O_ ),_( OOO_, OOO_ ),_( __O_, OOOO),_( _OO_, OOOO),_( _OO_, _OO_ ), _( _OO_, OOO_ ), _( _OOO, OOO_ ), _( OOOO, OOOO ),
_( O__O, _OO_ ),_( ___O, ___O ),_( _OO_, O___),_( O___, ___O),_( O__O, O__O ), _( O__O, O__O ), _( O___, O__O ), _( O___, O___ ),
_( O__O, __O_ ),_( _OO_, _OO_ ),_( O_O_, OOO_),_( OOOO, __O_),_( _OO_, _OOO ), _( OOOO, OOO_ ), _( O___, O__O ), _( OOO_, OOO_ ),
_( O__O, __O_ ),_( O___, ___O ),_( OOOO, ___O),_( O__O, _O__),_( O__O, ___O ), _( O__O, O__O ), _( O___, O__O ), _( O___, O___ ),
_( _OO_, _OOO ),_( OOOO, OOO_ ),_( __O_, OOO_),_( _OO_, _O__),_( _OO_, _OO_ ), _( O__O, OOO_ ), _( _OOO, OOO_ ), _( OOOO, O___ ),

};
#undef _


pcxwrite::pcxwrite(multifile *mfile)
{
	setfile(mfile);
	spriteno=-1;	// so the first one will be 0
	lastdigitx = -50;
}

void pcxwrite::filedone(int final)
{
	int y;

	if (curfile) {
		if (final) {
			// fill with blank lines to the right height if necessary
			for (y=totaly; y<sy; y++) {
				startencoding();
				encodebytes(background, sx);
				endencoding();
			}

			if (sy != -1)
				totaly = sy;

		}

		writepal();
		writeheader();
		fflush(curfile);
	}
}

void pcxwrite::setcolours(U8 bg, U8 bord, int skip)
{
	background = bg;
	border = bord;
	borderskip = skip;
}

void pcxwrite::startsubimage(int /*x*/, int /*y*/, int sx, int sy)
{
#define BORDERSIZE 4
	int bordersizex = BORDERSIZE;
	int bordersizey = BORDERSIZE + DIGITHEIGHT + 1;

	sx += bordersizex;
	sy += bordersizey;

	subx += ( (int) (px + bandx) / bandx) * bandx;

	if (subx + sx >= pcxfile::sx) {
		newband();
		lastdigitx = -50;
	}

	if (sy > thisbandy) {
		thisbandy = ( (int) (sy + bandy - 1) / bandy) * bandy;
		alloclines(thisbandy);
	}

	cx = 0;
	cy = 0;

	px = sx;// - bordersizex;
	py = sy;// - bordersizey;

	dx = 1;
	dy = 1;

	if (borderskip) {
		int i, bx, by;
		bx = sx - 2;
		by = sy - 2;
		for (i=0; i < 2*bx+2*by; i+=borderskip) {
			border = i & 8;

			if (i < bx)
				putpixel(i, 0, border);

			else if (i < bx + by)
				putpixel(0, i - bx, border);

			else if (i < 2*bx + by)
				putpixel(i-bx-by, by, border);

			else
				putpixel(bx, i-2*bx-by, border);
		}
	}

	//  WRONG Y IN NFO FILE !!

	// show sprite number
	showspriteno();

	dx++;
	dy+=1+DIGITHEIGHT+1;
}

void pcxwrite::showspriteno()
{
	char spritenum[10];
	int newlastx;
	if(_hexspritenums)
		sprintf(spritenum, "%X", spriteno);
	else
		sprintf(spritenum, "%d", spriteno);

	newlastx = subx+strlen(spritenum)*(DIGITWIDTH+1)+dx;
	if (newlastx >= pcxfile::sx)
		return;

	if (subx+dx < lastdigitx + 2*(DIGITWIDTH+1))
		return;

	lastdigitx = newlastx;

	for (int i=0; spritenum[i]; i++) {
		int digit = spritenum[i] - '0';
		if (digit > 9)
			digit += '0' - 'A';
		for (int y=0; y<DIGITHEIGHT; y++) {
			int pixels = digitlines[DIGITIND(digit,y)] >> DIGITSHR(digit);
			for (int x=DIGITWIDTH-1; x>=0; x--) {
				if (pixels & 1)
					putpixel(x + i*(DIGITWIDTH+1),y,border);
				pixels >>= 1;
			}
		}
	}
}

void pcxwrite::setline(U8 *band)
{
	memset(band, background, sx);
}

void pcxwrite::spritedone(int sx, int sy){
	spritedone();

	bool maybeGlyph=!_mapAll;

	for(int cx=0, x=subofsx(cx,0);cx<sx&&maybeGlyph;cx++,x++)
		for(int cy=0, y=subofsy(cy,0);cy<sy;cy++,y++)
			maybeGlyph &= (band[y][x] < 3);

	if (!maybeGlyph)
		for(int cx=0, x=subofsx(cx,0);cx<sx;cx++,x++)
			for(int cy=0, y=subofsy(cy,0);cy<sy;cy++,y++)
				band[y][x] = putcolourmap[band[y][x]];
}

void pcxwrite::writeheader()
{
	long oldpos = ftell(curfile);
	fseek(curfile, 0, SEEK_SET);

	header.window[3] = totaly - 1;
	header.screen[1] = totaly - 1;

	#ifdef __BIG_ENDIAN__
		pcxheader le_header = header;
		be_swapheader(le_header);
	#else
		pcxheader& le_header = header;
	#endif
	fwrite(&le_header, sizeof(pcxheader), 1, curfile);
	fseek(curfile, oldpos, SEEK_SET);
}

void pcxwrite::writepal()
{
	long oldpos = ftell(curfile);

	fseek(curfile, 0, SEEK_END);

	fputc(12, curfile);
	fwrite(palette, 768, 1, curfile);

	fseek(curfile, oldpos, SEEK_SET);
}

void pcxwrite::setpalette(const U8 *palette)
{
	pcxwrite::palette = palette;
}

/*void pcxwrite::setpalette(FILE *palfile)
{
	cfread(palette, 1, 768, palfile);
}*/


/***********************\
*						*
* class pcxread			*
*						*
\***********************/

pcxread::pcxread(singlefile *mfile)
{
	setfile(mfile);
};

void pcxread::startsubimage(int x, int y, int sx, int sy)
{
	subx = x;

	if (y + sy > pcxread::sy) {
		printf("\nError: Sprite y extends beyond end of PCX file.\nFile has %d lines, sprite wants %d..%d\n.",
			pcxread::sy, y, y + sy - 1);
		exit(2);
	}

	if (sy > bandlines) {	// don't have enough lines in memory, read rest
		alloclines(sy);
	}

	if (y > totaly) {	// doesn't start right at the top -> delete extra lines
		expirelines(y - totaly);
		totaly = y;
	}

	cx = 0;
	cy = 0;

	px = sx;
	py = sy;

	dx = 0;
	dy = 0;
}

void pcxread::setline(U8 *band)
{
	startdecoding();

	decodebytes(band, sx);
}

extern bool _force;
extern int _quiet;

void pcxread::readheader()
{
	long oldpos = ftell(curfile);
	fseek(curfile, 0, SEEK_SET);

	cfread("reading pcx header", &header, sizeof(pcxheader), 1, curfile);
	be_swapheader(header);

	if (header.nplanes == 3) {
		fprintf(stderr, "Cannot read truecolour PCX files!\n");
		exit(2);
	}
	if ( (header.bpp != 8) || (header.nplanes != 1) ) {
		fprintf(stderr, "PCX file is not a 256 colour file!\n");
		exit(2);
	}

	fseek(curfile,-768,SEEK_END);
	U8 palette[768];

	if (fread(palette,1,768,curfile) != 768 ) {
		fprintf(stderr, "Could not read palette from PCX file!\n");
		exit(2);
	}

	int i=0;
	for(;i<NUM_PALS;i++)
		if(!memcmp(palette,defaultpalettes[i],768)) break;

	if ( i == NUM_PALS ) {
		if ( _force ) {
			if (!_quiet) fprintf(stderr, "Warning: Encoding despite unrecognized palette.\n");
		} else {
			fprintf(stderr, "Error: Unrecognized palette, aborting.\n"
				"Specify -f on the command line to override this check.\n");
			exit(2);
		}
	}

	fseek(curfile, oldpos, SEEK_SET);

	sx = header.window[2] + 1;
	sy = header.window[3] + 1;
	totaly = 0;

	thisbandy = 0;
}

