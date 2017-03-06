#ifndef ZIPDEFINES_H
#define ZIPDEFINES_H


// THIS FILE is almost entirely based upon code by info-zip.
// It has been modified by Lucian Wischik. The modifications
// were a complete rewrite of the bit of code that generates the
// layout of the zipfile, and support for zipping to/from memory
// or handles or pipes or pagefile or diskfiles, encryption, unicode.
// The original code may be found at http://www.info-zip.org
// The original copyright text follows.
//
//
//
// This is version 1999-Oct-05 of the Info-ZIP copyright and license.
// The definitive version of this document should be available at
// ftp://ftp.cdrom.com/pub/infozip/license.html indefinitely.
//
// Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.
//
// For the purposes of this copyright and license, "Info-ZIP" is defined as
// the following set of individuals:
//
//   Mark Adler, John Bush, Karl Davis, Harald Denker, Jean-Michel Dubois,
//   Jean-loup Gailly, Hunter Goatley, Ian Gorman, Chris Herborth, Dirk Haase,
//   Greg Hartwig, Robert Heath, Jonathan Hudson, Paul Kienitz, David Kirschbaum,
//   Johnny Lee, Onno van der Linden, Igor Mandrichenko, Steve P. Miller,
//   Sergio Monesi, Keith Owens, George Petrov, Greg Roelofs, Kai Uwe Rommel,
//   Steve Salisbury, Dave Smith, Christian Spieler, Antoine Verheijen,
//   Paul von Behren, Rich Wales, Mike White
//
// This software is provided "as is," without warranty of any kind, express
// or implied.  In no event shall Info-ZIP or its contributors be held liable
// for any direct, indirect, incidental, special or consequential damages
// arising out of the use of or inability to use this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. Redistributions of source code must retain the above copyright notice,
//       definition, disclaimer, and this list of conditions.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, definition, disclaimer, and this list of conditions in
//       documentation and/or other materials provided with the distribution.
//
//    3. Altered versions--including, but not limited to, ports to new operating
//       systems, existing ports with new graphical interfaces, and dynamic,
//       shared, or static library versions--must be plainly marked as such
//       and must not be misrepresented as being the original source.  Such
//       altered versions also must not be misrepresented as being Info-ZIP
//       releases--including, but not limited to, labeling of the altered
//       versions with the names "Info-ZIP" (or any variation thereof, including,
//       but not limited to, different capitalizations), "Pocket UnZip," "WiZ"
//       or "MacZip" without the explicit permission of Info-ZIP.  Such altered
//       versions are further prohibited from misrepresentative use of the
//       Zip-Bugs or Info-ZIP e-mail addresses or of the Info-ZIP URL(s).
//
//    4. Info-ZIP retains the right to use the names "Info-ZIP," "Zip," "UnZip,"
//       "WiZ," "Pocket UnZip," "Pocket Zip," and "MacZip" for its own source and
//       binary releases.
//


typedef unsigned char uch;      // unsigned 8-bit value
typedef unsigned short ush;     // unsigned 16-bit value
typedef unsigned long ulg;      // unsigned 32-bit value
typedef size_t extent;          // file size
typedef unsigned Pos;   // must be at least 32 bits
typedef unsigned IPos; // A Pos is an index in the character window. Pos is used only for parameter passing

#ifndef EOF
#define EOF (-1)
#endif


// Error return values.  The values 0..4 and 12..18 follow the conventions
// of PKZIP.   The values 4..10 are all assigned to "insufficient memory"
// by PKZIP, so the codes 5..10 are used here for other purposes.
#define ZE_MISS         -1      // used by procname(), zipbare()
#define ZE_OK           0       // success
#define ZE_EOF          2       // unexpected end of zip file
#define ZE_FORM         3       // zip file structure error
#define ZE_MEM          4       // out of memory
#define ZE_LOGIC        5       // internal logic error
#define ZE_BIG          6       // entry too large to split
#define ZE_NOTE         7       // invalid comment format
#define ZE_TEST         8       // zip test (-T) failed or out of memory
#define ZE_ABORT        9       // user interrupt or termination
#define ZE_TEMP         10      // error using a temp file
#define ZE_READ         11      // read or seek error
#define ZE_NONE         12      // nothing to do
#define ZE_NAME         13      // missing or empty zip file
#define ZE_WRITE        14      // error writing to a file
#define ZE_CREAT        15      // couldn't open to write
#define ZE_PARMS        16      // bad command line
#define ZE_OPEN         18      // could not open a specified file to read
#define ZE_MAXERR       18      // the highest error number


// internal file attribute
#define UNKNOWN (-1)
#define BINARY  0
#define ASCII   1

#define BEST -1                 // Use best method (deflation or store)
#define STORE 0                 // Store method
#define DEFLATE 8               // Deflation method

