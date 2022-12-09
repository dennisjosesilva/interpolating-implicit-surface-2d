#pragma once 

#include <QVector>
#include <QVector3D>

#include <functional>

class InterpolatingImplicitFunction
{
public:
  using ImplFunctionType = std::function<double (const QVector3D &)>;

  InterpolatingImplicitFunction();
  InterpolatingImplicitFunction(const QVector<QVector3D> &constraints,
    const QVector<float> &fval);
  InterpolatingImplicitFunction(const QVector<QVector3D> &boundaryConstraint,
    const QVector<QVector3D> &posConstraint, const QVector<QVector3D> &negConstraint);

  
  int pushBoundaryConstraint(const QVector3D &c);
  int pushInteriorConstraint(const QVector3D &c);
  int pushExteriorConstraint(const QVector3D &c);

  const QVector3D& constraint(int index) const { return constraints_[index]; }
  float fval(int index) const { return fval_[index]; }

  ImplFunctionType optimize();

private:
  double radialBasisFunction(const QVector3D &p);  


private:
  QVector<QVector3D> constraints_;
  QVector<float> fval_;
};