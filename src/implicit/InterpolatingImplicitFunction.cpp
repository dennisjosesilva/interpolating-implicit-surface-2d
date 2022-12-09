#include "implicit/InterpolatingImplicitFunction.hpp"

#include <Eigen/Dense>
#include <Eigen/Core>

#include <QtMath>

#include <iostream>

InterpolatingImplicitFunction::InterpolatingImplicitFunction()
{}

InterpolatingImplicitFunction::InterpolatingImplicitFunction(
  const QVector<QVector3D> &constraints, const QVector<float> &fval)
  : constraints_{constraints}, fval_{fval}
{}

InterpolatingImplicitFunction::InterpolatingImplicitFunction(
  const QVector<QVector3D> &boundaryConstraint, 
  const QVector<QVector3D> &posConstraint,
  const QVector<QVector3D> &negConstraint)
{
  for (const QVector3D &c : negConstraint) {
    constraints_.append(c);
    fval_.append(-1.0f);
  }

  for (const QVector3D &c : boundaryConstraint) {
    constraints_.append(c);
    fval_.append(0.0f);
  }

  for (const QVector3D &c : posConstraint) {
    constraints_.append(c);
    fval_.append(1.0f);
  }
}

int InterpolatingImplicitFunction::pushBoundaryConstraint(const QVector3D &c)
{
  constraints_.append(c);
  fval_.append(0.0f);
  return constraints_.count() - 1;
}

int InterpolatingImplicitFunction::pushInteriorConstraint(const QVector3D &c)
{
  constraints_.append(c);
  fval_.append(1.0f);
  return constraints_.count() - 1;
}

int InterpolatingImplicitFunction::pushExteriorConstraint(const QVector3D &c)
{
  constraints_.append(c);
  fval_.append(-1.0f);
  return constraints_.count() - 1;
}

InterpolatingImplicitFunction::ImplFunctionType
InterpolatingImplicitFunction::optimize()
{
  using Eigen::MatrixXf;
  using Eigen::VectorXf;

  // ============================
  // Solve optimisation problem
  // ============================
  const unsigned int NC = constraints_.count();
  const unsigned int MS = NC+4;

  // ================================
  // Create matrix for optimisation
  // ================================
  MatrixXf M{MS, MS};
  for (int row = 0; row < NC; row++) {
    const QVector3D &ci = constraints_[row];
    for (int col = 0; col < NC; col++) {      
      const QVector3D &cj = constraints_[col];

      M(row, col) = radialBasisFunction(ci - cj);
    }

    M(row, NC) = 1.0f;
    M(row, NC+1) = ci.x();
    M(row, NC+2) = ci.y();
    M(row, NC+3) = ci.z();
  }

  for (int col=0; col < NC; col++)
    M(NC, col) = 1;
  
  M(NC, NC) = M(NC, NC+1) = M(NC, NC+2) = M(NC, NC+3) = 0;

  for (int col=0; col < NC; col++) {
    const QVector3D &c = constraints_[col];
    M(NC+1, col) = c.x();
    M(NC+2, col) = c.y();
    M(NC+3, col) = c.z();
  }

  M(NC+1, NC) = M(NC+1, NC+1) = M(NC+1, NC+2) = M(NC+1, NC+3) = 0;
  M(NC+2, NC) = M(NC+2, NC+1) = M(NC+2, NC+2) = M(NC+2, NC+3) = 0;
  M(NC+3, NC) = M(NC+3, NC+1) = M(NC+3, NC+2) = M(NC+3, NC+3) = 0;

  // =============
  // Call solver 
  // =============
  QVector<float> cfval = fval_;
  cfval.append(0); cfval.append(0); cfval.append(0); cfval.append(0); 
  VectorXf b = 
    Eigen::Map<VectorXf, Eigen::Unaligned>(cfval.data(), cfval.count());

  VectorXf sol = M.colPivHouseholderQr().solve(b);

  std::cout << "M = \n" << M << "\nsol =\n" << sol; 

  return [sol, NC, this] (const QVector3D &x) {
    double sum = 0.0;
    for (int i = 0; i < NC; i++)
      sum += sol[i] * radialBasisFunction(x - constraints_[i]);
    
    return sum + sol[NC] 
                    + (sol[NC+1] * x.x()) 
                    + (sol[NC+2] * x.y())
                    + (sol[NC+3] * x.z());
  };
}

double InterpolatingImplicitFunction::radialBasisFunction(const QVector3D &p)
{
  double plength = p.length();
  if (plength == 0.0)
    return 0;
  
  return (plength*plength) * log(plength);
}