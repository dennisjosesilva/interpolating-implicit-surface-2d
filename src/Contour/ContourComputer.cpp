#include "Contour/ContourComputer.hpp"


// =============================================================================================
// ContourComputer
// =============================================================================================
const QVector<QPoint> ContourComputer::Adj = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

ContourComputer::ContourComputer(const QSize &size, const QVector<bool> &bimg)
  : size_{size}, bimg_{bimg}
{}

QVector<bool> ContourComputer::compute() const
{
  QPoint p;
  QVector<bool> contour(size_.width() * size_.height(), false);

  for (p.ry() = 0; p.y() < size_.height(); p.ry()++) {
    for (p.rx() = 0; p.x() < size_.width(); p.rx()++) {
      if (boundary(p)) 
        contour[index(p)] = true;     
    }
  }

  return contour;
}

QPoint ContourComputer::point(unsigned int pidx) const 
{
  unsigned int w = static_cast<unsigned int>(size_.width());
  return QPoint{ pidx % w, pidx / w };
}

unsigned int ContourComputer::index(const QPoint &p) const 
{
  int w = size_.width();
  return static_cast<unsigned int>(w * p.y() + p.x());
}

bool ContourComputer::imgContains(const QPoint &p) const 
{
  return 0 <= p.x() && p.x() < size_.width() 
    && 0 <= p.y() && p.y() < size_.height();     
}

bool ContourComputer::boundary(const QPoint &p) const 
{
  if (bimg_[index(p)])  {
    for (const QPoint &o : Adj) {
      QPoint n = p + o;
      if (!imgContains(n) || !bimg_[index(n)])
        return true;
    }
  }

  return false;    
}

// ===============================================================================================
// FUNCTIONS 
// ===============================================================================================
QVector<bool> computeContours(const QSize &size, const QVector<bool> &bimg)
{
  return ContourComputer{size, bimg}();
}

QVector<QVector2D> extractContourPoints(const QSize &size, const QVector<bool> &bimg)
{
  QVector<QVector2D> contours;
  QVector<bool> c = ContourComputer(size, bimg)();
  unsigned int w = static_cast<unsigned int>(size.width());

  for (unsigned int pidx = 0; pidx < c.size();  pidx++) {
    if (c[pidx])
      contours.append(QVector2D{ pidx % w + 0.5f, pidx / w + 0.5f });
  }

  return contours;
}