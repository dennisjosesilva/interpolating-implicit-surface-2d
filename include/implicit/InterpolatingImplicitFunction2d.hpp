#pragma once 

#include <QVector>
#include <QVector2D>

#include <functional>

class InterpolatingImplicitFunction2D
{
public:
  using ImplFunctionType = std::function<double (const QVector2D &)>;

  InterpolatingImplicitFunction2D();
  InterpolatingImplicitFunction2D(const QVector<QVector2D> &constraints,
    const QVector<float> &fval);
  InterpolatingImplicitFunction2D(const QVector<QVector2D> &boundaryConstraint,
    const QVector<QVector2D> &posConstraint, const QVector<QVector2D> &negConstraint);

  int pushBoundaryConstraint(const QVector2D &c);
  int pushInteriorConstraint(const QVector2D &c);
  int pushExteriorConstraint(const QVector2D &c);

  const QVector2D& constraint(int index) const { return constraints_[index]; }
  float fval(int index) const { return fval_[index]; }

  ImplFunctionType optimize() const;

  unsigned int numConstraints() const { return constraints_.count(); }
  bool isInteriorConstraint(int index) const { return fval_[index] > 0; }
  bool isExteriorConstraint(int index) const { return fval_[index] < 0; } 
  bool isBoundaryConstraint(int index) const { return fval_[index] == 0.0f; }

private:
  double radialBasisFunction(const QVector2D &p) const;  


private:
  QVector<QVector2D> constraints_;
  QVector<float> fval_;
};