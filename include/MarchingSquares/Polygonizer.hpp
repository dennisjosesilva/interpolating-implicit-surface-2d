#pragma once 

#include "MarchingSquares/Grid.hpp"

struct Polygon
{
  QVector<QVector2D> vertPos;
  QVector<QVector2D> vertNormal;
};

class Polygonizer
{
public:
  using ImplicitFunction = Grid::ImplicitFunction;
  using GradImplicitFunction = 
    std::function<QVector2D(const QVector2D &)>;
  
  Polygonizer(ImplicitFunction f, const QVector2D &tl,
    const QVector2D &br, float cellSize, float isoval=0.0f);

  Polygonizer(ImplicitFunction f, const QVector2D &tl,
    const QVector2D &br, float cellSize, GradImplicitFunction fgrad,
    float isoval = 0.0f);

  void setImpllicitFunction(ImplicitFunction f) { f_ = f; grid_.updateImplicitFunction(f); }
  void setGradImplicitFunction(GradImplicitFunction fgrad) { fgrad_ = fgrad; }
  void setIsoVal(float isoval) { isoval_ = isoval; }

  Polygon polygonize();

private:
  void polygonize(const GridCell &cell, Polygon &polygon);

  QVector2D interp(const QVector2D &p1, const QVector2D &p2, 
    float pval1, float pval2) const;

private:
  Grid grid_;
  float isoval_;
  ImplicitFunction f_;
  GradImplicitFunction fgrad_;
  std::array<int, 16> edgeTable_;
  std::array<std::array<int, 5>, 16> lineTable_;
};