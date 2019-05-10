#include "iconv.h"
#include "SDL/SDL.h"
#include "fonts.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
static void quit(int rc);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
 
void DrawOnePoint(SDL_Surface *screen,int x,int y);
void DrawChineseCharacter(SDL_Surface *screen,int ShowX,int ShowY,unsigned char InCode0,unsigned char InCode1);

int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
        char *outbuf, size_t outlen) {
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset, from_charset);
    if (cd == 0)
        return -1;
    memset(outbuf, 0, outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
        return -1;
    iconv_close(cd);
    *pout = '\0';

    return 0;
}
int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert("utf-8", "gbk", inbuf, inlen, outbuf, outlen);
}

int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert("gbk", "utf-8", inbuf, inlen, outbuf, outlen);
}
DrawChineseText(SDL_Surface *screen,int x, int y,unsigned char* s) {
   fprintf(stderr,"DrawChineseTExt for: %s\n", s);
   unsigned char buf[1024];
   int origin_x = x;
   u2g(s, strlen(s), buf, sizeof(buf));
   printf("string length:%ld\n",strlen(buf)); 
   for(int i =0 ; i < strlen(buf);) {
     if(buf[i] == '\n') {
    	x = origin_x;
        y +=16; 
        i +=1;
     }
     else if(buf[i] < 0x7F) {
     DrawAscIICharacter(screen,x, y, buf[i]);
     i +=1;
     x +=8;

     }
     else {
     DrawChineseCharacter(screen,x, y, buf[i], buf[i+1]);
     i += 2;
     x+=12;
     }
   }
} 
FILE *HZK;
int main(int argc, char *argv[])
{
	SDL_Surface *screen;
    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
    {
        fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
 
    /*
    * Initialize the display in a 240x320 8-bit palettized mode,
    * requesting a software surface
    */
    screen = SDL_SetVideoMode(320, 240, 8, SDL_SWSURFACE);
    if ( screen == NULL ) 
    {
        fprintf(stderr, "Couldn't set 320x240x8 video mode: %s\n",SDL_GetError());
        exit(1);
    }
	
     if((HZK=fopen("./hzk1216/HZK12","rb"))==NULL)
   {
           printf("Can't Open hzk16\n");
           exit(0);
   }
    unsigned char *s = "（hellow world!怎么输出2个字节,\n这是一个漂亮的繁体字\n点阵字体也叫位图字体，其中每个字形都\n以一组二维像素信息表示。我这种文字显示方式\n于较早前的电脑系统（例如未有图形接口时的 \nDOS 操作系统）被普遍采用。由于\n位图的缘故";
    unsigned char buf[10];
    u2g(s, strlen(s), buf, sizeof(buf));
    
    DrawChineseText(screen,30, 50, s);
    SDL_Delay(10*1000);
    SDL_Quit();
    fclose(HZK);
	return 0;
}
 
 
/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}
 
/*
* Return the pixel value at (x, y)
* NOTE: The surface must be locked before calling this!
*/
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) 
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0; /* shouldn't happen, but avoids warnings */
    }
}
 
/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
 
    switch(bpp) {
    case 1:
        *p = pixel;
        break;
 
    case 2:
        *(Uint16 *)p = pixel;
        break;
 
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
 
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
void DrawAscIICharacter(SDL_Surface *screen,int ShowX,int ShowY,unsigned char c) {
  int x=ShowX;
   int y=ShowY; // 显示位置设置
  fprintf(stderr,"DrawAscIIChar for:%d, 0x%2x,%c\n",c, c ,c);
  int_least16_t ret, pos;
  pos = (c-FONT_START)*(8*FONT_HEIGHT/8);
  fprintf(stderr,"DrawAscIIChar poS: %d\n", pos);
  uint8_t* mat = &font_PGM[pos];  
  int i,j,k;
  for(j=0;j<12;j++)
   {
            for(k=0;k<8;k++)
                        {
                                if(((mat[j]>>(7-k))&0x1)!=0)
                                {
                                        DrawOnePoint(screen,x+k,y+j);
                                }
                        }
   }
   x+=FONT_WIDTH;

}
void DrawChineseCharacter(SDL_Surface *screen,int ShowX,int ShowY,unsigned char InCode0,unsigned char InCode1)
{
   int x=ShowX;
   int y=ShowY; // 显示位置设置
   int i,j,k;   
   unsigned char incode[3]={0};    // 要读出的汉字
   incode[0]=InCode0;
   incode[1]=InCode1; // GBK内码
   unsigned char licode[3]={0};
   licode[0] = InCode1;
   licode[1] = InCode0; 
   unsigned char buf[10];
   g2u(incode, strlen(licode), buf, sizeof(buf));

   fprintf(stderr,"DrawCNGBKChar: 0x%2x%2x,%s\n", incode[0],incode[1],buf);
   unsigned char qh,wh;   
   unsigned long offset;   
         
   char mat[32]; 
 
   // 占两个字节,取其区位号   
   qh=incode[0]-0xa0;    
   wh=incode[1]-0xa0;                
   offset=(94*(qh-1)+(wh-1))*24;  
   
   fseek(HZK,offset,SEEK_SET);   
   fread(mat,24,1,HZK);
   for(j=0;j<12;j++)
   {
        for(i=0;i<2;i++)
		{
            for(k=0;k<8;k++)
			{
				if(((mat[j*2+i]>>(7-k))&0x1)!=0)
				{
					DrawOnePoint(screen,x+6*i+k,y+j);
				}
			}
		}
   }
}
 
void DrawOnePoint(SDL_Surface *screen,int x,int y)
{
    Uint32 yellow;
    /* Map the color yellow to this display (R=0xff, G=ff, B=0x00)
       Note:  If the display is palettized, you must set the palette first.
    */
    yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
    /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }
    putpixel(screen, x, y, yellow);
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    /* Update just the part of the display that we've changed */
    SDL_UpdateRect(screen, x, y, 1, 1);
    return;
}
