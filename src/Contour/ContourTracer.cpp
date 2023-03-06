#include "Contour/ContourTracer.hpp"
#include "Contour/ContourComputer.hpp"

const QVector<QPoint> ContourTracer::Dir = {{ 1, 0}, { 1, 1}, { 0, 1}, {-1, 1},
                                            {-1, 0}, {-1,-1}, { 0,-1}, { 1,-1}};

ContourTracer::ContourTracer(const QVector<bool> &contourImg, int width, int height)
  :width_{width}, height_{height}, contourImg_{contourImg}
{}

QVector<float> ContourTracer::traceFlat()
{
  visited_ = QVector<bool>(contourImg_.count(), false);

  QPoint p;
  for (p.ry() = 0; p.y() < height_; p.ry()++) {
    for (p.rx() = 0; p.x() < width_; p.rx()++) {
      if (contourImg_[index(p)])
        return traceContour(p);
    }
  }

  return QVector<float>();
}

QVector<QVector2D> ContourTracer::tracePoint()
{
  QVector<float> fcontour = traceFlat();
  QVector<QVector2D> pcontour(fcontour.count() / 2);

  for (int i = 0; i < fcontour.count(); i+=2) {
    pcontour[i].setX(fcontour[i]);
    pcontour[i].setY(fcontour[i+1]);
  }

  return pcontour;
}

QVector<float> ContourTracer::traceContour(const QPoint &p)
{
  QVector<float> trace;
  visited_[index(p)] = true;

  QPoint cp = p;
  bool stop = false;
  while (!stop) {
    stop = true;
    trace.append(static_cast<float>(cp.x()) + 0.5f);
    trace.append(static_cast<float>(cp.y()) + 0.5f);
    int cpidx = index(cp);
    visited_[cpidx] = true;

    for (const QPoint &o : Dir) {
      QPoint q = cp + o;
      int qidx = index(q);
      if (imgContains(q) && contourImg_[qidx] && !visited_[qidx]) {
        stop = false;
        cp = q;
        break;
      }
    }
  }
  return trace;
}

bool ContourTracer::imgContains(const QPoint &p) const
{
  return 0 <= p.x() && p.x() < width_ && 0 <= p.y() && p.y() < height_;
}

int ContourTracer::index(const QPoint &p) const
{
  return p.y() * width_ + p.x();
}

QPoint ContourTracer::point(int idx) const
{
  return { idx % width_, idx / width_ };
}

QVector<float> traceContourFlat(const QVector<bool> &bimg, 
  int width, int height)
{
  QVector<bool> contourImg = computeContours({width, height},
    bimg);
  ContourTracer tracer{contourImg, width, height};
  return tracer.traceFlat();
}

QVector<QVector2D> traceContourPoint(const QVector<bool> &bimg,
  int width, int height)
{
  QVector<bool> contourImg = computeContours({width, height},
    bimg);
  ContourTracer tracer{contourImg, width, height};
  return tracer.tracePoint();
}