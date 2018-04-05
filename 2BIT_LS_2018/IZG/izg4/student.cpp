/******************************************************************************
 * Laborator 04 Krivky - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz
 *
 */

#include "student.h"
#include "globals.h"
#include "main.h"

#include <time.h>
#include <math.h>

//Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2d_vecGetPtr((pVec), (i)))

/* Secte dva body Point2d a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	addPoint2d(const Point2d *a, const Point2d *b, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = a->x+b->x;
	result->y = a->y+b->y;
}

/* Vynasobi bod Point2d skalarni hodnotou typu double a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	mullPoint2d(double val, const Point2d *p, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = p->x*val;
	result->y = p->y*val;
}

/**
 * Inicializace ridicich bodu horni trajektorie ve vykreslovacim okne.
 * Pocatek souradnicove soustavy je v levem hornim rohu okna. Xova souradnice
 * roste smerem doprava, Y smerem dolu.
 * @param points vystup, kam se pridavaji ridici body
 * @param offset_x posun vsech ridicich bodu v horizontalnim smeru (aby se mohli souradnice zadavat s pocatkem [0,0])
 * @param offset_y posun vsech ridicich bodu ve vertikalnim smeru
 */
void initControlPointsUp(S_Vector **points, int offset_x, int offset_y) {
  *points = vecCreateEmpty(sizeof(Point2d));
  Point2d p;
  p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
  p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);
  
  p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 210; p.y = -180; point2d_vecPushBack(*points, p);
  p.x = 350; p.y = -180; point2d_vecPushBack(*points, p);
  
  p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
  p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);
  
  p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 470; p.y = -70;  point2d_vecPushBack(*points, p);
  p.x = 525; p.y = -70;  point2d_vecPushBack(*points, p);
  
  p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
  p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);
  
  p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

  Point2d offset = {offset_x, offset_y, 1.0};
  for(int i = 0; i < (*points)->size; i++) {
    addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
  }
}

void initControlPointsDown(S_Vector **points, int offset_x, int offset_y) {
  /* == TODO ==
   * Uloha c.2
   * Nasledujuci volanni funkce initControlPointsUp(.) zmazte a nahradte vlastnim kodem,
   * ktery inicializuje ridici body tak, aby byla trajektorie spojita (C1). Muzete skopirovat
   * kod funkce initControlPointsUp(.) a upravit primo souradnice bodu v kodu.
   */
  *points = vecCreateEmpty(sizeof(Point2d));
  Point2d p;
  p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);//p0

  p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);//p1
  p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);//p2
  
  p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);//p3

  p.x = 228.8; p.y = 180; point2d_vecPushBack(*points, p);//p4
  // 180*40/250 = 28.8 (vzdalenost od P3)
  // 28.8 + 200
  p.x = 360-28.8; p.y = 180; point2d_vecPushBack(*points, p);//p5
  
  p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);//p6
  //(331.2, 180)
  // vektor (p6-p5)= (360-331.2, 0-180) = (28.8, -180) => (-28.8,180)
  // (x, -120) => x = (-120*-28.8 / 180)
  p.x = -120*-28.8 / 180 + 360; p.y = -120;  point2d_vecPushBack(*points, p);//p7
  p.x = 460 - -120*-28.8 / 180 ; p.y = -120;  point2d_vecPushBack(*points, p);//p8
  
  p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);//p9

  // (440.8, -120)
  // vektor (p9-p8) = (460-440.8,0+120) = (20,120)
  // (x, 70) => x = ( 70*20/120 ) = 11,6 ... +460	
  p.x = 460 + 11.6; p.y = 70;  point2d_vecPushBack(*points, p);//p10
  p.x = 535 - 11.6; p.y = 70;  point2d_vecPushBack(*points, p);//p11
  
  p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);//p12
  // (523.4, 70)
  // vektor (p12 - p11) = (11.6, -70) => (-11,6,70)
  // (x, -40) => x = ( -40*-11.6/70 ) = 6.63 ... + 535
  p.x = 535 + 6.63; p.y = -40;  point2d_vecPushBack(*points, p);
  p.x = 585 - 6.63; p.y = -40;  point2d_vecPushBack(*points, p);
  
  p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

  Point2d offset = {offset_x, offset_y, 1.0};
  for(int i = 0; i < (*points)->size; i++) {
    addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
  }
}

/**
 * Implementace vypoctu Bezierove kubiky.
 * @param P0,P1,P2,P3 ridici body kubiky
 * @param quality pocet bodu na krivke, ktere chceme vypocitat
 * @param trajectory_points vystupni vektor bodu kubiky (nemazat, jen pridavat body)
 */
void bezierCubic(const Point2d *P0, const Point2d *P1, const Point2d *P2, const Point2d *P3, 
  const int quality, S_Vector *trajectory_points) {
	
  
  /* == TODO ==
   * Soucast Ulohy c.1:
   * Sem pridejte kod vypoctu Bezierove kubiky. Body krivky pridavejte do trajectory_points.
   */
	for ( double t = 0.0; t < 1.0; t += 1.0/quality ){
	Point2d p = { 0.0,0.0,1.0 };
	p.x = P0->x * pow( 1-t, 3 ) + P1->x * 3 * t * pow( 1-t, 2 ) + P2->x * 3*t*t*(1-t)+P3->x*pow(t,3);
	p.y = P0->y * pow( 1-t, 3 ) + P1->y * 3 * t * pow( 1-t, 2 ) + P2->y * 3*t*t*(1-t)+P3->y*pow(t,3);
	point2d_vecPushBack(trajectory_points, p);
	}

}

/* 
 * Implementace vypoctu trajektorie, ktera se sklada z Bezierovych kubik.
 * @param quality pocet bodu krivky, ktere mame urcit
 * @param control_points ridici body krivky 
 * @param trajectory_points vystupni body zakrivene trajektorie 
 */
void	bezierCubicsTrajectory(int quality, const S_Vector *control_points, S_Vector *trajectory_points) {
  // Toto musi byt na zacatku funkce, nemazat.
  point2d_vecClean(trajectory_points);


  /* == TODO ==
   * Uloha c.1
   * Ziskejte postupne 4 ridici body a pro kazdou ctverici vypocitejte body Bezierovy kubiky.
   * */

   for( int i = 0;i < vecSize( control_points ) - 1; i += 3){
      Point2d *P0 = point2d_vecGetPtr( control_points, i );
       Point2d *P1 = point2d_vecGetPtr( control_points, i +1 );
       Point2d *P2 = point2d_vecGetPtr( control_points, i +2);
       Point2d *P3 = point2d_vecGetPtr( control_points, i +3);
       bezierCubic(P0, P1, P2, P3, quality, trajectory_points);
     }
   /* Nasledujici volani funkce getLinePoints(.) zmazte - je to jen ilustrace hranate trajektorie.
   */
  //getLinePoints(control_points, trajectory_points);
}
