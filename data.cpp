/*
 * data.cpp
 * Data file contents and implementation of helper functions.
 *
 * Copyright 2005-2006 by Dale McCoy.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef _WIN32
#include"win32.h"
#endif

#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<string>
#include<sys/stat.h>
#include<cstdarg>
#include<errno.h>
#ifndef _WIN32
#   include<unistd.h>
#endif

using namespace std;

#include"renum.h"
#include"globals.h"
#include"data.h"
#include"messages.h"
#include"win32.h"

//Let's dump the data files into the source code. That sounds like fun, right?

//enum ActBit{ACT0=1,ACT1=2,ACT2=4,ACT3=4,ACT4=8,EMPTY1=0x10,OVERRIDE3=0x20};
//                                          00              04              08              0C
static const char _datfeat[]="\x10\x00\x0D\x3F\x1F\x1F\x3F\x1F\x06\x09\x0F\x01\x0F\x0D\x0F\x01\x00";

//third chawmp: 0x:default 1x:quote 2x:decimal 3x:B-E hex (2x and 3x are currently unsupported)
//The upper chawmp, which only applies in FE strings:
//0x:default  Cx:linebreak;long lead
//Full description in act0.cpp.
static const char _dat0[]="\x0B\x02"
"\x22\xFF\x01\x21\x21\x01\x01\x21\x01\x22\xFF\x22\xFF\x01\x04\xFF\xFF\xFF\x01\x01\x21\x01\x01\x01\x01\x01\x01\x22\x01\x34\x01\x01\x01\x01\x01\x21\x01\x01\x21\x01\x32\x32\x00"
"\x22\xFF\x01\x21\x21\xFF\x01\x21\x21\x01\x04\xFF\xFF\xFF\x01\x21\x01\x01\x01\x21\x21\x21\x34\x01\x01\x01\x01\x21\x01\x32\x32\x00"
//                                 08                              10                              18                              20                              28
"\x22\xFF\x01\x21\x21\xFF\x01\x21\x01\x01\x01\x21\x01\x22\xFF\x01\x01\x34\x01\x01\x01\x01\x21\x01\x32\x32\x00"
"\x22\xFF\x01\x21\x21\xFF\x01\x21\x01\x01\x01\x01\x21\x01\x01\x22\xFF\x21\x01\x34\x01\x01\x21\x01\x32\x32\x00"
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x14\xFE\x01\x01\x01\x01\xFE\x01\x22\x01\x34\x01\x01\x01\x32\x21\x01\x00"
  "\x03r\xFE\x80\x00"//                ^^
	"l\\\x00l\\\x00l\\\x00l\\\x00|\x34*\xFE\x01\x80\x00"
	  "\x01\x01\x01\x01\x01\x01\x34\x00"
  "*\xFE\x02\\\x00\\\x00\x00" //                           ^^
	"\x01\x01r\x01x\x80\x81\xC0\x00"
"\x00"
//                                 08                              10
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x21\x21\x21\x01\x22\xFE\x01\x00"
  "\x01\x01r\x34x\x81\x20\xC0\x00"//                   ^^
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01\x01\x02\x01\x01\x01\x01\x01\x22\x01\x32\x02\x01\x01\x01\x04\x01\x01\x01\x01\x01\x01\x04\x00"
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01\x14\x32\x24\x12\x14\x14\x22\xFE\x00"
  "r\xFE\x0B\xFE\x00"//                                           ^^
	"r\x01\x20\xC0\x00"
	"r\x01\x20\x00"
//                                 08                              10                              18                              20
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01\x01\x02\x02\x02\x01\x01\x02\x01\x01\x01\x00"
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01\x01\xFE\x01\x02\x02\x02\x01\x02\x04\x01\x01\x01\xFE\xFE\x01\x01\x01\x04\x02\x04\x04\x04\x02\x04\x01\x01\x00"
  "\x01\xF4r\xFE\x80\x00"//                ^^
	"l\xFE\x01\xC1|*\xFE\x02\\\x00\x80\x00"
	  "\x00"
	  "\x01\x01\xFE\xC0\x00"
		"l\xFE\x02|\x01\x00"
		  "\x00"
  "\x01r\x01\x80\x00"//                                                                ^^
  "r\x01\x03\x00"//                                                                        ^^
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x21\x32\x32\x32\x32\x32\x32\x01\x01\x01\x34\x01\x01\x01\x32\x14\x01\x32\x01\x00"
//                                 08                              10                              18
"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01\x01\x01\x00"
"\x00";

static const char _dat2v[]="\x09\x06"
	"\x00\x82\x01\x81\x02\x81\x03\x81\x09\x82\x0A\x82\x0C\x82\x10\x84\x18\x84\x1A\x84\x1B\x81\x1C\x84\xFF\xF0"
"\x00\x40\x83\x41\x83\x42\x84\x43\x84\x45\x83\x46\x84\x47\x84\x48\x81\x5F\x81\x60\x81\x73\x7E\x84\xFF\x7F\x84\x7F\xFF\xF0"
"\x01\x42\x81\x43\x84\x47\x84\x48\x81\x5F\x81\x7E\x84\xFF\x7F\x84\x7F\xFF\xF0"
"\x02\x42\x81\x43\x84\x47\x84\x48\x81\x5F\x81\x7E\x84\xFF\x7F\x84\x7F\xFF\xF0"
"\x03\x40\x83\x42\x81\x43\x84\x48\x81\x44\x82\x47\x84\x5F\x81\x7E\x84\xFF\x7F\x84\x7F\xFF\xF0"
"\x08\x11\x81\x40\x84\x41\x84\x42\x82\x43\x83\x44\x81\x45\x82\x46\x84\x47\x84\x48\x85\x49\x84\x4A\x81\x5F\x83\x60\x82\x1F\x61\x81\x1F\x62\x84\x1F\x63\x81\x1F\x64\x82\x1F\x65\x81\x1F\x66\x84\xFF\x7E\x84\xFF\x7F\x84\x7F\xFF\xF0"
"\x05\x7E\x84\xFF\x7F\x84\x7F\x82\x80"
"\x06\x80\x80"
"\x08\x40\x81\x41\x81\x42\x81\x43\x81\x44\x84\x45\x81\x46\x81\x5F\x81\x60\x82\x6B\x61\x82\xFF\x62\x84\xFF\x7E\x84\xFF\x7F\x84\x7F\x80\x80"
"\x08\x40\x81\x41\x81\x7E\x84\xFF\x7F\x84\x7F\xDE\x80"
"\x0A\x40\x81\x41\x81\x42\x81\x43\x83\x44\x81\x5F\x81\x60\x84\xFF\x61\x84\xFF\x7E\x84\xFF\x7F\x84\x7F\x80\x80"
"\x08\x40\x82\x41\x82\x42\x82\x60\x82\xFF\x61\x81\xFF\x62\x84\xFF\x63\x84\xFF\x63\x84\xFF\x7E\x84\xFF\x7F\x84\x7F\xB6\x80"
"\x0B\x7E\x84\xFF\x7F\x84\x7F\x80\x80"
"\x0C\x80\x80"
"\x0D\x80\x80";
//Format: Low nibble: width (1/2/4) high nibble: bitmask: Read in 7, read in D, write in D, reserved
//                                          80                              88                              90                              98                              A0
static const char _dat79Dv[]="\x1F\x00\x20\x00\x00\x00\xC1\xC1\x80\x80\x00\x84\x00\x00\xC4\x00\xC1\xE1\xE4\x00\x00\xC1\xE2\xE2\xE2\xE2\xE1\x00\x24\x00\x00\x00\xC4\xE4\x24";
static const char _datD[]="\x13\x00"
	"\x10\x0C"                      "\x74\x00\x58\x00\x0B\x00\x29\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1E\x13\x00\x00\x25\x00\x40\x00\x00\x00\x00\x00";
//Feature:                             --00--                          --04--                          --08--                          --0C--
static const char _datIDs[]="\x76\x00\x73\x00\x57\x00\x0A\x00\x28\x00\xFF\x00\x04\x00\x0A\x00\xFF\x00\x00\x00\xFF\x00\x24\x00\x1F\x00\xFF\xFF\x00\x00";
static const char _dat4[]="\x02\x00"
"\x04\x04\x04\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x1C\x1C\x1C\x1C\x04\x04\x00\x04\x00\x1C\x1D\x06\x00\x00";
static const char _datB[]="\x01\x00\x03\x04\x02\x02\x02\x03";
//                                         05      06              08                              0C
static const char _dat5[]="\x02\x01\x0D\x30\x00\x4A\x00\x49\x00\x41\x00\x06\x00\x00\x01\x4C\x00\x85\x00\x10\x00";
//Count of IDs in class:                   -0000-                          -2000-                          -4000-                          -6000-                          -8000-                          -A000-                          -C000-                          -E000-
static const char _datTextIDs[]="\x04\x02\x35\x03\x11\x00\x25\x00\x19\x00\x5D\x00\x11\x00\x6D\x00\x08\x00\x11\x00\x3C\x00\x2A\x00\x08\x00\x19\x00\x39\x00\x80\x00\x00\x00\x08\x01\x6C\x00\x38\x00\x43\x00\x44\x00\x00\x00\x06\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x4D\x00\x00\x00\x00\x00\x05\x01"
	"\x03\xC4\xC5\xC9";
//                                     --08--                          --0C--                          --10--
static const char _dat0f8[]="\x02\x00\x30\x30\x00\xFC\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x12\x00\x00";

static const char _datcallbacks[]="\x05\x00\x43\x01"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x8F\x04\x04\x8F\x00\x07\x9F\x8F\x07\x07\x07\x00\x07\x07"
//                 x4              x8              xC
"\x07\x07\x0A\x8F\x04\x09\x09\x09\x0A\x0A\x07\x09\x09\x8F\x07\x09"
"\x09\x8F\x8F\x8F\x8F\x0A\x8F\x0A\x0A\x0B\x0A\x0A\x09\x0A\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
//                 x4              x8              xC
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
//                 x4              x8              xC
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
//                 x4              x8              xC
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
//                 x4              x8              xC
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x05\x05\x05";

//byte triples: num 80 bits/num 83 bits/num triggers
//                                    --  00  --                                      --  04  --                                      --  08  --                                      --  0C  --
static const char _dat2r[]="\x01\x00\x08\x08\x05\x08\x08\x05\x08\x08\x05\x08\x08\x05\x20\x00\x06\x00\x00\x00\x00\x00\x00\x08\x00\x02\x00\x00\x00\x08\x10\x03\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

static const char _datlangs[]="\x00\x01"
//		x0				x1				x2				x3				x4				x5				x6				x7				x8				x9				xA				xB				xC				xD				xE				xF
/*0x*/	"American\n"	"English\n"		"German\n"		"French\n"		"Spanish\n"		"\n"			"\n"			"Russian\n"		"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"
/*1x*/	"\n"			"\n"			"\n"			"\n"			"\n"			"Czech\n"		"Slovak\n"		"\n"			"\n"			"\n"			"\n"			"Afrikaans\n"	"\n"			"\n"			"Greek\n"		"Dutch\n"
/*2x*/	"\n"			"\n"			"Catalan\n"		"\n"			"Hungarian\n"	"\n"			"\n"			"Italian\n"		"Romanian\n"	"Icelandic\n"	"Latvian\n"		"Lithuanian\n"	"Slovenian\n"	"Danish\n"		"Swedish\n"		"Norwegian\n"
/*3x*/	"Polish\n"		"Galician\n"	"Frisian\n"		"Ukrainian\n"	"Estonian\n"	"Finnish\n"		"Portuguese\n"	"Brazilian Portuguese\nCroatian\n\n"			"\n"			"\n"			"\n"			"\n"			"Turkish\n"		"\n"
/*4x*/	"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"
/*5x*/	"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"
/*6x*/	"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"
/*7x*/	"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"\n"			"any\n";


