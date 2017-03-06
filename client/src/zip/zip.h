#ifndef _zip_H
#define _zip_H

#include "zipdefines.h"

// ZIP functions -- for creating zip files
// This file is a repackaged form of the Info-Zip source code available
// at www.info-zip.org. The original copyright notice may be found in
// zip.cpp. The repackaging was done by Lucian Wischik to simplify and
// extend its use in Windows/C++. Also to add encryption and unicode.


#ifndef _unzip_H
DECLARE_HANDLE(HZIP);
#endif
// An HZIP identifies a zip file that is being created

typedef DWORD ZRESULT;
// return codes from any of the zip functions. Listed later.



HZIP CreateZip(const TCHAR *fn, const char *password);
HZIP CreateZip(void *buf,unsigned int len, const char *password);
HZIP CreateZipHandle(HANDLE h, const char *password);
// CreateZip - call this to start the creation of a zip file.
// As the zip is being created, it will be stored somewhere:
// to a pipe:              CreateZipHandle(hpipe_write);
// in a file (by handle):  CreateZipHandle(hfile);
// in a file (by name):    CreateZip("c:\\test.zip");
// in memory:              CreateZip(buf, len);
// or in pagefile memory:  CreateZip(0, len);
// The final case stores it in memory backed by the system paging file,
// where the zip may not exceed len bytes. This is a bit friendlier than
// allocating memory with new[]: it won't lead to fragmentation, and the
// memory won't be touched unless needed. That means you can give very
// large estimates of the maximum-size without too much worry.
// As for the password, it lets you encrypt every file in the archive.
// (This api doesn't support per-file encryption.)
// Note: because pipes don't allow random access, the structure of a zipfile
// created into a pipe is slightly different from that created into a file
// or memory. In particular, the compressed-size of the item cannot be
// stored in the zipfile until after the item itself. (Also, for an item added
// itself via a pipe, the uncompressed-size might not either be known until
// after.) This is not normally a problem. But if you try to unzip via a pipe
// as well, then the unzipper will not know these things about the item until
// after it has been unzipped. Therefore: for unzippers which don't just write
// each item to disk or to a pipe, but instead pre-allocate memory space into
// which to unzip them, then either you have to create the zip not to a pipe,
// or you have to add items not from a pipe, or at least when adding items
// from a pipe you have to specify the length.
// Note: for windows-ce, you cannot close the handle until after CloseZip.
// but for real windows, the zip makes its own copy of your handle, so you
// can close yours anytime.


ZRESULT ZipAdd(HZIP hz,const TCHAR *dstzn, const TCHAR *fn);
ZRESULT ZipAdd(HZIP hz,const TCHAR *dstzn, void *src,unsigned int len);
ZRESULT ZipAddHandle(HZIP hz,const TCHAR *dstzn, HANDLE h);
ZRESULT ZipAddHandle(HZIP hz,const TCHAR *dstzn, HANDLE h, unsigned int len);
ZRESULT ZipAddFolder(HZIP hz,const TCHAR *dstzn);
// ZipAdd - call this for each file to be added to the zip.
// dstzn is the name that the file will be stored as in the zip file.
// The file to be added to the zip can come
// from a pipe:  ZipAddHandle(hz,"file.dat", hpipe_read);
// from a file:  ZipAddHandle(hz,"file.dat", hfile);
// from a filen: ZipAdd(hz,"file.dat", "c:\\docs\\origfile.dat");
// from memory:  ZipAdd(hz,"subdir\\file.dat", buf,len);
// (folder):     ZipAddFolder(hz,"subdir");
// Note: if adding an item from a pipe, and if also creating the zip file itself
// to a pipe, then you might wish to pass a non-zero length to the ZipAddHandle
// function. This will let the zipfile store the item's size ahead of the
// compressed item itself, which in turn makes it easier when unzipping the
// zipfile from a pipe.

