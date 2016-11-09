/*
 	ACRG
	Generates cipher decoder rings for arbitrary alphabets in SVG.

    Copyright (C) 2016 Morgan Hill <morgan@pcwizzltd.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
typedef struct circle {
	double x, y, r;// Values in virtual pixels possition from centre of circle
} Circle;

typedef struct point {
	double x, y;
} Point;

typedef struct line {
	Point p1, p2;
} Line;

typedef struct utf8{
	char** str;
	size_t len;
} UTF8;

double RadToDeg (double x);// Converts from radians to degrees
void DrawCircle(Circle c);// Marshalls the circle into an svg circle and outputs to stdout
void DrawLine(Line l);// Marshalls a solid black stroke line 1 virtual pixel wide into a svg line and outputs to stdout
void DrawSeparators(Circle inner, Circle outer, int len);
void SVGHeader();//Opens svg tag
void SVGFooter();//Closes svg tag
void DrawChar(Point p, double a, const char *c);// Marshalls a char (or string due to suporting unicode multibyte chars) into a SVG text element and outputs to stdout
void DrawRing(Circle c, UTF8 utf8);//Marshalls a ring of chars folling the path of the given circle
void DrawFull(UTF8 utf8, double outerDiamiter, double middleDiamiter, double innerDiamiter);//does the lot
double CalcStep(int len);
Point PointOnCircle(Circle c, double a);
void free_utf8(UTF8 obj);
void exit_and_free_utf8(int rtn, UTF8 obj);
int utf8_codepoint_len(char first);//returns the length of the charactor in bytes givin the first byte
void clean_alloc(void* a, size_t nbytes);
