/*
    Copyright (c) 2011, Philipp Krähenbühl
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Stanford University nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Philipp Krähenbühl ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Philipp Krähenbühl BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "util.h"
#include <cstdio>
#include <cstring>

void writePGM ( const char* filename, int W, int H, const char* data )
{
	FILE* fp = fopen ( filename, "wb" );
	if ( !fp )
	{
		printf ( "Failed to open file '%s'!\n", filename );
		return;
	}
	fprintf ( fp, "P5\n%d %d\n%d\n", W, H, 255 );
	fwrite ( data, 1, W*H, fp );
	fclose ( fp );
}
unsigned char* readPPM ( const char* filename, int& W, int& H )
{
	FILE* fp = fopen ( filename, "rb" );
	if ( !fp )
	{
		printf ( "Failed to open file '%s'!\n", filename );
	}
	char hdr[256]={};
	size_t l=0;
	// Read the header
	char p,n;
	int D;
	while ( sscanf ( hdr, "%c%c %d %d %d", &p, &n, &W, &H, &D ) < 5 )
	{
		fgets ( hdr+l, 256-l, fp );
		char * comment = strchr ( hdr, 'p' );
		if ( comment ) l = hdr - comment;
		else l = strlen ( hdr );
		if ( l>=255 )
		{
			W=H=0;
			fclose ( fp );
			return NULL;
		}
	}
	if ( p != 'P' )
	{
		W=H=0;
		fclose ( fp );
		return NULL;
	}
	unsigned char * r = new unsigned char[W*H*3];
	if ( n=='6' )
		fread ( r, 1, W*H*3, fp );
	else if ( n=='3' )
	{
		int c;
		for ( int i=0; i<W*H*3; i++ )
		{
			fscanf ( fp, "%d", &c );
			r[i] = 255*c / D;
		}
	}
	else
	{
		W=H=0;
		fclose ( fp );
		return NULL;
	}
	fclose ( fp );
	return r;
}
void writePPM ( const char* filename, int W, int H, unsigned char* data )
{
	FILE* fp = fopen ( filename, "wb" );
	if ( !fp )
	{
		printf ( "Failed to open file '%s'!\n", filename );
	}
	fprintf ( fp, "P6\n%d %d\n%d\n", W, H, 255 );
	fwrite ( data, 1, W*H*3, fp );
	fclose ( fp );
}

void  get_color(int c, int& r, int& g, int& b)
{
    r = 0;
    g = 0;
    b = 0;
    switch(c)
    {
        case 0:
            r = 128;
            break;
        case 1:
            g = 128;
            break;
        case 2:
            r = 128; g=128;
            break;
        case 3:
            b = 128;
            break;
        case 4:
            g = 128; b = 128;
            break;
        case 5:
            r = 128; b = 128; g = 128;
            break;
        case 6:
            r = 192;
            break;
        case 7:
            r = 64; g = 128;
            break;
        case 8:
            r = 192; g = 128;
            break;
        case 9:
            r = 64; b = 128;
            break;
        case 10:
            r = 192; b = 128;
            break;
        case 11:
            r = 64; g = 128; b = 128;
            break;
        case 12:
            r = 192; g = 128; b = 128;
            break;
        case 13:
            g = 64;
            break;
        case 14:
            r = 128; g = 64;
            break;
        case 15:
            g = 192;
            break;
        case 16:
            r = 128; g = 64; b = 128;
            break;
        case 17:
            g = 192; b = 128;
            break;
        case 18:
            r = 128; g = 192; b = 128;
            break;

        case 19:
            r = 64; g = 64;
            break;
        case 20:
            r = 192; g = 64;
            break;
    }
}