ZRESULT ZipGetMemory(HZIP hz, void **buf, unsigned long *len);
// ZipGetMemory - If the zip was created in memory, via ZipCreate(0,len),
// then this function will return information about that memory block.
// buf will receive a pointer to its start, and len its length.
// Note: you can't add any more after calling this.

ZRESULT CloseZip(HZIP hz);
// CloseZip - the zip handle must be closed with this function.

unsigned int FormatZipMessage(ZRESULT code, TCHAR *buf,unsigned int len);
// FormatZipMessage - given an error code, formats it as a string.
// It returns the length of the error message. If buf/len points
// to a real buffer, then it also writes as much as possible into there.



class TTreeState
{
public:
  TTreeState();

  ct_data dyn_ltree[HEAP_SIZE];    // literal and length tree
  ct_data dyn_dtree[2*D_CODES+1];  // distance tree
  ct_data static_ltree[L_CODES+2]; // the static literal tree...
  // ... Since the bit lengths are imposed, there is no need for the L_CODES
  // extra codes used during heap construction. However the codes 286 and 287
  // are needed to build a canonical tree (see ct_init below).
  ct_data static_dtree[D_CODES]; // the static distance tree...
  // ... (Actually a trivial tree since all codes use 5 bits.)
  ct_data bl_tree[2*BL_CODES+1];  // Huffman tree for the bit lengths

  tree_desc l_desc;
  tree_desc d_desc;
  tree_desc bl_desc;

  ush bl_count[MAX_BITS+1];  // number of codes at each bit length for an optimal tree

  int heap[2*L_CODES+1]; // heap used to build the Huffman trees
  int heap_len;               // number of elements in the heap
  int heap_max;               // element of largest frequency
  // The sons of heap[n] are heap[2*n] and heap[2*n+1]. heap[0] is not used.
  // The same heap array is used to build all trees.

  uch depth[2*L_CODES+1];
  // Depth of each subtree used as tie breaker for trees of equal frequency

  uch length_code[MAX_MATCH-MIN_MATCH+1];
  // length code for each normalized match length (0 == MIN_MATCH)

  uch dist_code[512];
  // distance codes. The first 256 values correspond to the distances
  // 3 .. 258, the last 256 values correspond to the top 8 bits of
  // the 15 bit distances.

  int base_length[LENGTH_CODES];
  // First normalized length for each code (0 = MIN_MATCH)

  int base_dist[D_CODES];
  // First normalized distance for each code (0 = distance of 1)

  uch far l_buf[LIT_BUFSIZE];  // buffer for literals/lengths
  ush far d_buf[DIST_BUFSIZE]; // buffer for distances

  uch flag_buf[(LIT_BUFSIZE/8)];
  // flag_buf is a bit array distinguishing literals from lengths in
  // l_buf, and thus indicating the presence or absence of a distance.

  unsigned last_lit;    // running index in l_buf
  unsigned last_dist;   // running index in d_buf
  unsigned last_flags;  // running index in flag_buf
  uch flags;            // current flags not yet saved in flag_buf
  uch flag_bit;         // current bit used in flags
  // bits are filled in flags starting at bit 0 (least significant).
  // Note: these flags are overkill in the current code since we don't
  // take advantage of DIST_BUFSIZE == LIT_BUFSIZE.

  ulg opt_len;          // bit length of current block with optimal trees
  ulg static_len;       // bit length of current block with static trees

  ulg cmpr_bytelen;     // total byte length of compressed file
  ulg cmpr_len_bits;    // number of bits past 'cmpr_bytelen'

  ulg input_len;        // total byte length of input file
  // input_len is for debugging only since we can get it by other means.

  ush *file_type;       // pointer to UNKNOWN, BINARY or ASCII
//  int *file_method;     // pointer to DEFLATE or STORE
};



