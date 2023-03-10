#include "Contour/Simplification/simplifier.hpp"
#include "Contour/ContourTracer.hpp"
#include "Contour/Simplification/psimpl.h"

QVector<float> extractSimplifiedContourFlat(const QVector<bool> &bimg, 
  int width, int height, int npoints)
{
  QVector<float> contour = traceContourFlat(bimg, width, height);
  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(scontour));
  return scontour;
}

QVector<float> extractSimplifiedContourFlat(const QSize &size,
  const QVector<bool> &bimg, int npoints)
{
  QVector<float> contour = traceContourFlat(size, bimg);
  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(scontour));
  return scontour;
}

QVector<QVector2D> extractSimplifiedContourPoint(
  const QVector<bool> &bimg, int width, int height, int npoints)
{
  QVector<float> contour = traceContourFlat(bimg, width, height);
  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < s.count(); i+=2) {
    scontour[i].setX(s[i]);
    scontour[i].setY(s[i+1]);
  }

  return scontour;
}

QVector<QVector2D> extractSimplifiedContourPoint(const QSize &size,
  const QVector<bool> &bimg, int npoints)
{
  QVector<float> contour = traceContourFlat(size, bimg);
  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < scontour.count(); i++) {
    scontour[i].setX(s[2*i]);
    scontour[i].setY(s[2*i+1]);
  }

  return scontour;
}

QVector<float> extractSimplifiedContourFlatPercentage(
  const QVector<bool> &bimg, int width, int height, float perc)
{
  QVector<float> contour = traceContourFlat(bimg, width, height);
  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    perc * contour.count(), std::back_inserter(scontour));
  return scontour;    
}

QVector<float> extractSimplifiedContourFlatPercentage(
  const QSize &size,const QVector<bool> &bimg, float perc)
{
  QVector<float> contour = traceContourFlat(size, bimg);
  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    perc * contour.count(), std::back_inserter(scontour));
  return scontour;
}

QVector<QVector2D> extractSimplifiedContourPointPercentage(
  const QVector<bool> &bimg, int width, int height, float perc)
{
  QVector<float> contour = traceContourFlat(bimg, width, height);
  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    perc * contour.count(), std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < s.count(); i+=2) {
    scontour[i].setX(s[i]);
    scontour[i].setY(s[i+1]);
  }

  return scontour;
}

QVector<QVector2D> extractSimplifiedContourPointPercentage(
  const QSize &size, const QVector<bool> &bimg,  float perc)
{
  QVector<float> contour = traceContourFlat(size, bimg);
  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    perc * contour.count(), std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < scontour.count(); i++) {
    scontour[i].setX(s[2*i]);
    scontour[i].setY(s[2*i+1]);
  }

  return scontour;
}

QVector<float> simplifyContourFlat(const QVector<float> &contour, 
  int npoints)
{
  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(scontour));
  return scontour;
}

QVector<float> simplifyContourFlat(const QVector<QVector2D> &contour, 
  int npoints)
{
  QVector<float> fcontour;
  fcontour.reserve(contour.count()*2);
  for (const QVector2D &p : contour)  {
    fcontour.append(p.x());
    fcontour.append(p.y());
  }

  QVector<float> scontour;
  psimpl::simplify_douglas_peucker_n<2>(fcontour.begin(), fcontour.end(),
    npoints, std::back_inserter(scontour));
  return scontour;
}

QVector<QVector2D> simplifyContourPoint(const QVector<float> &contour, 
  int npoints)
{
  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(contour.begin(), contour.end(),
    npoints, std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < scontour.count(); i+=2) {
    scontour[i].setX(s[2*i]);
    scontour[i].setY(s[2*i+1]);
  }

  return scontour;  
}

QVector<QVector2D> simplifyContourPoint(const QVector<QVector2D> &contour, 
  int npoints)
{
  QVector<float> fcontour;
  fcontour.reserve(contour.count()*2);
  for (const QVector2D &p : contour)  {
    fcontour.append(p.x());
    fcontour.append(p.y());
  }

  QVector<float> s;
  psimpl::simplify_douglas_peucker_n<2>(fcontour.begin(), fcontour.end(),
    npoints, std::back_inserter(s));

  QVector<QVector2D> scontour(s.count() / 2);
  for (int i = 0; i < scontour.count(); i+=2) {
    scontour[i].setX(s[2*i]);
    scontour[i].setY(s[2*i+1]);
  }

  return scontour;  
}