#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../zip.h"
#include "../../unzip.h"

void CreateFiles();
// will create "\\simple.bmp" and "\\simple.txt" for subsequent zipping


void main()
{ CreateFiles();
  HZIP hz; DWORD writ;

  // EXAMPLE 1 - create a zipfile from existing files
  hz = CreateZip(_T("\\simple1.zip"),0);
  ZipAdd(hz,_T("znsimple.bmp"), _T("\\simple.bmp"));
  ZipAdd(hz,_T("znsimple.txt"), _T("\\simple.txt"));
  CloseZip(hz);
  _tprintf(_T("Created '\\simple1.zip'\n"));


  // EXAMPLE 2 - unzip it with the names suggested in the zip
  hz = OpenZip(_T("\\simple1.zip"),0);
  SetUnzipBaseDir(hz,_T("\\"));
  ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++)
  { GetZipItem(hz,zi,&ze);
    UnzipItem(hz,zi,ze.name);
  }
  CloseZip(hz);
  _tprintf(_T("Unzipped 'znsimple.bmp' and 'znsimple.txt' from 'simple1.zip'\n"));



  // EXAMPLE 3 - create an auto-allocated pagefile-based zip file from various sources
  // the second argument says how much address space to reserve for it. We can
  // afford to be generous: no address-space is allocated unless it's actually needed.
  hz = CreateZip(0,100000,"password");
  // adding a conventional file...
  ZipAdd(hz,_T("znsimple.txt"),  _T("\\simple.txt"));
  // adding something from memory...
  char buf[1000]; for (int zj=0; zj<1000; zj++) buf[zj]=(char)(zj&0x7F);
  ZipAdd(hz,_T("simple.dat"),  buf,1000);
  // adding something from a pipe...
#ifndef UNDER_CE
  HANDLE hread,hwrite; CreatePipe(&hread,&hwrite,NULL,0);
  DWORD WINAPI Ex3ThreadFunc(void *dat);
  HANDLE hthread = CreateThread(0,0,Ex3ThreadFunc,(void*)hwrite,0,0);
  ZipAddHandle(hz,_T("simple3.dat"),  hread,1000);  // the '1000' is optional, but it makes for nicer zip files if sizes are known in advance.
  WaitForSingleObject(hthread,INFINITE);
  CloseHandle(hthread); CloseHandle(hread); // the thread will close hwrite
#endif
  //
  // and now that the zip is created in pagefile, let's do something with it:
  void *zbuf; unsigned long zlen; ZipGetMemory(hz,&zbuf,&zlen);
  HANDLE hfz = CreateFile(_T("\\simple3 - pwd is 'password'.zip"),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
  WriteFile(hfz,zbuf,zlen,&writ,NULL);
  CloseHandle(hfz);
  CloseZip(hz);
  _tprintf(_T("Created 'simple3.zip' via pagefile\n"));


  // EXAMPLE 4 - unzip directly from resource into a file
  // resource RT_RCDATA/#1 happens to be a zip file...
  HINSTANCE hInstance=GetModuleHandle(0);
  HRSRC hrsrc = FindResource(hInstance,MAKEINTRESOURCE(1),RT_RCDATA);
  HANDLE hglob = LoadResource(hInstance,hrsrc);
  void *zipbuf=LockResource(hglob);
  unsigned int ziplen=SizeofResource(hInstance,hrsrc);
  hz = OpenZip(zipbuf, ziplen, 0);  SetUnzipBaseDir(hz,_T("\\"));
  int i; FindZipItem(hz,_T("simple.jpg"),true,&i,&ze);
  //   - unzip to a file -
  UnzipItem(hz,i,ze.name);
  //   - unzip to a membuffer -
  char *ibuf = new char[ze.unc_size];
  UnzipItem(hz,i, ibuf, ze.unc_size);
  delete[] ibuf;
  //   - unzip to a fixed membuff, bit by bit -
  char fbuf[1024]; ZRESULT zr=ZR_MORE; unsigned long totsize=0;
  while (zr==ZR_MORE)
  { zr = UnzipItem(hz,i, fbuf,1024);
    unsigned long bufsize=1024; if (zr==ZR_OK) bufsize=ze.unc_size-totsize;
    // now do something with this chunk which we just unzipped...
    totsize+=bufsize;
  }
  //   - finished -
  CloseZip(hz);
  // note: no need to free resources obtained through Find/Load/LockResource
  _tprintf(_T("Unzipped 'simple.jpg' from resource zipfile\n"));


  
  DeleteFile(_T("\\simple.txt"));
  DeleteFile(_T("\\simple.bmp"));
  _tprintf(_T("Deleted 'simple.txt' and 'simple.bmp'\n"));
}



