#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>              

#include "ovdisis_types.h"

#define BIN8(c) ((c)>>7)&1,((c)>>6)&1,((c)>>5)&1,((c)>>4)&1, \
		((c)>>3)&1,((c)>>2)&1,((c)>>1)&1,((c)>>0)&1

int main(int argc, char *argv[])
{
	int fnt;
	int val_i;
	int fontdataoffset;
	int i,h,off,shift;
	unsigned int ch,mask;
	unsigned char *fontdata;

	FontInfo font;
	
	if(argc < 2) {
		fprintf(stderr,"Usage: %s <fnt-file>\n",argv[0]);
		exit(1);
	}
	
	fnt = open(argv[1], O_RDONLY);
	if(fnt == -1) {
		perror(argv[1]);
		exit(1);
	}

	
	read(fnt, &font.id, 2);
	read(fnt, &font.size, 2);
	read(fnt, &font.name, 32);
	read(fnt, &font.first, 2);
	read(fnt, &font.last, 2);
	read(fnt, &font.top, 2);
	read(fnt, &font.ascent, 2);
	read(fnt, &font.half, 2);
	read(fnt, &font.descent, 2);
	read(fnt, &font.bottom, 2);
	read(fnt, &font.wchar, 2);
	read(fnt, &font.wcell, 2);
	read(fnt, &font.left, 2);
	read(fnt, &font.right, 2);
	read(fnt, &font.thick, 2);
	read(fnt, &font.underline, 2);
	read(fnt, &font.light, 2);
	read(fnt, &font.skew, 2);
	read(fnt, &font.flags, 2);
	read(fnt, &val_i, 4);
	font.horizontal_offset = NULL;
	read(fnt, &val_i, 4);
	font.character_offset = NULL;
	read(fnt, &val_i, 4);
	fontdataoffset = val_i;
	read(fnt, &font.formwidth, 2);
	read(fnt, &font.formheight, 2);
	font.next = NULL;
		

	printf("/* Converted by Tomas */\n\n"
	       "#include \"../ovdisis.h\"\n\n");
	printf("#define FONTDATAMAX (%d*256)\n\n",font.formheight);

	printf("int  fontheight_system_%dx%d = %d;\n",font.wcell,font.formheight,font.formheight);
	printf("int  fontwidth_system_%dx%d = %d;\n\n",font.wcell,font.formheight,font.wcell);
	printf("unsigned char fontdata_system_%dx%d[FONTDATAMAX] = {\n",
	       font.wcell,font.formheight);

	lseek(fnt, fontdataoffset, SEEK_SET);
	if((fontdata = (char *)malloc(font.formwidth*font.formheight)) == NULL) {
		fprintf(stderr,"no mem left\n");
		exit(1);
	}
	read(fnt, fontdata, font.formwidth*font.formheight);
	close(fnt);
		
	for(i=0,mask=0 ; i<font.wcell ; i++) mask |= 1 << (7-i);

	off = 0; shift = 0;
	for(i=0 ; i<256 ; i++) {
		if(i < 32)
			printf("\n\t/* %d 0x%02x '^%c' */\n",i,i,(unsigned char)i+65);
		else
			printf("\n\t/* %d 0x%02x '%c' */\n",i,i,(unsigned char)i);
		
		for(h=0 ; h<font.formheight ; h++) {
			ch = ((unsigned int)fontdata[h*font.formwidth+off] & 0xff) << shift;
			if(shift > 0)
				ch |= ((unsigned int)fontdata[h*font.formwidth+off+1] & 0xff) 
				  >> (8-shift);
			ch &= mask;
							
			printf("\t0x%02x, /* %d%d%d%d%d%d%d%d */\n",ch,BIN8(ch));
		}
		shift += font.wcell;
		if(shift >= 8) {
			off++;
			shift -= 8;
		}
	}
		
	printf("};\n\n");


	printf("/* FontInfo struct for oVDIsis */\n");
	printf("FontInfo system_%dx%d = {\n",font.wcell,font.formheight);
	printf("\t%hd,\t\t/* Face ID */\n",font.id);
	printf("\t%hd,\t\t/* Face size */\n",font.size);
	printf("\t\"%s\",\t/* Face name */\n",font.name);
	printf("\t%hd,\t\t/* First character index */\n",font.first);
	printf("\t%hd,\t\t/* Last character index */\n",font.last);
	printf("\t%hd,\t\t/* Top line distance */\n",font.top);
	printf("\t%hd,\t\t/* Ascent line distance */\n",font.ascent);
	printf("\t%hd,\t\t/* Half line distance */\n",font.half);
	printf("\t%hd,\t\t/* Descent line distance */\n",font.descent);
	printf("\t%hd,\t\t/* Bottom line distance */\n",font.bottom);
	printf("\t%hd,\t\t/* Width of widest character */\n",font.wchar);
	printf("\t%hd,\t\t/* Width of widest character cell (==width of font) */\n",font.wcell);
	printf("\t%hd,\t\t/* Left offset */\n",font.left);
	printf("\t%hd,\t\t/* Right offset */\n",font.right);
	printf("\t%hd,\t\t/* Thickening size */\n",font.thick);
	printf("\t%hd,\t\t/* Underline size */\n",font.underline);
	printf("\t0x%04hx,\t\t/* Lightening mask */\n",font.light);
	printf("\t0x%04hx,\t\t/* Skewing mask */\n",font.skew);
	printf("\t0x%04hx,\t\t/* Font flags */\n",font.flags);
	printf("\tNULL,\t\t/* Character offset table (currently unused) */\n");
	printf("\tNULL,\t\t/* Horizontal offset table (currently unused) */\n");
	printf("\tfontdata_system_%dx%d,\t/* Pointer to the fontdata above */\n",
	       font.wcell, font.formheight);
	printf("\t%hd,\t\t/* Fontdata form width (don't care) */\n",font.formwidth);
	printf("\t%hd,\t\t/* Fontdata form height (==height of font) */\n",font.formheight);
	printf("\tNULL\t\t/* Pointer to next font */\n");
	printf("};\n");



	return 0;
}
