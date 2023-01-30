#pragma once 

// =========================================================================================
// SKELETON COMPUTER BASED ON AUGMENTED FAST MARCHING METHOD (PROF. ALEX TELEA)
// =========================================================================================

#include <map>
#include <QVector>
#include <QPoint>
#include <QSize>

// ==========================================================================================
// MULTIMAP WRAPPER TO EXPOSE THE NARROW BAND FUNCTIONALITY
// ==========================================================================================

class NarrowBand
{
public:
  NarrowBand() {}

  inline void insert(float dist, int pidx) { map_.insert({dist, pidx}); }
  bool empty() const { return map_.empty(); }
  unsigned int getSmallest();
  void reinsert(float dist, int pidx);
  inline int count(float dist) const { return map_.count(dist); }
  inline int size() const { return map_.size(); }

private:
  std::multimap<float, int> map_;

// =============================================================================================
// ITERATORS AND USEFUL TYPES 
// =============================================================================================
public:
  using map_type = decltype(map_);
  using key_type = typename map_type::key_type;
  using mapped_type = typename map_type::mapped_type;
  using value_type = typename map_type::value_type;
  using iterator = typename map_type::iterator;
  using const_iterator = typename map_type::const_iterator;
  using reverse_iterator = typename map_type::reverse_iterator;
  using const_reverse_iterator = typename map_type::const_reverse_iterator;

  iterator begin() { return map_.begin(); }
  iterator end() { return map_.end(); }
  const_iterator cbegin() const { return map_.cbegin(); }
  const_iterator cend() const { return map_.cend(); }
  reverse_iterator rbegin() { return map_.rbegin(); }
  reverse_iterator rend() { return map_.rend(); }
  const_reverse_iterator crbegin() const { return map_.crbegin(); }
  const_reverse_iterator crend() const { return map_.crend(); }
};

// ===============================================================================================
// SKELETON COMPUTER (AFMM) 
// ===============================================================================================
class SkeletonComputer
{
public:
// ===============================================================================================
// ENUMS 
// ===============================================================================================
  enum State { Known, Band, Unknown };
  enum InitVertScanOrder { TopToBottom, BottomToTop };

// ===============================================================================================
// PUBLIC METHODS
// ===============================================================================================
  SkeletonComputer(const QSize& size, const QVector<bool> &bimg);

  QVector<bool> compute(float thres, InitVertScanOrder order=TopToBottom);

  inline QVector<bool> operator()(float thres, InitVertScanOrder order=TopToBottom)
    { return compute(thres, order); }

private:
// ===============================================================================================
// PRIVATE METHODS
// ===============================================================================================
  QPoint point(unsigned int pidx) const;
  unsigned int index(const QPoint &p) const;
  bool imgContains(const QPoint &p) const;

  NarrowBand initTopToBottom();
  NarrowBand initBottomToTop();

  bool boundary(const QPoint &p) const;
  float update(int lr, int i, int j, int tb);

  void computeFeatureTransformPoint(unsigned int nidx);
  void computeFeatureTransformBoundary(const NarrowBand &narrowband);
  void traceBoundary(unsigned int pidx);

// =================================================================================================
// MEMBERS VARIABLES (ATTRIBUTES)
// =================================================================================================
  const QVector<bool> &bimg_;
  QSize size_;

  QVector<float> DT_;
  QVector<State> state_;
  QVector<unsigned int> FT_;  // Feature Transform := U from the paper
  QVector<QPoint> from_;
  QVector<bool> visited_;

  unsigned int curBoundaryId_;

// ==================================================================================================
// STATIC VARIABLES (MEMBERS)
// ==================================================================================================
  const static float MAX_DIST;
  const static QVector<QPoint> AdjE; // eight connected adjacency
  const static QVector<QPoint> Adj;
  const static QVector<QPoint> SkelAdj;
  const static unsigned int INVALID_FT;
};