DWORD WINAPI Ex3ThreadFunc(void *dat)
{ HANDLE hwrite = (HANDLE)dat;
  char buf[1000]={17};
  DWORD writ; WriteFile(hwrite,buf,1000,&writ,NULL);
  CloseHandle(hwrite);
  return 0;
}


void CreateFiles()
{ HANDLE hf;
  // Dummy file: bitmap snapshot of screen
  int w=GetSystemMetrics(SM_CXSCREEN), h=GetSystemMetrics(SM_CYSCREEN); 
  int bw=w/2, bh=h/2; if (bw>320) bw=320; if (bh>240) bh=240; int bw4=(bw+3)&0xFFFFFFFC;
	BITMAPINFO bi; bi.bmiHeader.biBitCount=24; bi.bmiHeader.biClrImportant=0;
	bi.bmiHeader.biClrUsed=0;	bi.bmiHeader.biCompression=BI_RGB; bi.bmiHeader.biHeight=bh;
	bi.bmiHeader.biPlanes=1; bi.bmiHeader.biSize=40; bi.bmiHeader.biSizeImage=bw4*bh*3;
	bi.bmiHeader.biWidth=bw4; bi.bmiHeader.biXPelsPerMeter=3780; bi.bmiHeader.biYPelsPerMeter=3780;
	bi.bmiColors[0].rgbBlue=0; bi.bmiColors[0].rgbGreen=0; bi.bmiColors[0].rgbRed=0; bi.bmiColors[0].rgbReserved=0;
	HDC sdc=GetDC(0); BYTE *bits; HBITMAP hbm=CreateDIBSection(sdc,&bi,DIB_RGB_COLORS,(void**)&bits,NULL,0);
  HDC hdc=CreateCompatibleDC(sdc); HGDIOBJ hold=SelectObject(hdc,hbm); 
  StretchBlt(hdc,0,0,bw,bh,sdc,0,0,w,h,SRCCOPY);
  SelectObject(hdc,hold); DeleteDC(hdc); ReleaseDC(0,sdc);
  //
  hf=CreateFile(_T("\\simple.bmp"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  BITMAPFILEHEADER bfh; ZeroMemory(&bfh,sizeof(bfh));
  bfh.bfType=0x4d42; bfh.bfOffBits=sizeof(BITMAPFILEHEADER)+bi.bmiHeader.biSize;
  bfh.bfSize=bfh.bfOffBits+bi.bmiHeader.biSizeImage; bfh.bfReserved1=0; bfh.bfReserved2=0;
  DWORD writ; WriteFile(hf, &bfh, sizeof(bfh), &writ, NULL);
  WriteFile(hf, &bi.bmiHeader, sizeof(bi.bmiHeader), &writ, NULL);
  WriteFile(hf, bits, bi.bmiHeader.biSizeImage, &writ, NULL);
  CloseHandle(hf);
  DeleteObject(hbm);
  _tprintf(_T("Created '\\simple.bmp'\n"));
  //
  // Dummy file: text
  hf = CreateFile(_T("\\simple.txt"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  const char *src = "Few things are more distressing\r\nto a well regulated mind\r\nthan to see "
                    "a boy who ought to know better\r\ndisporting himself at improper moments.\r\n";
  WriteFile(hf,src,(DWORD)strlen(src),&writ,NULL);
  CloseHandle(hf);
  _tprintf(_T("Created '\\simple.txt'\n"));
}



// and when compiling for windows-ce we need winmain...
#ifdef UNDER_CE
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPWSTR,int) {main(); return 0;}
#endif