/*** Add data file data here ***/

/* The first byte of each _dat* is a format byte; start it at \x00. Each
 * time the file format gets updated (ie you change the file reader),
 * increment it and reset the version byte to \x00. The second byte is a
 * version byte; increment it every time the file gets updated.
 * These bytes are never seen except in myfopen()*/

struct dat{
	char*data,*name;
	uint len;
};

#undef _RENUM_DATA_H_INCLUDED_
#undef DATA
#undef DATA_FILE
#define DATA() static const dat data[]={
#define DATA_FILE(name)\
	{(char*)_dat##name,"/.renum/" #name ".dat",sizeof(_dat##name)-1},\

#include "data.h"

bool makedir(const string&dir,bool dieonfail=false){
	if(dir==""){
		if(dieonfail)exit(EDATA);
		return false;
	}
	if(mkdir((dir+"/.renum").c_str(),0755)){
		IssueMessage(0,CREATE_FAILED,dir.c_str(),errno);
		perror(NULL);
		if(dosleep)sleep(5);
		if(dieonfail)exit(EDATA);
		return false;
	}else{
		IssueMessage(0,CREATED_DIR,dir.c_str());
		if(dosleep)sleep(5);
		return true;
	}
}

bool finddir(string&dir){
	if(dir=="")return false;
	struct stat Stat;
	if(dir[dir.length()-1]=='\\'||dir[dir.length()-1]=='/')
		dir[dir.length()-1]='\0';
	if(stat((dir+"/.renum").c_str(),&Stat))return false;
	else if(Stat.st_mode&S_IFREG)return false;
	return true;
}

string safetostring(const char*ptr){return ptr?ptr:"";}

string getdir(){
	if(datadir!=""){
		if(finddir(datadir)||makedir(datadir,true))return datadir;
		INTERNAL_ERROR(makedir(datadir,true),false);
	}
	char*pcwd=getcwd(NULL,0);
	const string cwd=pcwd;
	string ret;
	free(pcwd);
	if(finddir(ret=cwd)||finddir(ret=safetostring(getenv("HOME")))||makedir(ret)||makedir(ret=cwd,true))return ret;
	INTERNAL_ERROR(makedir(cwd,true),false);
}

FILE*tryopen(const char*name,const char*mode,bool allownull=false){
	static string dir=getdir();
	FILE*pFile=fopen((dir+name).c_str(),mode);
	if(pFile||allownull)return pFile;
	IssueMessage(0,DATAFILE_ERROR,OPEN,name+1,"(%d)",errno);
	perror(NULL);
	assert(false);
	exit(EDATA);
}

FILE*_myfopen(files file){
	FILE*pFile=tryopen(data[file].name,"rb",true);
	if(pFile){
		if(fgetc(pFile)==data[file].data[0]&&fgetc(pFile)>=data[file].data[1])return pFile;
		fclose(pFile);
	}
	pFile=tryopen(data[file].name,"wb");
	if(fwrite(data[file].data,1,data[file].len,pFile)!=data[file].len){
		IssueMessage(0,DATAFILE_ERROR,WRITE,data[file].name+1,"");
		assert(false);
		exit(EDATA);
	}
	fclose(pFile);
	pFile=tryopen(data[file].name,"rb");
	fgetc(pFile);
	fgetc(pFile);
	return pFile;
}

int _CheckEOF(int dat,files file,const char*src,int line){
	if(dat==EOF){
		IssueMessage(0,DATAFILE_ERROR,LOAD,data[file].name+8,"(%s:%d)",src,line);
		assert(false);
		exit(EDATA);
	}
	return dat;
}

int _GetCheckWord(FILE*pFile,files file,const char*src,int line){
	int ret=fgetc(pFile);
	return ret|(_CheckEOF(fgetc(pFile),file,src,line)<<8);
}

void _myfread(FILE*pFile,uchar*target,uint count,files file,const char*src,int line){
	if(fread(target,1,count,pFile)!=count){
		IssueMessage(0,DATAFILE_ERROR,LOAD,data[file].name+8,"(%s:%d)",src,line);
		assert(false);
		exit(EDATA);
	}
}