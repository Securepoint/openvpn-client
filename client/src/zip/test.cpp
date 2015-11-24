#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../zip.h"
#include "../../unzip.h"

// This program runs a bunch of test cases on zip/unzip.
// Most of the test cases come from bugs that had been reported
// in an earlier version of the ziputils, to be sure that they're fixed.



bool tsame(const FILETIME t0, const FILETIME t1)
{ if (t0.dwHighDateTime!=t1.dwHighDateTime) return false;
  if ( (t0.dwLowDateTime>>28) != (t1.dwLowDateTime>>28)) return false;
  return true;
  // we allow some flexibility in the lower bits. That's because zip's don't store times with as much precision.
}

bool fsame(const TCHAR *fn0, const TCHAR *fn1)
{ HANDLE hf0=CreateFile(fn0,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
  HANDLE hf1=CreateFile(fn1,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
  if (hf0==INVALID_HANDLE_VALUE || hf1==INVALID_HANDLE_VALUE)
  { if (hf0!=INVALID_HANDLE_VALUE) CloseHandle(hf0);
    if (hf1!=INVALID_HANDLE_VALUE) CloseHandle(hf1);
    return false;
  }
  DWORD size0=GetFileSize(hf0,0), size1=GetFileSize(hf1,0);
  if (size0!=size1) {CloseHandle(hf0); CloseHandle(hf1); return false;}
  DWORD size=size0;
  //
  char *buf[2]; buf[0]=new char[16384]; buf[1]=new char[16384];
  DWORD done=0;
  while (done<size)
  { DWORD left=size-done; if (left>16384) left=16384;
    DWORD red; ReadFile(hf0,buf[0],left,&red,0); ReadFile(hf1,buf[1],left,&red,0);
    if (memcmp(buf[0],buf[1],left)!=0) break;
    done+=left;
  }
  delete[] buf[0]; delete[] buf[1];
  CloseHandle(hf0); CloseHandle(hf1);
  return (done==size);
}

void SaveResource(const TCHAR *res, const TCHAR *fn)
{ HINSTANCE hInstance = GetModuleHandle(0);
  HRSRC hrsrc=FindResource(hInstance,res,RT_RCDATA);
  HANDLE hglob = LoadResource(hInstance,hrsrc);
  void *buf=LockResource(hglob);
  unsigned int len=SizeofResource(hInstance,hrsrc);
  HANDLE hf = CreateFile(fn,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
  DWORD writ; WriteFile(hf,buf,len,&writ,0);
  CloseHandle(hf);
}

bool any_errors=false; bool p_abort=false;
void msg(const TCHAR *s)
{ if (s[0]=='*') any_errors=true;
#ifdef UNDER_CE
  int res=IDOK;
  if (s[0]=='*') res=MessageBox(0,s,_T("Zip error"), MB_ICONERROR|MB_OKCANCEL);
  else if (s[0]=='.') MessageBeep(0);
  else MessageBox(0,s,_T("Zip test"),MB_OKCANCEL);
  if (res==IDCANCEL) p_abort=true;
#else
  _tprintf(_T("%s\n"),s);
#endif
}


void main()
{ HZIP hz; HANDLE hf; DWORD writ; ZRESULT zr; ZIPENTRY ze; TCHAR m[1024];
  bool fast=false;
#ifdef UNDER_CE
  fast=true;
#endif
#ifdef __CODEGUARD__
  fast=true;
#endif
  

  msg(_T("Zip-utils tests. Files will be left in \"\\z\""));
  CreateDirectory(_T("\\z"),0);
  SaveResource(MAKEINTRESOURCE(1),_T("\\z\\extra.zip"));
  SaveResource(MAKEINTRESOURCE(2),_T("\\z\\ce2ce.jpg"));
  SaveResource(MAKEINTRESOURCE(3),_T("\\z\\ce2ce.txt"));


  

  // fixed bug: OpenZip errors and returns0 when you try to open a zip with no files in it
  msg(_T("empty - testing whether it fails to open empty zipfiles"));
  hz=CreateZip(_T("\\z\\empty.zip"),0); if (hz==0) msg(_T("* Failed to create empty.zip"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close empty.zip"));
  if (p_abort) return;
  hz=OpenZip(_T("\\z\\empty.zip"),0); if (hz==0) msg(_T("* Failed to open empty.zip"));
  zr=GetZipItem(hz,-1,&ze); if (zr!=ZR_OK) msg(_T("* Failed to get empty.zip index"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close empty.zip"));
  if (p_abort) return;

  // fixed bug: IsZipHandle should return false for a NULL handle.
  msg(_T("IsZipHandle - testing whether 0 is considered a handle"));
  bool b = IsZipHandleZ(0) || IsZipHandleU(0);
  if (b) msg(_T("IsZipHandle failed to deny handlehood of NULL"));
  if (p_abort) return;
  
  // fixed bug: if one file is bigger then the following must be smaller than 64k -- diff. between release and debug mode
  // fixed bug: test0=71k, test1=152.2k, test2=145b, test3=120k, here test3 returns ZR_WRITE
  msg(_T("sizes - testing whether large-then-small files work okay"));
  { char *c=new char[200*1024]; for (int i=0; i<200*1024; i++) c[i]=(char)(rand()%255);
    hf=CreateFile(_T("\\z\\sizes-71k.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,71*1024,&writ,0); CloseHandle(hf);
    hf=CreateFile(_T("\\z\\sizes-152_2k.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,152*1024+1024/5,&writ,0); CloseHandle(hf);
    hf=CreateFile(_T("\\z\\sizes-145b.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,145,&writ,0); CloseHandle(hf);
    hf=CreateFile(_T("\\z\\sizes-120k.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,120*1024,&writ,0); CloseHandle(hf);
    delete[] c;
  }
  //
  hz=CreateZip(_T("\\z\\sizes.zip"),0); if (hz==0) msg(_T("* Failed to create sizes.zip"));
  zr=ZipAdd(hz,_T("sizes-71k.out.dat"),_T("\\z\\sizes-71k.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add 71k"));
  zr=ZipAdd(hz,_T("sizes-152_2k.out.dat"),_T("\\z\\sizes-152_2k.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add 152_2k"));
  zr=ZipAdd(hz,_T("sizes-145b.out.dat"),_T("\\z\\sizes-145b.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add 145b"));
  zr=ZipAdd(hz,_T("sizes-120k.out.dat"),_T("\\z\\sizes-120k.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add 120k"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close sizes.zip"));
  if (p_abort) return;
  hz=OpenZip(_T("\\z\\sizes.zip"),0); if (hz==0) msg(_T("* Failed to open sizes.zip"));
  zr=UnzipItem(hz,0,_T("\\z\\sizes-71k.out.dat")); if (zr!=ZR_OK) msg(_T("* Failed to unzip 71k"));
  if (!fsame(_T("\\z\\sizes-71k.dat"),_T("\\z\\sizes-71k.out.dat"))) msg(_T("* Failed to unzip all of 71k"));
  zr=UnzipItem(hz,1,_T("\\z\\sizes-152_2k.out.dat")); if (zr!=ZR_OK || !fsame(_T("\\z\\sizes-152_2k.dat"),_T("\\z\\sizes-152_2k.out.dat"))) msg(_T("* Failed to unzip 152_2k"));
  zr=UnzipItem(hz,2,_T("\\z\\sizes-145b.out.dat")); if (zr!=ZR_OK || !fsame(_T("\\z\\sizes-145b.dat"),_T("\\z\\sizes-145b.out.dat"))) msg(_T("* Failed to unzip 145b"));
  zr=UnzipItem(hz,3,_T("\\z\\sizes-120k.out.dat")); if (zr!=ZR_OK || !fsame(_T("\\z\\sizes-120k.dat"),_T("\\z\\sizes-120k.out.dat"))) msg(_T("* Failed to unzip 120k"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close sizes.zip"));
  if (p_abort) return;

  // fixed bug: if file-size is multiple of internal bufzip, 16k, then last buffer is not saved. Due to unzReadCurrentFile. When buf full and EOF is true at same time, EOF is returned. Could have extra parameter in unzReadCurrentFile.
  // fixed bug: Otherwise unzipping into memory never works! (always fails, returns 0). if (err==Z_STREAM_END) return (iRead==0)?UNSZ_EOF:iRead; -->  if (err==Z_STREAM_END) return (iRead==len)?UNZ_EOF:iRead. 
  msg(_T("unztomem - testing whether ZR_OK is returned upon successfull unzip to memory"));
  { char *c=new char[10240]; for (int i=0; i<10240; i++) c[i]=(char)(rand()%255);
    hf=CreateFile(_T("\\z\\unztomem-10k24.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,10240,&writ,0); CloseHandle(hf);
    hf=CreateFile(_T("\\z\\unztomem-10k00.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,10000,&writ,0); CloseHandle(hf);
    delete[] c;
  }
  hz=CreateZip(_T("\\z\\unztomem.zip"),0); if (hz==0) msg(_T("* Failed to create unztomem.zip"));
  zr=ZipAdd(hz,_T("unztomem-10k24.dat"),_T("\\z\\unztomem-10k24.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add unztomem-10k24.dat"));
  zr=ZipAdd(hz,_T("unztomem-10k00.dat"),_T("\\z\\unztomem-10k00.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add unztomem-10k00.dat"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close unztomem.zip"));
  if (p_abort) return;
  // test unzipping a round multiple into smaller buffer
  { hz=OpenZip(_T("\\z\\unztomem.zip"),0); if (hz==0) msg(_T("* Failed to open unztomem.zip"));
    GetZipItem(hz,0,&ze); char buf[1024]; zr=ZR_MORE; long totsize=0;
    while (zr==ZR_MORE)
    { zr=UnzipItem(hz,0,buf,1024); if (zr!=ZR_MORE && zr!=ZR_OK) msg(_T("* Failed to unzip a buffer of unztomem-10k24"));
      long curbuf = (zr==ZR_MORE) ? 1024 : ze.unc_size-totsize;
      totsize += curbuf;
    }
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close unztomem.zip"));
    if (totsize!=ze.unc_size) msg(_T("* Failed to unzip all of unztomem-10k24 to mem"));
  }
  if (p_abort) return;
  // unzip a non-round multiple into a smaller buffer
  { hz=OpenZip(_T("\\z\\unztomem.zip"),0); if (hz==0) msg(_T("* Failed to open unztomem.zip"));
    GetZipItem(hz,1,&ze); char buf[1024]; zr=ZR_MORE; long totsize=0;
    while (zr==ZR_MORE)
    { zr=UnzipItem(hz,1,buf,1024); if (zr!=ZR_MORE && zr!=ZR_OK) msg(_T("* Failed to unzip a buffer of unztomem-10k00.zip"));
      long curbuf = (zr==ZR_MORE) ? 1024 : ze.unc_size-totsize;
      totsize += curbuf;
    }
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close unztomem.zip"));
    if (totsize!=ze.unc_size) msg(_T("* Failed to unzip all of unztomem-10k00 to mem"));
  }
    if (p_abort) return;
  // unzip a round multiple into a bigger buffer
  { hz=OpenZip(_T("\\z\\unztomem.zip"),0); if (hz==0) msg(_T("* Failed to open unztomem.zip"));
    char buf[16384];
    zr=UnzipItem(hz,0,buf,16384); if (zr!=ZR_OK) msg(_T("* Failed to unzip the buffer of unztomem-10k24"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close unztomem.zip"));
  }
    if (p_abort) return;
  // unzip a nonround multiple into a bigger buffer
  { hz=OpenZip(_T("\\z\\unztomem.zip"),0); if (hz==0) msg(_T("* Failed to open unztomem.zip"));
    char buf[16384];
    zr=UnzipItem(hz,0,buf,16384); if (zr!=ZR_OK) msg(_T("* Failed to unzip the buffer of unztomem-10k00"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close unztomem.zip"));
  }
  if (p_abort) return;
  

  // fixed bug: doesn't work on ARM, change lutime_t mtime = *(lutime_t*)(extra+epos); epos+=4; to lutime_t mtime; memcpy(&mtime,&extra[epos],4); epos+=4;. Also in TUnzip::Get, such atime and ctime
  // fixed bug: (extrapos+epos) has size 4 while sizeof(lutime_t) is currently 8
  msg(_T("time - testing whether timestamps are preserved on CE"));
  { TCHAR *c=_T("Time waits for no man\r\n");
    hf=CreateFile(_T("\\z\\time.txt"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,(DWORD)(_tcslen(c)*sizeof(TCHAR)),&writ,0); CloseHandle(hf);
    hf=CreateFile(_T("\\z\\time.txt"),GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    BY_HANDLE_FILE_INFORMATION fi; GetFileInformationByHandle(hf,&fi); CloseHandle(hf);
    hz=CreateZip(_T("\\z\\time.zip"),0); if (hz==0) msg(_T("* Failed to create hello.zip"));
    zr=ZipAdd(hz,_T("time.txt"),_T("\\z\\time.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add time.txt"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close time.zip"));
    if (p_abort) return;
    hz=OpenZip(_T("\\z\\time.zip"),0); if (hz==0) msg(_T("* Failed to open time.zip"));
    zr=GetZipItem(hz,0,&ze); if (zr!=ZR_OK) msg(_T("* Failed to GetZipItem(time.txt)"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close time.zip"));
    if (!tsame(fi.ftCreationTime,ze.ctime)) msg(_T("* Failed to preserve creation time of hello.txt"));
    if (!tsame(fi.ftLastWriteTime,ze.mtime)) msg(_T("* Failed to preserve modify time of hello.txt"));
    if (!tsame(fi.ftLastAccessTime,ze.atime)) msg(_T("* Failed to preserve access time of hello.txt"));
    if (p_abort) return;
  }

  // fixed bug: Some problem occurs in the library when reading the "extra field" removing the "extra field" the GetZipItem function functions.  Some solution?
  // fixed bug: unzipping a password-zipfile. Seems to miss 12chars from the original file. Line 3530: pfile_in_zip_read_info->rest_read_uncompressed-=uDoEncHead. Is this right?
  msg(_T("extra - testing whether the extra headers cause problems, with passwords"));
  if (GetFileAttributes(_T("\\z\\extra.zip"))!=0xFFFFFFFF)
  { hz=OpenZip(_T("\\z\\extra.zip"),0); if (hz==0) msg(_T("extra.zip is absent; can't do this test"));
    zr=GetZipItem(hz,0,&ze); if (zr!=ZR_OK) msg(_T("* Failed to GetItem(extra.txt)"));
    zr=UnzipItem(hz,0,_T("\\z\\extra.txt")); if (zr!=ZR_OK) msg(_T("* Failed to UnzipItem(extra.txt)"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close extra.zip"));
    if (p_abort) return;
    hf=CreateFile(_T("\\z\\extra.txt"),GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    writ=GetFileSize(hf,0);
    CloseHandle(hf);
    if ((long)writ!=ze.unc_size) msg(_T("* Failed to unzip all of extra.txt"));
    if (p_abort) return;
  }
  { TCHAR *c=_T("Hello there this will have to be a very long file if I want to see what I want to see the quick brown fox jumped over the lazy dog and a merry time was had by all at the party few things are more distressing to a well-regulated mind than to see a boy who ought to know better disporting himself at improper moments\r\n");
    hf=CreateFile(_T("\\z\\extrapp.txt"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,(DWORD)(_tcslen(c)*sizeof(TCHAR)),&writ,0); CloseHandle(hf);
    hz=CreateZip(_T("\\z\\extrapp.zip"),"password"); if (hz==0) msg(_T("* Failed to create extrapp.zip"));
    zr=ZipAdd(hz,_T("extrapp.txt"),_T("\\z\\extrapp.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add extrapp.txt"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close extrapp.zip"));
    if (p_abort) return;
    hz=OpenZip(_T("\\z\\extrapp.zip"),"password"); if (hz==0) msg(_T("* Failed to open extrapp.zip"));
    zr=GetZipItem(hz,0,&ze); if (zr!=ZR_OK) msg(_T("* Failed to GetItem(extrapp.txt)"));
    zr=UnzipItem(hz,0,_T("\\z\\extrapp.out.txt")); if (zr!=ZR_OK) msg(_T("* Failed to unzip extrapp.txt"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close extrapp.zip"));
    if (!fsame(_T("\\z\\extrapp.txt"),_T("\\z\\extrapp.out.txt"))) msg(_T("* Failed to unzip all of extrapp.txt"));
    if (p_abort) return;
  }

  // fixed bug: XP(zip)->CE(unzip) works, and Winzip->CE(unzip) works, but CE(zip)->CE(unzip) doesn't.
  msg(_T("ce2ce - testing whether ce zipping and unzipping work"));
  if (GetFileAttributes(_T("\\z\\ce2ce.jpg"))!=0xFFFFFFFF && GetFileAttributes(_T("\\z\\ce2ce.txt"))!=0xFFFFFFFF)
  { hz=CreateZip(_T("\\z\\ce2ce.zip"),0); if (hz==0) msg(_T("* Failed to create ce2ce.zip"));
    zr=ZipAdd(hz,_T("ce2ce.out.jpg"),_T("\\z\\ce2ce.jpg")); if (zr!=ZR_OK) msg(_T("* Failed to zip ce2ce.jpg"));
    zr=ZipAdd(hz,_T("ce2ce.out.txt"),_T("\\z\\ce2ce.txt")); if (zr!=ZR_OK) msg(_T("* Failed to zip ce2ce.txt"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close ce2ce.zip"));
    if (p_abort) return;
    hz=OpenZip(_T("\\z\\ce2ce.zip"),0); if (hz==0) msg(_T("* Failed to open ce2ce.zip"));
    zr=SetUnzipBaseDir(hz,_T("\\z")); if (zr!=ZR_OK) msg(_T("* Failed to set base dir for ce2ce.zip"));
    zr=GetZipItem(hz,-1,&ze); if (zr!=ZR_OK) msg(_T("* Failed to get index of ce2ce.zip"));
    zr=GetZipItem(hz,0,&ze); if (zr!=ZR_OK) msg(_T("* Failed to GetItem(ce2ce.jpg)"));
    zr=UnzipItem(hz,0,ze.name); if (zr!=ZR_OK) msg(_T("* Failed to unzip ce2ce.out.jpg"));
    zr=GetZipItem(hz,1,&ze); if (zr!=ZR_OK) msg(_T("* Failed to GetItem(ce2ce.txt)"));
    zr=UnzipItem(hz,1,ze.name); if (zr!=ZR_OK) msg(_T("* Failed to unzip ce2ce.out.txt"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close ce2ce.zip"));
    if (!fsame(_T("\\z\\ce2ce.jpg"),_T("\\z\\ce2ce.out.jpg"))) msg(_T("* Failed to unzip all of ce2ce.jpg"));
    if (!fsame(_T("\\z\\ce2ce.txt"),_T("\\z\\ce2ce.out.txt"))) msg(_T("* Failed to unzip all of ce2ce.txt"));
    if (p_abort) return;
  }

  // fixed bug: doesn't work with multiple levels of subfolder
  // fixed bug: EnsureDirectory,  bug: line 3940 of unzip, replace _tcscat(cd,name); with _tcscat(cd,dir); otherwise root directories are never created!
  msg(_T("folders - testing whether unzip to subfolders works"));
  { TCHAR *c=_T("Hello just a dummy file\r\n");
    hf=CreateFile(_T("\\z\\folders.txt"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); WriteFile(hf,c,(DWORD)(_tcslen(c)*sizeof(TCHAR)),&writ,0); CloseHandle(hf);
    hz=CreateZip(_T("\\z\\folders.zip"),0); if (hz==0) msg(_T("* Failed to create folders.zip"));
    zr=ZipAdd(hz,_T("folders.out.txt"),_T("\\z\\folders.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add folders.txt"));
    zr=ZipAdd(hz,_T("sub1\\folders.out.txt"),_T("\\z\\folders.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add sub1\\folders.txt"));
    zr=ZipAdd(hz,_T("sub1\\sub2\\folders.out.txt"),_T("\\z\\folders.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add sub1\\sub2\\folders.txt"));
    zr=ZipAdd(hz,_T("sub1\\sub2\\sub3\\sub4\\folders.out.txt"),_T("\\z\\folders.txt")); if (zr!=ZR_OK) msg(_T("* Failed to add sub1\\sub2\\sub3\\sub4\\folders.txt"));
    zr=ZipAddFolder(hz,_T("sub1\\sub5\\sub6")); if (zr!=ZR_OK) msg(_T("* Failed to add sub1\\sub5\\sub6"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close folders.zip"));
    if (p_abort) return;
    hz=OpenZip(_T("\\z\\folders.zip"),0); if (hz==0) msg(_T("* Failed to open folders.zip"));
    zr=SetUnzipBaseDir(hz,_T("\\z")); if (zr!=ZR_OK) msg(_T("* Failed to set base dir for folders.zip"));
    zr=GetZipItem(hz,-1,&ze); if (zr!=ZR_OK) msg(_T("* Failed to get index of folders.zip"));
    int numitems=ze.index; for (int i=0; i<numitems; i++)
    { zr=GetZipItem(hz,i,&ze); if (zr!=ZR_OK) {wsprintf(m,_T("* Failed to get index %i of folders.zip"),i); msg(m);}
      zr=UnzipItem(hz,i,ze.name); if (zr!=ZR_OK) {wsprintf(m,_T("* Failed to unzip item %i of folders.zip"),i); msg(m);}
      TCHAR absfn[MAX_PATH]; wsprintf(absfn,_T("\\z\\%s"),ze.name);
      if ((ze.attr&FILE_ATTRIBUTE_DIRECTORY)==0 && !fsame(_T("\\z\\folders.txt"),absfn)) {wsprintf(m,_T("* Failed to unzip all of item %i of folders.zip"),i); msg(m);}
    }
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close folders.zip"));
    if (p_abort) return;
  }

  // fixed bug: it skips first few (8/12) bytes when extracting a big file (27760609; 17838044 is okay)
  msg(_T("skip - testing whether it skips bytes when extracting big files... (will take a while)"));
  { DWORD size= (fast) ? 277606 : 27760609;
    char *c=new char[size]; for (DWORD i=0; i<size; i++) c[i]=(char)(rand()%255);
    HANDLE hf = CreateFile(_T("\\z\\skip.dat"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
    DWORD writ; WriteFile(hf,c,size,&writ,0);
    CloseHandle(hf);
    delete[] c;
    if (p_abort) return;
    //
    hz=CreateZip(_T("\\z\\skip.zip"),0); if (hz==0) msg(_T("* Failed to create skip.zip"));
    zr=ZipAdd(hz,_T("skip.dat"),_T("\\z\\skip.dat")); if (zr!=ZR_OK) msg(_T("* Failed to add skip.dat"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close skip.zip"));
    if (p_abort) return;
    hz=OpenZip(_T("\\z\\skip.zip"),0); if (hz==0) msg(_T("* Failed to open skip.zip"));
    zr=UnzipItem(hz,0,_T("\\z\\skip.out.dat")); if (zr!=ZR_OK) msg(_T("* Failed to unzip skip.out.zip"));
    zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close skip.zip"));
    if (!fsame(_T("\\z\\skip.dat"),_T("\\z\\skip.out.dat"))) msg(_T("* Failed to unzip all of skip.zip"));
    if (p_abort) return;
  }

  // fixed bug: had problems with big files
  msg(_T("win - testing with zipping up the whole windows directory... (will take a while)"));
  hz=CreateZip(_T("\\z\\win.big"),0); if (hz==0) msg(_T("* Failed to create win.big"));
  DWORD tot=0, max=fast?1*1024*1024:30*1024*1024; WIN32_FIND_DATA fdat; HANDLE hfind=FindFirstFile(_T("\\windows\\*"),&fdat);
  for (BOOL gotmore=(hfind!=INVALID_HANDLE_VALUE); gotmore; gotmore=FindNextFile(hfind,&fdat))
  { if (fdat.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) continue;
    if (fdat.cFileName[0]=='$') continue;
    TCHAR src[MAX_PATH]; wsprintf(src,_T("\\windows\\%s"),fdat.cFileName);
    TCHAR dst[MAX_PATH]; wsprintf(dst,_T("out_%s"),fdat.cFileName);
    wsprintf(m,_T(".%s (%i%%)"),src,(tot*100/max)); msg(m);
    HANDLE hf=CreateFile(src,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
    if (hf==INVALID_HANDLE_VALUE) continue; tot+=GetFileSize(hf,0);
    CloseHandle(hf);
    zr=ZipAdd(hz,dst,src); if (zr!=ZR_OK) {wsprintf(m,_T("* Failed to add %s to win.big"),src); msg(m);}
    if (tot>max) break;
    if (p_abort) break;
  }
  if (hfind!=INVALID_HANDLE_VALUE) FindClose(hfind);
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close win.big"));
  if (p_abort) return;
  //
  hz=CreateZip(_T("\\z\\big.zip"),0); if (hz==0) msg(_T("* Failed to create big.zip"));
  zr=ZipAdd(hz,_T("win.out.big"),_T("\\z\\win.big")); if (zr!=ZR_OK) msg(_T("* Failed to add win.big to big.zip"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close big.zip"));
  if (p_abort) return;
  hz=OpenZip(_T("\\z\\big.zip"),0); if (hz==0) msg(_T("* Failed to open big.zip"));
  zr=UnzipItem(hz,0,_T("\\z\\win.out.big")); if (zr!=ZR_OK) msg(_T("* Failed to unzip win.big"));
  zr=CloseZip(hz); if (zr!=ZR_OK) msg(_T("* Failed to close big.zip"));
  if (!fsame(_T("\\z\\win.big"),_T("\\z\\win.out.big"))) msg(_T("* Failed to unzip all of win.big"));
  if (p_abort) return;



  if (any_errors) msg(_T("Finished"));
  else msg(_T("Finished. No errors."));


}




// and when compiling for windows-ce we need winmain...
#ifdef UNDER_CE
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPWSTR,int) {main(); return 0;}
#endif

