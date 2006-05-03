/*
 * messages.h
 * Contains messages output by NFORenum, and declares related functions
 *
 * Copyright 2004-2006 by Dale McCoy.
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

#ifndef _RENUM_MESSAGES_H_INCLUDED_
#define _RENUM_MESSAGES_H_INCLUDED_

#include<cstdarg>

string vIssueMessage(int,int,va_list);
string IssueMessage(int,int,...);
void AutoConsoleMessages();
void ManualConsoleMessages();
string mysprintf(const char*,...);

#ifndef MESSAGE
#define MESSAGE(name,message,props)name,
#define START_MESSAGES() enum{
#define END_MESSAGES() };
#endif//MESSAGE

#ifndef ERR_MESSAGE
#define ERR_MESSAGE(name,message,props) MESSAGE(name,message,NO_CONSOLE|props|TO_ERROR)
#define OUT_MESSAGE(name,message,props) MESSAGE(name,message,NO_CONSOLE|props|TO_OUT)
#define NFO_MESSAGE(name,message,props) MESSAGE(name,message,MAKE_COMMENT|props|TO_NFO)
#endif

#ifndef MESSAGE_EX
#define MESSAGE_EX(name,msg,props,loc,base) MESSAGE(name,msg,props|TO_##loc)
#endif

#ifndef MESSAGE_UNUSED
#define MESSAGE_UNUSED(name) MESSAGE(unused_##name,NULL,0)
#endif

//#ifndef VMESSAGE // Verbose output messages
//#define VMESSAGE(name,message) NFO_MESSAGE(V_##name,message,NO_CONSOLE|HAS_OFFSET)
//#endif

#ifndef GENERAL_STRINGS
#define GENERAL_STRINGS

#define PREFIX_UNPARSEABLE "Processing Failure: "
#define PREFIX_LINT_FATAL "Fatal Error (%d): "
#define PREFIX_LINT_ERROR "Error (%d): "
#define PREFIX_LINT_WARNING "Warning (%d): "
#define OFFSET "Offset %d: "

#define LOADING "loading"
#define LOADED "loaded"
#define LOTS "lots-of-cargo"
#define HOUSE_INSTYTILE "houses and industry tiles"
#define INDUSTRIES "industries"
#define BASICSTD2 "Basic standard 02s"
#define PROD2 "Production callbacks"
#define GROUND "ground"
#define NONTRANS "non-transparent"

#define ACTION "action %xs"
#define IMPORTS "sound import sprites"
#define TYPE "%s for %s"
#define AT_LEAST "at least %d"
#define EXACTLY "%d"
#define ONE_OF "%d or %d"
#define VARS "%s and %s"
#define VALS "%2x and %2x"
#define REAL_S "real sprite"
#define BIN_INCLUDE "binary include"
#define RECOLOR_TABLE "recolor table"
#define SET "action 1 set"
#define CID "cargo ID"
#define DAT2 "%2x %2x"
#define DAT3 "%2x %2x %2x"

#define UNKNOWN_FAIL "Unknown reason"
#define IS_FILE "Exists as a file"
#define NO_SUCH_DIR "No such directory"

#define EOF_READING_NAME "EOF while reading name %2x."
#define OVERLENGTH_NAME "Name %2x exceeds 100 characters long."

#define OPEN "open"
#define LOAD "load"
#define WRITE "write"

//#defines for properties bit-field
#define MAKE_COMMENT 1 //Indicates that the message should be prefixed with "//!!"
#define USE_PREFIX 2  //Use the appropriate PREFIX_* string, as determined by the minSan argument to IssueMessage
#define HAS_OFFSET 4  //Prefix "Offset %d"
#define NO_CONSOLE 8  //Don't auto-send message to console
//Which output stream to use:
#define TO_ERROR 0
#define TO_OUT 0x10
#define TO_NFO 0x20
#define TO_NULL 0x30
#define TO_MASK 0x30
#define TO_SHIFT 4

#endif//GENERAL_STRINGS

//The first message is different for a reason. It is not run through myvsprintf; a number is appended at runtime.
//All other messages follow the printf format (see myvsprintf in messages.cpp for supported specifiers).

START_MESSAGES()
ERR_MESSAGE(FATAL_MESSAGE_ERROR,"Fatal error issuing message ",0)
ERR_MESSAGE(INTERNAL_ERROR_TEXT,"%s(%d): (in function %s) %s cannot equal %d.\n",0)
ERR_MESSAGE(INVALID_DATAFILE,"Data file \"%s\" contains invalid data. (%S)\n",0)
ERR_MESSAGE(NO_INPUT_FILE,"Could not open \"%s\" specified on the command line.\n",0)
ERR_MESSAGE(NO_OUTPUT_FILE,"Could not open output file \"%s\" for input \"%s\".\n",0)
ERR_MESSAGE(REPLACE_FAILED,"Could not replace old file (%s) with new file (%s). (%d)\n",0)
ERR_MESSAGE(DATAFILE_ERROR,"Failed to %s data file: \"%s\". %S\n",0)
ERR_MESSAGE(CREATE_FAILED,"Could not create .renum directory in %s. (%d)\n\t%s.\n",0)
  MESSAGE_UNUSED(INTERNAL_SWITCH_ERROR)//,"%s(%d): Function %s is missing case for %s==%d.\n",0)

OUT_MESSAGE(CREATED_DIR,"Created .renum directory in %s.\n",0)
OUT_MESSAGE(PROCESSING,"Processing from standard input.\n",0)
OUT_MESSAGE(PROCESSING_FILE,"Processing file \"%s\".\n",0)
OUT_MESSAGE(PROCESSING_COMPLETE,"Processing complete.\n",0)
OUT_MESSAGE(APPARENTLY_NOT_NFO,"Does not appear to be an NFO file.\n",0)
OUT_MESSAGE(UNKNOWN_VERSION,"Unknown NFO file version: %d.  ",0)//No \n. That's intentional.
OUT_MESSAGE(SKIPPING_FILE,"Skipping file.\n",0)
OUT_MESSAGE(PARSING_FILE,"Attempting to parse as version 4.\n",0)
OUT_MESSAGE(PARSE_FAILURE,"The sprite following sprite %d could not be processed.\n",0)
OUT_MESSAGE(PARTIAL_PARSE_FAILURE,"A portion of sprite %d could not be processed.\n",0)
OUT_MESSAGE(NOT_IN_SPRITE,"Found pseudosprite continuation line while looking for sprite %d.\n",0)
OUT_MESSAGE(CONSOLE_LINT_FATAL,"Linter failure on sprite %d.\n",0)
OUT_MESSAGE(CONSOLE_LINT_ERROR,"Error on sprite %d.\n",0)
OUT_MESSAGE(CONSOLE_LINT_WARNING,"Warning on sprite %d (level %d).\n",0)
MESSAGE_EX(UNEXP_EOF_STD2,"Unexpected EOF: Expected additional standard Action 2s.\n",0,OUT,UNUSED_SET)
MESSAGE_EX(UNEXP_EOF_CARGOID,"Unexpected EOF: Unused CargoIDs detected for feature %x.\n",0,OUT,UNUSED_ID)
MESSAGE_EX(UNEXP_EOF_TOWNNAMES,"Unexpected EOF: Unused town name IDs detected.\n",0,OUT,UNUSED_ID)
OUT_MESSAGE(STARTUP,"NFORenum " VERSION " - Copyright " YEARS " Dale McCoy.\n",0)

NFO_MESSAGE(BAD_RPN,"Invalid RPN expression while reading character %c.\n",0)
NFO_MESSAGE(BAD_RPN_EOF,"No close parenthesis found.\n",0)
NFO_MESSAGE(UNDEF_VAR,"Undefined variable \"%s\".\n",0)
NFO_MESSAGE(COMMAND_INVALID_ARG,"Invalid argument to %s command.\n",0)
NFO_MESSAGE(COMMAND_UNKNOWN,"Unknown comment command: %s\n",0)
NFO_MESSAGE(COMMAND_REVERT_DEFAULT,"Assuming \"DEFAULT\".\n",0)

//UNPARSEABLE
NFO_MESSAGE(INVALID_CHARACTER,"Invalid character: \"%c\".\n",0)
NFO_MESSAGE(INVALID_EXTENSION,"Invalid escape sequence.\n",0)
NFO_MESSAGE(UNTERMINATED_STRING,"Unterminated literal string.\n",0)
  MESSAGE_UNUSED(OFFSET_STRING)//,"Literal string found after an odd number of hex characters.\n",0)
  MESSAGE_UNUSED(OFFSET_EXTENSION)//,"Format extention found after an odd number of hex characters.\n",0)
  MESSAGE_UNUSED(BAD_SIZE)//,"Could not determine size.\n",0)
MESSAGE(REAL_NO_FILENAME,"Apparent real sprite does not contain a file name.\n",TO_NULL|MAKE_COMMENT)
NFO_MESSAGE(REAL_MISSING_DATA,"Could not read %s from apparent real sprite.\n",0)

//FATAL
NFO_MESSAGE(INVALID_LENGTH,"All %S are %S bytes long.\n",USE_PREFIX)
NFO_MESSAGE(BAD_LENGTH,"Length does not match %S of %S. (Expected %d bytes)\n",USE_PREFIX)
NFO_MESSAGE(TOO_SHORT,"Linter requires %d byte(s) which do(es) not exist.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_FEATURE,"Invalid feature byte.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_TYPE,"Invalid nument1 or random-/variational-type byte.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_ACTION,"Invalid action byte.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_PROP,"Invalid property %2x.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(UNTERM_ACT6,"Action 6 appears to be missing terminating FF.\n",USE_PREFIX)

//ERROR
NFO_MESSAGE(INSUFFICIENT,"Missing %s. Sprite %d declared %d, but only %d have been seen.\n",USE_PREFIX)
NFO_MESSAGE(EXTRA,"Extra %s. The %d declared by sprite %d have already been seen.\n",USE_PREFIX)
NFO_MESSAGE(UNEXPECTED,"Unexpected %s found.\n",USE_PREFIX)
NFO_MESSAGE(FEATURE_MISMATCH,"Feature byte does not match feature byte of preceding Action %x at sprite %d.\n",USE_PREFIX)
NFO_MESSAGE(RAND_2_NUMSETS,"Rand02s require power-of-two sets to choose from.\n",USE_PREFIX)
  MESSAGE_UNUSED(NEITHER_SET_CALLBACK)
NFO_MESSAGE(NEITHER_ID_CALLBACK,"%4x is neither callback nor %s.\n",USE_PREFIX)
NFO_MESSAGE(UNDEFINED_SPRITE_SET,"Sprite set %2x does not appear in the preceeding Action 1 (sprite %d).\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_CARGO_TYPE,"Invalid cargo type: %2x\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_REQD_SETS,"Action 2 declaring no %s sets.\n",USE_PREFIX)
NFO_MESSAGE(SIGNALS,"Signals require 48, 112, or 240 (0x30, 0x70, or 0xF0) sprites.\n",USE_PREFIX)
NFO_MESSAGE(ACTION_5,"Expected %d (0x%x) sprites for this type.\n",USE_PREFIX)
  MESSAGE_UNUSED(FOUNDATIONS)
NFO_MESSAGE(UNDEFINED_ID,"ID %2x has not been defined.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INSUFFICIENT_PROPS,"Expected %d more properties.\n",USE_PREFIX)
NFO_MESSAGE(INSUFFICIENT_DATA,"Expected more data for prop %2x. (%d bytes at %d.)\n",USE_PREFIX)
NFO_MESSAGE(INSUFFICIENT_DATA2,"Expected %d more bytes for prop %2x.\n",USE_PREFIX)
NFO_MESSAGE(MISSING_TERMINATOR,"Property data is missing terminating byte(s).\n",USE_PREFIX)
NFO_MESSAGE(DUPLICATE_ACT,"Action %x already found at sprite %d.\n",USE_PREFIX)
NFO_MESSAGE(MISSING_8,"An action 8 must preceed action %x.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_VERSION,"Invalid GRF version number.\n",USE_PREFIX)
  MESSAGE_UNUSED(BAD_VARIABLE)
NFO_MESSAGE(BAD_VARSIZE,"Variable size %2x is invalid.\n",USE_PREFIX)
NFO_MESSAGE(BAD_CONDITION,"Condition %2x is invalid.\n",USE_PREFIX)
NFO_MESSAGE(GRFCOND_NEEDS_GRFVAR,"Condition %2x requires variable 88.\n",USE_PREFIX)
NFO_MESSAGE(GRFVAR_NEEDS_GRFCOND,"Variable 88 requires a GRFid condition.\n",USE_PREFIX)
NFO_MESSAGE(BITTEST_VARIABLE,"Variable %2x requires a bit-test condition.\n",USE_PREFIX)
NFO_MESSAGE(VARIABLE_SIZE_MISMATCH,"Variable %2x is %d byte(s) long.\n",USE_PREFIX)
  MESSAGE_UNUSED(INVALID_LABEL)//,"Labels must have the high 3 bits set (E0-FF).\n",USE_PREFIX)
NFO_MESSAGE(INVALID_LITERAL,"%2x was found where a literal %2x was expected.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INSUFFICIENT_STRINGS,"Expected %d more string(s) for this action 4.\n",USE_PREFIX)
NFO_MESSAGE(TOO_LARGE,"Value of <%s> must not exceed 0Fh.\n",USE_PREFIX)
NFO_MESSAGE(SPRITENUM_TOO_HIGH,"TTD only defines sprites up to 4984 (1E 13).\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_COLOR_TRANS,"Color translation mode 3 is undefined.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_COLOR_SPRITE,"Sprite %d is not a color translation sprite.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_GROUNDSPRITE,"No %s sprite was specified.\n",USE_PREFIX)
NFO_MESSAGE(NO_STD_3,"This livery override action 3 does not follow a standard action 3.\n",USE_PREFIX)
NFO_MESSAGE(NONEXISTANT_VARIABLE,"Testing nonexistant variable %2x.\n",USE_PREFIX)
NFO_MESSAGE(MODIFY_BEYOND_LENGTH,"Action 6 attempts to modify a byte after the end of this sprite.\n",USE_PREFIX)
NFO_MESSAGE(NO_FOLLOWING_SPRITE,"There is no following sprite for this Action 6 to modify.\n",USE_PREFIX)

//WARNING1
NFO_MESSAGE(NO_SETS,"Action %x declaring no sets.\n",USE_PREFIX)
NFO_MESSAGE(NO_SPRITES,"Action %x declaring sets with no sprites.\n",USE_PREFIX)
NFO_MESSAGE(SET_WITH_NO_SPRITES,"Set %d contains no sprites.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_PROPS,"Action 0 setting no properties.\n",USE_PREFIX)
NFO_MESSAGE(NO_IDS,"Action 0 setting properties for no IDs.\n",USE_PREFIX)
NFO_MESSAGE(UNUSED_ID,"Redefining ID %2x not used since previous definition at sprite %d.\n",USE_PREFIX)
MESSAGE_EX(UNUSED2IDLEAD,"For feature %x the following cargoIDs have not been used since their most recent definition:\n",MAKE_COMMENT,NFO,UNUSED_ID)
MESSAGE_EX(UNUSEDIDFINAL,"%2x (last defined at sprite %d)\n",MAKE_COMMENT,NFO,UNUSED_ID)
NFO_MESSAGE(UNUSED_SET,"Set %2x defined by the previous Action 1 (sprite %d) has not been used.\n",USE_PREFIX)
NFO_MESSAGE(UNREACHABLE_VAR,"Variation %d cannot be reached.\n",USE_PREFIX)
NFO_MESSAGE(EXTRA_DATA,"No more data was expected.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(REUSED_DEFAULT,"Default ID appears earlier in sprite.\n",USE_PREFIX)
NFO_MESSAGE(RESERVED_GRFID,"GRFIDs with a first byte of FF are reserved.\n",USE_PREFIX)
  MESSAGE_UNUSED(DOES_NOT_FOLLOW)
NFO_MESSAGE(SHIFT_TOO_FAR,"Shifting variable %2x past its length.\n",USE_PREFIX)
NFO_MESSAGE(NO_MODIFICATIONS,"Action 6 does not make any changes.\n",USE_PREFIX)
NFO_MESSAGE(DOES_NOT_MODIFY,"This section does not make any changes.\n",USE_PREFIX|HAS_OFFSET)

//WARNING2
  MESSAGE_UNUSED(BACKWARDS_GOTO)//,"Action %x jumping backwards to label %2x defined at sprite %d.\n",USE_PREFIX)
NFO_MESSAGE(UNUSED_LABEL_NO_PREV_DEF,"Label %2x has not been goto'ed.\n",USE_PREFIX)
NFO_MESSAGE(UNUSED_LABEL_PREV_DEF,"Label %2x has not been goto'ed since previous definition at sprite %d.\n",USE_PREFIX)
NFO_MESSAGE(REPEATED_PROP,"Property %2x is previously defined at offset %d.\n",USE_PREFIX|HAS_OFFSET)
 MESSAGE_UNUSED(OFF_TILE)

//WARNING3
NFO_MESSAGE(ONLY_ONE_CHOICE,"Random 2 declaring only one choice.\n",USE_PREFIX)
NFO_MESSAGE(NOT_VARIATIONAL,"Variational 2 accesses only constant variables (1A & 1C).\n",USE_PREFIX)
NFO_MESSAGE(NOT_RANDOM,"All random options have the same ID.\n",USE_PREFIX)
  MESSAGE_UNUSED(TOO_MANY_STRINGS)

NFO_MESSAGE(NO_ACT1,"No preceeding action 1.\n",USE_PREFIX)
  MESSAGE_UNUSED(INVALID_AGAIN)//,"<again> must be 00 or 01.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_OP,"Invalid operation %2x.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(SET_TOO_LARGE,"Sprite set contains more sprites than given feature can use.\n",USE_PREFIX)
NFO_MESSAGE(STRANGE_SET_SIZE,"Sprite set contains an unusual number of sprites.\n",USE_PREFIX)
MESSAGE_EX(UNUSEDFIDLEAD,"For new town names, the following IDs have not been used since their most recent definition:\n",MAKE_COMMENT,NFO,UNUSED_ID)
NFO_MESSAGE(DUPLICATE_GRFID,"GRF ID previously deactivated at offset %d (%8x).\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_GRFIDS,"Action E is not disabling any GRF files.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_SRC,"Source parameter %d is invalid.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_TARGET,"Target parameter is invalid.\n",USE_PREFIX)
NFO_MESSAGE(ONLY_ONE_DATA,"Only one of src1 and src2 may be 0xFF.\n",USE_PREFIX)
NFO_MESSAGE(DUPLICATE_LANG_NAME,"A name for %L was previously specified at offset %d.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(MISSING_FALLBACK,"No fallback string was specified.\n",USE_PREFIX)
NFO_MESSAGE(NO_PARTS,"Zero parts were specified.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OUT_OF_RANGE_BITS,"Only %d random bits are available.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INSUFFICIENT_BITS,"%x bits are required, but only %2x were allocated.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_PROBABILITY,"A probability of 0 was specified for this part.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(PARAM_TOO_LARGE,"Parameter %2x out of valid range for variable %2x.\n",USE_PREFIX)
NFO_MESSAGE(NO_BUILDING_SPRITE,"Building sprites may not be null.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OVERRAN_SPRITE,"Unexpected end-of-sprite reading building sprite %d.\n",USE_PREFIX)
NFO_MESSAGE(FIRST_SPRITE_CANNOT_SHARE,"First building sprite cannot share its bounding box with the previous sprite.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_GRFID,"Action %x references reserved GRFid.\n",USE_PREFIX|HAS_OFFSET)
  MESSAGE_UNUSED(INSUFFICIENT_INCLUDE)
  MESSAGE_UNUSED(EXTRA_INCLUDE)
  MESSAGE_UNUSED(UNEXPECTED_INCLUDE)
NFO_MESSAGE(OOR_COUNT,"Attempting to manage too many IDs.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_ID,"ID %2x out of valid range (%2x..%2x).\n",USE_PREFIX)
NFO_MESSAGE(NO_CARGOTYPES,"<num-cid> must be 0 for this feature.\n",USE_PREFIX)
NFO_MESSAGE(UNUSED_CONTROL,"Using unspecified control character %2x.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_CONTROL,"Control character %2x should not be used in this string.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(SMASH_STACK,"Control character %2x does not match %K on top of stack.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(SMASH_STACK_SPAN,"Control character %2x reads data from multiple parameters.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OVERRAN_STACK,"Insufficient stack data for control character %2x.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(CANNOT_SHUFFLE,"There is insufficient stack data to shuffle the stack.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(NO_NULL_FOUND,"No terminating null found.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_SEVERITY,"Invalid severity byte.\n",USE_PREFIX)
NFO_MESSAGE(UNKNOWN_LANG_BIT,"Unrecognized bit set in language-id byte %2x.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_MESSAGEID,"Invalid message id.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_PARAM,"%2x is not a GRF parameter.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_OVERRIDE,"Cannot override graphics for this feature.\n",USE_PREFIX)
NFO_MESSAGE(COND_SIZE_MISMATCH,"Condition %2x requires %d byte(s).\n",USE_PREFIX)
NFO_MESSAGE(REAL_VAL_TOO_SMALL,"Metadata invalid. %s must not be smaller than %d.\n",USE_PREFIX)
NFO_MESSAGE(REAL_VAL_TOO_LARGE,"Metadata invalid. %s must not be larger than %d.\n",USE_PREFIX)
NFO_MESSAGE(REAL_MOVES_UP,"Metadata invalid. ypos must be non-decreasing.\n",USE_PREFIX)
NFO_MESSAGE(REAL_BAD_COMP,"Metadata invalid. compression must be one of 01, 03, 09, or 0B.\n",USE_PREFIX)
NFO_MESSAGE(REAL_SPRITE_TOO_LARGE,"Metadata invalid. Sprite size exceeds 64K.\n",USE_PREFIX)
NFO_MESSAGE(INVALID_TEXTID,"Text ID %4x is not a valid text ID.\n",USE_PREFIX|HAS_OFFSET)
  MESSAGE_UNUSED(DWORD_CALLBACK)
NFO_MESSAGE(CHECK_0C_RANGE,"Checking for var0C in the range [%2x,%2x].\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_CALLBACK,"%2x is not a valid callback for this feature.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_FONT,"%2x is not a valid font.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(SPANS_BLOCKS,"Set %d declares glyphs across a block-break.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OVERRAN_F_NAME,"The name for language(s) %2x has no terminating null.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OVERRAN_F_PART,"The name-part starting at %d has no terminating null.\n",USE_PREFIX)
NFO_MESSAGE(OBSCURED_VARIATION,"Variation %d is partially obscured.\n",USE_PREFIX)
NFO_MESSAGE(UNREACHABLE_DEFAULT,"Default result cannot be reached.\n",USE_PREFIX)
NFO_MESSAGE(DIVIDE_BY_ZERO,"var-adjust performs a divide-by-zero or modulo-zero operation.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(USE_SHIFT_AND,"A shift-and var-adjust would suffice here.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_SHIFT,"shift may not have both high bits set.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(UNDEFINED_TRIGGER,"Using an undefined trigger.\n",USE_PREFIX)
NFO_MESSAGE(OVERLENGTH_UTF8,"Invalid UTF-8 sequence: Overlength representation.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(INVALID_UTF8,"Invalid UTF-8 sequence: First byte not followed by sufficient continuation bytes.\n",USE_PREFIX|HAS_OFFSET)
NFO_MESSAGE(OUTOFRANGE_UTF8,"Invalid UTF-8 sequence: Encodes a character beyond the BMP.\n",USE_PREFIX|HAS_OFFSET)

  MESSAGE_UNUSED(UNEXPECTED_UTF8_CONT)
NFO_MESSAGE(UNKNOWN_LANGUAGE,"Language %2x is not defined.\n",USE_PREFIX|HAS_OFFSET)

/* Insert new NFO_MESSAGEs here. */

//VMESSAGEs
//Level 1
//VMESSAGE(IDS,"Setting IDs %x to %x (%d IDs).\n")

//Level 2
//VMESSAGE(ACTION_FOR,"Action %d %Y for feature %Y.\n")

/* Insert new VMESSAGEs here. */

END_MESSAGES()

#endif//_RENUM_MESSAGES_H_INCLUDED_