class TBitState
{
public:
  TBitState();
  int flush_flg;
  //
  unsigned bi_buf;
  // Output buffer. bits are inserted starting at the bottom (least significant
  // bits). The width of bi_buf must be at least 16 bits.
  int bi_valid;
  // Number of valid bits in bi_buf.  All bits above the last valid bit
  // are always zero.
  char *out_buf;
  // Current output buffer.
  unsigned out_offset;
  // Current offset in output buffer.
  // On 16 bit machines, the buffer is limited to 64K.
  unsigned out_size;
  // Size of current output buffer
  ulg bits_sent;   // bit length of the compressed data  only needed for debugging???
};

class TDeflateState
{
public:
  TDeflateState();

  uch    window[2L*WSIZE];
  // Sliding window. Input bytes are read into the second half of the window,
  // and move to the first half later to keep a dictionary of at least WSIZE
  // bytes. With this organization, matches are limited to a distance of
  // WSIZE-MAX_MATCH bytes, but this ensures that IO is always
  // performed with a length multiple of the block size. Also, it limits
  // the window size to 64K, which is quite useful on MSDOS.
  // To do: limit the window size to WSIZE+CBSZ if SMALL_MEM (the code would
  // be less efficient since the data would have to be copied WSIZE/CBSZ times)
  Pos    prev[WSIZE];
  // Link to older string with same hash index. To limit the size of this
  // array to 64K, this link is maintained only for the last 32K strings.
  // An index in this array is thus a window index modulo 32K.
  Pos    head[HASH_SIZE];
  // Heads of the hash chains or NIL. If your compiler thinks that
  // HASH_SIZE is a dynamic value, recompile with -DDYN_ALLOC.

  ulg window_size;
  // window size, 2*WSIZE except for MMAP or BIG_MEM, where it is the
  // input file length plus MIN_LOOKAHEAD.

  long block_start;
  // window position at the beginning of the current output block. Gets
  // negative when the window is moved backwards.

  int sliding;
  // Set to false when the input file is already in memory

  unsigned ins_h;  // hash index of string to be inserted

  unsigned int prev_length;
  // Length of the best match at previous step. Matches not greater than this
  // are discarded. This is used in the lazy match evaluation.

  unsigned strstart;         // start of string to insert
  unsigned match_start; // start of matching string
  int      eofile;           // flag set at end of input file
  unsigned lookahead;        // number of valid bytes ahead in window

  unsigned max_chain_length;
  // To speed up deflation, hash chains are never searched beyond this length.
  // A higher limit improves compression ratio but degrades the speed.

  unsigned int max_lazy_match;
  // Attempt to find a better match only when the current match is strictly
  // smaller than this value. This mechanism is used only for compression
  // levels >= 4.

  unsigned good_match;
  // Use a faster search when the previous match is longer than this

  int nice_match; // Stop searching when current match exceeds this
};

// These are the result codes:
#define ZR_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define ZR_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define ZR_GENMASK    0x0000FF00
#define ZR_NODUPH     0x00000100     // couldn't duplicate the handle
#define ZR_NOFILE     0x00000200     // couldn't create/open the file
#define ZR_NOALLOC    0x00000300     // failed to allocate some resource
#define ZR_WRITE      0x00000400     // a general error writing to the file
#define ZR_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define ZR_MORE       0x00000600     // there's still more data to be unzipped
#define ZR_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define ZR_READ       0x00000800     // a general error reading the file
// The following come from mistakes on the part of the caller
#define ZR_CALLERMASK 0x00FF0000
#define ZR_ARGS       0x00010000     // general mistake with the arguments
#define ZR_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define ZR_MEMSIZE    0x00030000     // the memory size is too small
#define ZR_FAILED     0x00040000     // the thing was already failed when you called this function
#define ZR_ENDED      0x00050000     // the zip creation has already been closed
#define ZR_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define ZR_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define ZR_ZMODE      0x00080000     // tried to mix creating/opening a zip
// The following come from bugs within the zip library itself
#define ZR_BUGMASK    0xFF000000
#define ZR_NOTINITED  0x01000000     // initialisation didn't work
#define ZR_SEEK       0x02000000     // trying to seek in an unseekable file
#define ZR_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define ZR_FLATE      0x05000000     // an internal error in the de/inflation code