#define CRCVAL_INITIAL  0L

// MSDOS file or directory attributes
#define MSDOS_HIDDEN_ATTR 0x02
#define MSDOS_DIR_ATTR 0x10

// Lengths of headers after signatures in bytes
#define LOCHEAD 26
#define CENHEAD 42
#define ENDHEAD 18

// Definitions for extra field handling:
#define EB_HEADSIZE       4     /* length of a extra field block header */
#define EB_LEN            2     /* offset of data length field in header */
#define EB_UT_MINLEN      1     /* minimal UT field contains Flags byte */
#define EB_UT_FLAGS       0     /* byte offset of Flags field */
#define EB_UT_TIME1       1     /* byte offset of 1st time value */
#define EB_UT_FL_MTIME    (1 << 0)      /* mtime present */
#define EB_UT_FL_ATIME    (1 << 1)      /* atime present */
#define EB_UT_FL_CTIME    (1 << 2)      /* ctime present */
#define EB_UT_LEN(n)      (EB_UT_MINLEN + 4 * (n))
#define EB_L_UT_SIZE    (EB_HEADSIZE + EB_UT_LEN(3))
#define EB_C_UT_SIZE    (EB_HEADSIZE + EB_UT_LEN(1))


// Macros for writing machine integers to little-endian format
#define PUTSH(a,f) {char _putsh_c=(char)((a)&0xff); wfunc(param,&_putsh_c,1); _putsh_c=(char)((a)>>8); wfunc(param,&_putsh_c,1);}
#define PUTLG(a,f) {PUTSH((a) & 0xffff,(f)) PUTSH((a) >> 16,(f))}


// -- Structure of a ZIP file --
// Signatures for zip file information headers
#define LOCSIG     0x04034b50L
#define CENSIG     0x02014b50L
#define ENDSIG     0x06054b50L
#define EXTLOCSIG  0x08074b50L


#define MIN_MATCH  3
#define MAX_MATCH  258
// The minimum and maximum match lengths


#define WSIZE  (0x8000)
// Maximum window size = 32K. If you are really short of memory, compile
// with a smaller WSIZE but this reduces the compression ratio for files
// of size > WSIZE. WSIZE must be a power of two in the current implementation.
//

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
// Minimum amount of lookahead, except at the end of the input file.
// See deflate.c for comments about the MIN_MATCH+1.
//

#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)
// In order to simplify the code, particularly on 16 bit machines, match
// distances are limited to MAX_DIST instead of WSIZE.
//


#define ZIP_HANDLE   1
#define ZIP_FILENAME 2
#define ZIP_MEMORY   3
#define ZIP_FOLDER   4



// ===========================================================================
// Constants
//

#define MAX_BITS 15
// All codes must not exceed MAX_BITS bits

#define MAX_BL_BITS 7
// Bit length codes must not exceed MAX_BL_BITS bits

#define LENGTH_CODES 29
// number of length codes, not counting the special END_BLOCK code

#define LITERALS  256
// number of literal bytes 0..255

#define END_BLOCK 256
// end of block literal code

#define L_CODES (LITERALS+1+LENGTH_CODES)
// number of Literal or Length codes, including the END_BLOCK code

#define D_CODES   30
// number of distance codes

#define BL_CODES  19
// number of codes used to transfer the bit lengths


#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2
// The three kinds of block type

#define LIT_BUFSIZE  0x8000
#define DIST_BUFSIZE  LIT_BUFSIZE
// Sizes of match buffers for literals/lengths and distances.  There are
// 4 reasons for limiting LIT_BUFSIZE to 64K:
//   - frequencies can be kept in 16 bit counters
//   - if compression is not successful for the first block, all input data is
//     still in the window so we can still emit a stored block even when input
//     comes from standard input.  (This can also be done for all blocks if
//     LIT_BUFSIZE is not greater than 32K.)
//   - if compression is not successful for a file smaller than 64K, we can
//     even emit a stored file instead of a stored block (saving 5 bytes).
//   - creating new Huffman trees less frequently may not provide fast
//     adaptation to changes in the input data statistics. (Take for
//     example a binary file with poorly compressible code followed by
//     a highly compressible string table.) Smaller buffer sizes give
//     fast adaptation but have of course the overhead of transmitting trees
//     more frequently.
//   - I can't count above 4
// The current code is general and allows DIST_BUFSIZE < LIT_BUFSIZE (to save
// memory at the expense of compression). Some optimizations would be possible
// if we rely on DIST_BUFSIZE == LIT_BUFSIZE.
//

#define REP_3_6      16
// repeat previous bit length 3-6 times (2 bits of repeat count)

#define REPZ_3_10    17
// repeat a zero length 3-10 times  (3 bits of repeat count)

