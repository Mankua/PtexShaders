#include "max.h"
#include "debug.h"

#define FNAME "c:\\Debug\\PtexMax.txt"

#ifdef DODEBUG

void Debug(int a, int b)
{
	FILE *file = fopen( FNAME, "w" );
	fprintf(file, "\n");
	fclose(file);
}

void Debug( TSTR str )
{
	FILE *file = fopen( FNAME, "a+" );
	fprintf(file, "%s",str);
	fclose(file);
}

void Debug( TSTR str, TSTR v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s%s", str, v );
	fclose( file );
}

void DebugTime( char * text )
{
	SYSTEMTIME time;
	GetSystemTime( &time );

	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s : %02d:%02d:%02d:%04d\n", text, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds );
	fclose( file );
}

void Debug( TSTR str, int v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s%d", str, v );
	if ( nl ) fprintf( file, "\n" );
	fclose( file );
}

void Debug( TSTR str, unsigned int v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s%d", str, v );
	if ( nl ) fprintf( file, "\n" );
	fclose( file );
}

void Debug( TSTR str, float v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s%f", str, v );
	if ( nl ) fprintf( file, "\n" );
	fclose( file );
}

void Debug( TSTR str, IPoint2 v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s[%d,%d]", str, v.x, v.y );
	if ( nl ) fprintf( file, "\n" );
	fclose(file);
} 

void Debug( TSTR str, IPoint3 v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s[%d,%d,%d]", str, v.x, v.y, v.z );
	if ( nl ) fprintf( file, "\n" );
	fclose(file);
} 

void Debug( TSTR str, Point2 v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s[%f,%f]", str, v.x, v.y );
	if ( nl ) fprintf( file, "\n" );
	fclose(file);
} 

void Debug( TSTR str, Point3 v, bool nl )
{
	FILE * file = fopen( FNAME, "a+" );
	fprintf( file, "%s[%f,%f,%f]", str, v.x, v.y, v.z );
	if ( nl ) fprintf( file, "\n" );
	fclose(file);
} 

#endif