// e.g.
//
// (1) Traditional use, creating a zipfile from existing files
//     HZIP hz = CreateZip("c:\\simple1.zip",0);
//     ZipAdd(hz,"znsimple.bmp", "c:\\simple.bmp");
//     ZipAdd(hz,"znsimple.txt", "c:\\simple.txt");
//     CloseZip(hz);
//
// (2) Memory use, creating an auto-allocated mem-based zip file from various sources
//     HZIP hz = CreateZip(0,100000, 0);
//     // adding a conventional file...
//     ZipAdd(hz,"src1.txt",  "c:\\src1.txt");
//     // adding something from memory...
//     char buf[1000]; for (int i=0; i<1000; i++) buf[i]=(char)(i&0x7F);
//     ZipAdd(hz,"file.dat",  buf,1000);
//     // adding something from a pipe...
//     HANDLE hread,hwrite; CreatePipe(&hread,&hwrite,NULL,0);
//     HANDLE hthread = CreateThread(0,0,ThreadFunc,(void*)hwrite,0,0);
//     ZipAdd(hz,"unz3.dat",  hread,1000);  // the '1000' is optional.
//     WaitForSingleObject(hthread,INFINITE);
//     CloseHandle(hthread); CloseHandle(hread);
//     ... meanwhile DWORD WINAPI ThreadFunc(void *dat)
//                   { HANDLE hwrite = (HANDLE)dat;
//                     char buf[1000]={17};
//                     DWORD writ; WriteFile(hwrite,buf,1000,&writ,NULL);
//                     CloseHandle(hwrite);
//                     return 0;
//                   }
//     // and now that the zip is created, let's do something with it:
//     void *zbuf; unsigned long zlen; ZipGetMemory(hz,&zbuf,&zlen);
//     HANDLE hfz = CreateFile("test2.zip",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
//     DWORD writ; WriteFile(hfz,zbuf,zlen,&writ,NULL);
//     CloseHandle(hfz);
//     CloseZip(hz);
//
// (3) Handle use, for file handles and pipes
//     HANDLE hzread,hzwrite; CreatePipe(&hzread,&hzwrite,0,0);
//     HANDLE hthread = CreateThread(0,0,ZipReceiverThread,(void*)hzread,0,0);
//     HZIP hz = CreateZipHandle(hzwrite,0);
//     // ... add to it
//     CloseZip(hz);
//     CloseHandle(hzwrite);
//     WaitForSingleObject(hthread,INFINITE);
//     CloseHandle(hthread);
//     ... meanwhile DWORD WINAPI ZipReceiverThread(void *dat)
//                   { HANDLE hread = (HANDLE)dat;
//                     char buf[1000];
//                     while (true)
//                     { DWORD red; ReadFile(hread,buf,1000,&red,NULL);
//                       // ... and do something with this zip data we're receiving
//                       if (red==0) break;
//                     }
//                     CloseHandle(hread);
//                     return 0;
//                   }



// Now we indulge in a little skullduggery so that the code works whether
// the user has included just zip or both zip and unzip.
// Idea: if header files for both zip and unzip are present, then presumably
// the cpp files for zip and unzip are both present, so we will call
// one or the other of them based on a dynamic choice. If the header file
// for only one is present, then we will bind to that particular one.
ZRESULT CloseZipZ(HZIP hz);
unsigned int FormatZipMessageZ(ZRESULT code, char *buf,unsigned int len);
bool IsZipHandleZ(HZIP hz);
#ifdef _unzip_H
#undef CloseZip
#define CloseZip(hz) (IsZipHandleZ(hz)?CloseZipZ(hz):CloseZipU(hz))
#else
#define CloseZip CloseZipZ
#define FormatZipMessage FormatZipMessageZ
#endif



#endif
