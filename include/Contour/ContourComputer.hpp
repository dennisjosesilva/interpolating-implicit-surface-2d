#pragma once 

#include <QPoint>
#include <QVector>
#include <QVector2D>
#include <QSize>

class ContourComputer
{
public:
  ContourComputer(const QSize &size, const QVector<bool> &bimg);

  QVector<bool> compute() const;

  inline QVector<bool> operator()() const { return compute(); }

private:
  // =====================================================================
  // PRIVATE AUXILIARY METHODS 
  // =====================================================================
  QPoint point(unsigned int pidx) const;
  unsigned int index(const QPoint &p) const;
  bool imgContains(const QPoint &p) const;
  bool boundary(const QPoint &p) const;  

  // =====================================================================
  // MEMBERS (ATTRIBUTES)
  // =====================================================================
  const QVector<bool> &bimg_;
  const QSize size_;

  // =====================================================================
  // STATIC CONST VARIABLES
  // =====================================================================
  const static QVector<QPoint> Adj;
};

// ========================================================================
// FUNCTIONS 
// ========================================================================
QVector<bool> computeContours(const QSize &size, 
  const QVector<bool> &bimg);

QVector<QVector2D> extractContours(const QSize &size, 
  const QVector<bool> &bimg);