#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DODEBUG

#ifdef DODEBUG

void Debug( int a, int b );
void Debug( TSTR str, TSTR v, bool nl = true );
void Debug( TSTR str, int v, bool nl = true );
void Debug( TSTR str, unsigned int v, bool nl = true );
void Debug( TSTR str, float v, bool nl = true );
void Debug( TSTR str, IPoint2 v, bool nl = true );
void Debug( TSTR str, IPoint3 v, bool nl = true );
void Debug( TSTR str, Point3 v, bool nl = true );
void Debug( TSTR str, Point2 v, bool nl = true );

#else

#define debug

#endif

#endif
