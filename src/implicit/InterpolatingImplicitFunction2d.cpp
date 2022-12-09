#include "implicit/InterpolatingImplicitFunction2d.hpp"

#include <Eigen/Dense>
#include <Eigen/Core>

#include <QtMath>

#include <iostream>

InterpolatingImplicitFunction2D::InterpolatingImplicitFunction2D()
{}

InterpolatingImplicitFunction2D::InterpolatingImplicitFunction2D(
  const QVector<QVector2D> &constraints, const QVector<float> &fval)
  : constraints_{constraints}, fval_{fval}
{}

InterpolatingImplicitFunction2D::InterpolatingImplicitFunction2D(
  const QVector<QVector2D> &boundaryConstraint, 
  const QVector<QVector2D> &posConstraint,
  const QVector<QVector2D> &negConstraint)
{
  for (const QVector2D &c : negConstraint) {
    constraints_.append(c);
    fval_.append(-1.0f);
  }

  for (const QVector2D &c : boundaryConstraint) {
    constraints_.append(c);
    fval_.append(0.0f);
  }

  for (const QVector2D &c : posConstraint) {
    constraints_.append(c);
    fval_.append(1.0f);
  }
}

int InterpolatingImplicitFunction2D::pushBoundaryConstraint(const QVector2D &c)
{
  constraints_.append(c);
  fval_.append(0.0f);
  return constraints_.count() - 1;
}

int InterpolatingImplicitFunction2D::pushInteriorConstraint(const QVector2D &c)
{
  constraints_.append(c);
  fval_.append(1.0f);
  return constraints_.count() - 1;
}

int InterpolatingImplicitFunction2D::pushExteriorConstraint(const QVector2D &c)
{
  constraints_.append(c);
  fval_.append(-1.0f);
  return constraints_.count() - 1;
}

InterpolatingImplicitFunction2D::ImplFunctionType
InterpolatingImplicitFunction2D::optimize() const
{
  using Eigen::MatrixXf;
  using Eigen::VectorXf;

  // ============================
  // Solve optimisation problem
  // ============================
  const unsigned int NC = constraints_.count();
  const unsigned int MS = NC+3;

  // ================================
  // Create matrix for optimisation
  // ================================
  MatrixXf M{MS, MS};
  for (int row = 0; row < NC; row++) {
    const QVector2D &ci = constraints_[row];
    for (int col = 0; col < NC; col++) {      
      const QVector2D &cj = constraints_[col];

      float r = radialBasisFunction(ci - cj);
      M(row, col) = r;
    }

    M(row, NC) = 1.0f;
    M(row, NC+1) = ci.x();
    M(row, NC+2) = ci.y();
  }

  for (int col=0; col < NC; col++)
    M(NC, col) = 1;
  
  M(NC, NC) = M(NC, NC+1) = M(NC, NC+2) = 0;

  for (int col=0; col < NC; col++) {
    const QVector2D &c = constraints_[col];
    M(NC+1, col) = c.x();
    M(NC+2, col) = c.y();
  }

  M(NC+1, NC) = M(NC+1, NC+1) = M(NC+1, NC+2) = 0;
  M(NC+2, NC) = M(NC+2, NC+1) = M(NC+2, NC+2) = 0;
  

  // =============
  // Call solver 
  // =============
  QVector<float> cfval = fval_;
  cfval.append(0); cfval.append(0); cfval.append(0);
  VectorXf b = 
    Eigen::Map<VectorXf, Eigen::Unaligned>(cfval.data(), cfval.count());

  VectorXf sol = M.colPivHouseholderQr().solve(b);

  return [sol, NC, this] (const QVector2D &x) {
    double sum = 0.0;
    for (int i = 0; i < NC; i++)
      sum += sol[i] * radialBasisFunction(x - constraints_[i]);
    
    return sum + sol[NC] 
                    + (sol[NC+1] * x.x()) 
                    + (sol[NC+2] * x.y());
  };
}

double InterpolatingImplicitFunction2D::radialBasisFunction(const QVector2D &p) const
{
  double plength = p.length();
  if (plength == 0.0)
    return 0;
  
  return (plength*plength) * log(plength);
}