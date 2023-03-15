#include "Contour/ContourTracer.hpp"
#include "Contour/ContourComputer.hpp"

const QVector<QPoint> ContourTracer::Dir = {{ 1, 0}, { 1, 1}, { 0, 1}, {-1, 1},
                                            {-1, 0}, {-1,-1}, { 0,-1}, { 1,-1}};

ContourTracer::ContourTracer(const QVector<bool> &contourImg, int width, int height)
  :width_{width}, height_{height}, contourImg_{contourImg}
{}

QVector<float> ContourTracer::traceFlat()
{
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

  for (int i = 0; i < pcontour.count(); i++) {
    pcontour[i].setX(fcontour[2*i]);
    pcontour[i].setY(fcontour[2*i+1]);
  }

  return pcontour;
}

QVector<float> ContourTracer::traceContour(const QPoint &p)
{
  QVector<float> trace;
  PointDir next = findNextPoint(p, 0);
  trace.append(static_cast<float>(next.p.x()) + 0.5f);
  trace.append(static_cast<float>(next.p.y()) + 0.5f);

  QPoint start = p;
  QPoint end = next.p;

  QPoint prev = p;
  QPoint cur = next.p;
  bool done = (prev == cur);

  while (!done) {
    int dsearch = (next.dir + 6) % 8;
    next = findNextPoint(cur, dsearch);
    prev = cur;
    cur = next.p;
    done = (prev == start && cur == end);
    if (!done) {
      trace.append(static_cast<float>(cur.x()) + 0.5f);
      trace.append(static_cast<float>(cur.y()) + 0.5f);
    }
  }

  return trace;
}

ContourTracer::PointDir 
ContourTracer::findNextPoint(const QPoint &curPoint, int dir) const
{
  for (int i = 0; i < 7; i++) {
    QPoint q = curPoint + Dir[dir];
    if (background(q))
      dir = (dir + 1) % 8;
    else 
      return {q, dir};
  }
  return {curPoint, dir};
}


bool ContourTracer::foreground(const QPoint &p) const
{
  if (imgContains(p)) {
    return contourImg_[index(p)];
  }

  return false;
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

QVector<float> traceContourFlat(const QSize &size, 
  const QVector<bool> &bimg)
{
  return traceContourFlat(bimg, size.width(), size.height());
}

QVector<QVector2D> traceContourPoint(const QVector<bool> &bimg,
  int width, int height)
{
  QVector<bool> contourImg = computeContours({width, height},
    bimg);
  ContourTracer tracer{contourImg, width, height};
  return tracer.tracePoint();
}

QVector<QVector2D> traceContourPoint(const QSize &size,
  const QVector<bool> &bimg)
{
  return traceContourPoint(bimg, size.width(), size.height());
}