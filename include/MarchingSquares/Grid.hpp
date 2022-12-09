#pragma once 

#include <QVector2D>
#include <QVector>
#include <array>

#include <functional>

class GridCell
{
public:
  GridCell();
  GridCell(const std::array<QVector2D, 4> &verts, const std::array<float, 4> &vals);

  const std::array<QVector2D, 4> &verts() const { return verts_; }
  std::array<QVector2D, 4> &verts() { return verts_; }
  const QVector2D &vertex(int index) const { return verts_[index]; }
  QVector2D &vertex(int index) { return verts_[index]; }

  const std::array<float, 4> &vals() const { return vals_; }
  std::array<float, 4> &vals() { return vals_; }
  float val(int index) const { return vals_[index]; }
  float &val(int index) { return vals_[index]; } 

private:
  std::array<QVector2D, 4> verts_;
  std::array<float, 4> vals_;
};

class Grid
{
public:
  using ImplicitFunction = std::function<float (const QVector2D &)>;

  Grid(ImplicitFunction f, const QVector2D &tl, const QVector2D &br, float cellSize);

  int nx() const { return nx_; }
  int ny() const { return ny_; }
  
  int ncells() const { return nx_ * ny_; }

  const QVector<GridCell> &cells() const { return cells_; }
  const GridCell &cell(int index) const { return cells_[index]; }
  const GridCell &cell(int ix, int iy) const; 
  
  void updateImplicitFunction(ImplicitFunction f) { f_ = f; updateValues(); }
  void updateValues();

private:
  QVector<GridCell> cells_;
  ImplicitFunction f_;
  int nx_;
  int ny_;
};