#define REPZ_11_138  18
// repeat a zero length 11-138 times  (7 bits of repeat count)

#define HEAP_SIZE (2*L_CODES+1)
// maximum heap size


// ===========================================================================
// Local data used by the "bit string" routines.
//

#define Buf_size (8 * 2*sizeof(char))
// Number of bits used within bi_buf. (bi_buf may be implemented on
// more than 16 bits on some systems.)

// Output a 16 bit value to the bit stream, lower (oldest) byte first
#define PUTSHORT(state,w) \
{ if (state.bs.out_offset >= state.bs.out_size-1) \
    state.flush_outbuf(state.param,state.bs.out_buf, &state.bs.out_offset); \
  state.bs.out_buf[state.bs.out_offset++] = (char) ((w) & 0xff); \
  state.bs.out_buf[state.bs.out_offset++] = (char) ((ush)(w) >> 8); \
}

#define PUTBYTE(state,b) \
{ if (state.bs.out_offset >= state.bs.out_size) \
    state.flush_outbuf(state.param,state.bs.out_buf, &state.bs.out_offset); \
  state.bs.out_buf[state.bs.out_offset++] = (char) (b); \
}

// DEFLATE.CPP HEADER

#define HASH_BITS  15
// For portability to 16 bit machines, do not use values above 15.

#define HASH_SIZE (unsigned)(1<<HASH_BITS)
#define HASH_MASK (HASH_SIZE-1)
#define WMASK     (WSIZE-1)
// HASH_SIZE and WSIZE must be powers of two

#define NIL 0
// Tail of hash chains

#define FAST 4
#define SLOW 2
// speed options for the general purpose bit flag

#define TOO_FAR 4096
// Matches of length 3 are discarded if their distance exceeds TOO_FAR



#define EQUAL 0
// result of memcmp for equal strings


// ===========================================================================
// Local data used by the "longest match" routines.

#define H_SHIFT  ((HASH_BITS+MIN_MATCH-1)/MIN_MATCH)
// Number of bits by which ins_h and del_h must be shifted at each
// input step. It must be such that after MIN_MATCH steps, the oldest
// byte no longer takes part in the hash key, that is:
//   H_SHIFT * MIN_MATCH >= HASH_BITS

#define max_insert_length  max_lazy_match
// Insert new strings in the hash table only if the match length
// is not greater than this length. This saves time but degrades compression.
// max_insert_length is used only for compression levels <= 3.



const int extra_lbits[LENGTH_CODES] // extra bits for each length code
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

const int extra_dbits[D_CODES] // extra bits for each distance code
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

const int extra_blbits[BL_CODES]// extra bits for each bit length code
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

const uch bl_order[BL_CODES] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
// The lengths of the bit length codes are sent in order of decreasing
// probability, to avoid transmitting the lengths for unused bit length codes.


typedef struct config {
   ush good_length; // reduce lazy search above this match length
   ush max_lazy;    // do not perform lazy search above this match length
   ush nice_length; // quit search above this match length
   ush max_chain;
} config;

// Values for max_lazy_match, good_match, nice_match and max_chain_length,
// depending on the desired pack level (0..9). The values given below have
// been tuned to exclude worst case performance for pathological files.
// Better values may be found for specific files.
//

const config configuration_table[10] = {
//  good lazy nice chain
    {0,    0,  0,    0},  // 0 store only
    {4,    4,  8,    4},  // 1 maximum speed, no lazy matches
    {4,    5, 16,    8},  // 2
    {4,    6, 32,   32},  // 3
    {4,    4, 16,   16},  // 4 lazy matches */
    {8,   16, 32,   32},  // 5
    {8,   16, 128, 128},  // 6
    {8,   32, 128, 256},  // 7
    {32, 128, 258, 1024}, // 8
    {32, 258, 258, 4096}};// 9 maximum compression */

// Note: the deflate() code requires max_lazy >= MIN_MATCH and max_chain >= 4
// For deflate_fast() (levels <= 3) good is ignored and lazy has a different meaning.







// Data structure describing a single value and its code string.
typedef struct ct_data {
    union {
        ush  freq;       // frequency count
        ush  code;       // bit string
    } fc;
    union {
        ush  dad;        // father node in Huffman tree
        ush  len;        // length of bit string
    } dl;
} ct_data;

typedef struct tree_desc {
    ct_data *dyn_tree;      // the dynamic tree
    ct_data *static_tree;   // corresponding static tree or NULL
    const int *extra_bits;  // extra bits for each code or NULL
    int     extra_base;     // base index for extra_bits
    int     elems;          // max number of elements in the tree
    int     max_length;     // max bit length for the codes
    int     max_code;       // largest code with non zero frequency
} tree_desc;



#endif // ZIPDEFINES_H
