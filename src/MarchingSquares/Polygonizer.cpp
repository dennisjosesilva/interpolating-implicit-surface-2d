#include "MarchingSquares/Polygonizer.hpp"

#include <QDebug>
#include <QtMath>

Polygonizer::Polygonizer(ImplicitFunction f, const QVector2D &tl, 
  const QVector2D &br, float cellSize, float isovalue)
  : Polygonizer{f, tl, br, cellSize, nullptr, isovalue}
{}

Polygonizer::Polygonizer(ImplicitFunction f, const QVector2D &tl,
  const QVector2D &br, float cellSize, GradImplicitFunction fgrad, 
  float isoval)
  : grid_{f, tl, br, cellSize}, f_{f}, fgrad_{fgrad}, isoval_{isoval}
{
  edgeTable_ = {0, 9, 3, 10, 6, 15, 5, 12, 12, 5, 15, 6, 10, 3, 9, 0};
  lineTable_ = {{
    {-1, -1, -1, -1, -1},
    { 0,  3, -1, -1, -1},
    { 0,  1, -1, -1, -1},
    { 1,  3, -1, -1, -1},
    { 1,  2, -1, -1, -1},
    { 0,  3,  1,  2, -1},
    { 0,  2, -1, -1, -1},
    { 2,  3, -1, -1, -1},
    { 2,  3, -1, -1, -1},
    { 0,  2, -1, -1, -1},
    { 0,  3, -1, -1, -1},
    { 1,  2, -1, -1, -1},
    { 1,  3, -1, -1, -1},
    { 0,  1, -1, -1, -1},
    { 0,  3, -1, -1, -1},
    {-1, -1, -1, -1, -1}
  }};
}

Polygon Polygonizer::polygonize()
{
  Polygon polygon;
  for (const GridCell &c : grid_.cells()) {
    polygonize(c, polygon);
  }

  return polygon;
}


void Polygonizer::polygonize(const GridCell &cell, Polygon &polygon)
{
  std::array<QVector2D, 4> vertList;

  int squareIndex = 0;
  if (cell.val(0) < isoval_) squareIndex |= 1;
  if (cell.val(1) < isoval_) squareIndex |= 2;
  if (cell.val(2) < isoval_) squareIndex |= 4;
  if (cell.val(3) < isoval_) squareIndex |= 8;

  if (edgeTable_[squareIndex] == 0)
    return;

  if (edgeTable_[squareIndex] & 1)  // e0
    vertList[0] = interp(cell.vertex(0), cell.vertex(1), cell.val(0), cell.val(1));
  if (edgeTable_[squareIndex] & 2)  // e1
    vertList[1] = interp(cell.vertex(1), cell.vertex(2), cell.val(1), cell.val(2));
  if (edgeTable_[squareIndex] & 4)  // e2
    vertList[2] = interp(cell.vertex(2), cell.vertex(3), cell.val(2), cell.val(3));
  if (edgeTable_[squareIndex] & 8)  // e4
    vertList[3] = interp(cell.vertex(3), cell.vertex(0), cell.val(3), cell.val(0));
  
  for (int i = 0; lineTable_[squareIndex][i] != -1; i+=2) {
    polygon.vertPos.append(vertList[lineTable_[squareIndex][i  ]]);
    polygon.vertPos.append(vertList[lineTable_[squareIndex][i+1]]);

    if (fgrad_) {
      int n = polygon.vertPos.count();
      polygon.vertNormal.append(fgrad_(polygon.vertPos[n-2]));
      polygon.vertNormal.append(fgrad_(polygon.vertPos[n-1]));
    }
    else {
      int n = polygon.vertPos.count();
      QVector2D normal;
      const QVector2D &v = polygon.vertPos[n-2];
      
      normal.setX(f_(QVector2D{v.x()-0.01f, v.y()}) - f_(QVector2D{v.x()+0.01f, v.y()}));
      normal.setY(f_(QVector2D{v.x(), v.y()-0.01f}) - f_(QVector2D{v.x(), v.y()+0.01f}));

      polygon.vertNormal.append(normal.normalized());
    }
  }
}

// TODO: Implement it here!
QVector2D Polygonizer::interp(const QVector2D &p1, const QVector2D &p2, 
  float pval1, float pval2) const
{
  return p1 + (isoval_ - pval1) * (p2 - p1) / (pval2 - pval1);
}