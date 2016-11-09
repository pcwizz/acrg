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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "acrg.h"


#define  M_PI (atan(1.0)*4)

void SVGHeader(){
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
}

void SVGFooter(){
	printf("</svg>");
}

double RadToDeg(double x) {
	return x / M_PI * 180;
}

Point PointOnCircle(Circle c, double a){
	Point p;
	p.x = c.x + c.r * cos(a);
	p.y = c.y + c.r * sin(a);
	return p;
}

void DrawCircle(Circle c){
	printf("<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"none\" stroke-width=\"1\" stroke=\"black\"></circle>\n", c.x, c.y, c.r);
}

void DrawLine(Line l){
	printf("<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\"></line>\n", l.p1.x, l.p1.y, l.p2.x, l.p2.y);
}

void DrawChar(Point p, double a, const char *c) {
	char* priv_c = 0;

	if (strcmp(c, "\"") == 0){
		const char* tmp_c = "&quot;";	
		priv_c = (char*)malloc(strlen(tmp_c)+1);
		clean_alloc((void*) priv_c, strlen(tmp_c)+1);
		strcpy(priv_c, tmp_c);
	} else if (strcmp(c, "'") == 0){
		const char* tmp_c = "&apos;";	
		priv_c = (char*)malloc(strlen(tmp_c)+1);
		clean_alloc((void*) priv_c, strlen(tmp_c)+1);
		strcpy(priv_c, tmp_c);
	} else if (strcmp(c, "<") == 0){
		const char* tmp_c = "&lt;";	
		priv_c = (char*)malloc(strlen(tmp_c)+1);
		clean_alloc((void*) priv_c, strlen(tmp_c)+1);
		strcpy(priv_c, tmp_c);
	} else if (strcmp(c, ">") == 0){
		const char* tmp_c = "&gt;";	
		priv_c = (char*)malloc(strlen(tmp_c)+1);
		clean_alloc((void*) priv_c, strlen(tmp_c)+1);
		strcpy(priv_c, tmp_c);
	} else if (strcmp(c, "&") == 0){
		const char* tmp_c = "&amp;";	
		priv_c = (char*)malloc(strlen(tmp_c)+1);
		clean_alloc((void*) priv_c, strlen(tmp_c)+1);
		strcpy(priv_c, tmp_c);
	} else {
		priv_c = (char*)malloc(utf8_codepoint_len(*c)+1);
		clean_alloc((void*)priv_c, utf8_codepoint_len(*c)+1);
		strcpy(priv_c, c);
	}

	//Output upto the character
	printf("<text x=\"%f\" y=\"%f\" text-anchor=\"middle\" transform=\"translate(0)rotate(%f %f %f)\" font-family=\"monospace\" font-size=\"25px\">%s</text>", p.x, p.y, RadToDeg(a + M_PI/2 ), p.x, p.y, priv_c);
	free((void*)priv_c);
}

double CalcStep(int len){
	return (2 * M_PI) / (double)len;
}

void DrawRing(Circle c, UTF8 utf8) {
	double a, step;
	step = CalcStep(utf8.len);
	a = 0;

	for ( int i = 0; i < utf8.len; i++ ){
		Point p = PointOnCircle(c, a);
		DrawChar(p, a, utf8.str[i]);
		a += step;
	}
}

void DrawSeparators(Circle inner, Circle outer, int len) {
	double a, step;
	Line line;
	step = CalcStep(len);
	a = step / 2;

	for ( int i = 0; i < len; i++ ){
		line.p1 = PointOnCircle(outer, a);
		line.p2 = PointOnCircle(inner, a);
		DrawLine(line);
		a += step;
	}
}

void DrawFull(UTF8 utf8, double outerDiamiter, double middleDiamiter, double innerDiamiter){

	if (outerDiamiter == 0 || middleDiamiter == 0 || innerDiamiter == 0){
		exit_and_free_utf8(-1, utf8);//This shouldn't happen!
	}
	
	SVGHeader();
	
	Circle outer;
	outer.x = outerDiamiter / 2;
	outer.y = outer.x;
	outer.r = outer.x;

	DrawCircle(outer);

	Circle middle;
	middle.x = outer.x;
	middle.y = outer.y;
	middle.r = middleDiamiter / 2;

	DrawCircle(middle);

	Circle inner;
	inner.x = outer.x;
	inner.y = outer.y;
	inner.r = innerDiamiter / 2;

	DrawCircle(inner);

	DrawSeparators(inner, outer, utf8.len);

	Circle outer_ring;
	outer_ring.x = outer.x;
	outer_ring.y = outer.y;
	outer_ring.r = middle.r + (outer.r - middle.r)/2 - 25/2;

	DrawRing(outer_ring, utf8);
	
	Circle inner_ring;
	inner_ring.x = outer.x;
	inner_ring.y = outer.y;
	inner_ring.r = inner.r + (middle.r - inner.r)/2 - 25/2;


	DrawRing(inner_ring, utf8);

	SVGFooter();
}

void free_utf8(UTF8 obj){
	//free utf8_input
	for (int i = 0; i < obj.len; i++){
		free((void*)obj.str[i]);
		obj.str[i] = NULL;
	}
	free((void*)obj.str);
	obj.str = NULL;
}

void exit_and_free_utf8(int rtn, UTF8 obj){
	free_utf8(obj);
	exit(rtn);
}

int utf8_codepoint_len(char first){
	if ((first & 0b11110000) == 0b11110000)
		return 4;
	else if ((first & 0b11100000) == 0b11100000)
		return 3;
	else if ((first & 0b11000000) == 0b11000000)
		return 2;
	else
		return 1;
}

void clean_alloc(void* a, size_t nbytes){
	char* str = (char*) a;
	for ( size_t i = 0; i < nbytes; i++)
		str[i] = '\0';
}	
