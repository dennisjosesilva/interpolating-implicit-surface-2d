#include "MarchingSquares/Grid.hpp"
#include <QDebug>

// =============================================================
// GRID CELL 
// =============================================================
GridCell::GridCell()
{}

GridCell::GridCell(const std::array<QVector2D, 4> &verts, 
  const std::array<float, 4> &vals)
  : verts_{verts}, vals_{vals}
{ }

// ===============================================================
// GRID 
// ===============================================================
Grid::Grid(ImplicitFunction f, const QVector2D &tl, 
  const QVector2D &br, float cellSize)
  : f_{f},
    nx_{static_cast<int>((br.x() - tl.x()) / cellSize)},
    ny_{static_cast<int>((tl.y() - br.y()) / cellSize)}
{
  // offsets from the top left vertex
  std::array<QVector2D, 4> offsets = {
    QVector2D{0.0f, -cellSize},                       // bottom left
    QVector2D{cellSize, -cellSize},                   // bottom right
    QVector2D{cellSize, 0.0},                         // top right
    QVector2D{0.0f, 0.0f}                             // top left
  };

  cells_.reserve(ncells());
  for (int y=0; y < ny_; y++) {
    for (int x=0; x < nx_; x++) {
      QVector2D topLeftVertex{
        tl.x() + x*cellSize,
        tl.y() - y*cellSize};
      
      
      GridCell c;
      for (int i=0; i < c.vals().size(); ++i) {
        c.vertex(i) = topLeftVertex + offsets[i];
        c.val(i) = f_(c.vertex(i));
      }
      cells_.append(c);
    }
  }
}

void Grid::updateValues()
{
  for (GridCell &c : cells_) {
    for (int i =0 ; i < c.vals().size(); ++i)
      c.val(i) = f_(c.vertex(i));
  }
}