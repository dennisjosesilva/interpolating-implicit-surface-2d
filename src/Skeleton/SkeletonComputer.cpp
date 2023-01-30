#include "Skeleton/SkeletonComputer.hpp"

#include <limits>
#include <cmath>
#include <set>

// ===============================================================================
// NARROW BAND 
// ===============================================================================
unsigned int NarrowBand::getSmallest()
{
  using NBItem = NarrowBand::const_iterator;
  NBItem item = map_.cbegin();
  unsigned int pidx = item->second;
  map_.erase(item);
  return pidx;
}

void NarrowBand::reinsert(float dist, int pidx)
{
  // Remove (dist, pidx)
  using NBIterator = NarrowBand::iterator;
  using Range = std::pair<NBIterator, NBIterator>;
  
  Range range = map_.equal_range(dist);
  NBIterator itr = range.first;
  while (itr->second != pidx && itr != range.second) {
    itr++;
  }

  if (itr != range.second)
   map_.erase(itr);

  // Insert pidx at dist (new distance)
  insert(dist, pidx);
}

// =====================================================================================
// SKELETON COMPUTER
// =====================================================================================
const float SkeletonComputer::MAX_DIST = std::numeric_limits<float>::max();
const QVector<QPoint> SkeletonComputer::AdjE = {{1, 0}, {1, 1,}, {0, 1}, {-1, 1}, 
  {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
const QVector<QPoint> SkeletonComputer::Adj = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
const unsigned int SkeletonComputer::INVALID_FT = 
  std::numeric_limits<unsigned int>::max();

SkeletonComputer::SkeletonComputer(const QSize &size, const QVector<bool> &bimg)
  : bimg_{bimg},
    size_{size},
    DT_(size.height() * size.width()),
    state_(size.height() / size.width()),
    FT_(size.height() * size.width()),
    visited_(size.height() * size.width(), false),
    curBoundaryId_{1}
{}

QVector<bool> SkeletonComputer::compute(float thres, InitVertScanOrder order)
{
  NarrowBand narrowband;

  // 1. Initialization
  if (order == TopToBottom)
    narrowband = initTopToBottom();
  else if (order == BottomToTop)
    narrowband = initBottomToTop();

  unsigned int boundaryLength = narrowband.size();

  from_.resize(boundaryLength + 1);
  // Compute "from" array containing an ID for each boundary pixel
  computeFeatureTransformBoundary(narrowband);

  // 2. March the narrowband
  while (!narrowband.empty()) {
    unsigned int pidx = narrowband.getSmallest();
    state_[pidx] = Known;

    QPoint p = point(pidx);

    // 3. Get non-known neighbors of p
    QVector<unsigned int> nbs;
    nbs.reserve(4);
    for (const QPoint &o : Adj) {
      QPoint q = p + o;
      if (imgContains(p)) {
        unsigned int qidx = index(q);
        if (state_[qidx] == Known) continue;
        nbs.append(qidx);
        if (state_[qidx] != Band) {
          state_[qidx] = Band;
          narrowband.insert(DT_[qidx], qidx);
          computeFeatureTransformPoint(qidx);
        }
      }
    }

    for (unsigned int qidx : nbs) {
      QPoint n = point(qidx);
      float d = DT_[qidx];
      d = std::min(d, update(n.x() - 1, n.x(), n.y(), n.y() - 1));
      d = std::min(d, update(n.x() + 1, n.x(), n.y(), n.y() - 1));
      d = std::min(d, update(n.x() - 1, n.x(), n.y(), n.y() + 1));
      d = std::min(d, update(n.x() + 1, n.x(), n.y(), n.y() + 1));

      if (d != DT_[qidx]) {
        narrowband.reinsert(d, qidx);
        DT_[qidx] = d;
      }
    }
  }

  // 4. Compute Skeleton 
  QVector<bool> skel(size_.width() * size_.height(), false);
  QPoint p;
  for (p.ry() = 0; p.y() < size_.height(); p.ry()++) {
    for (p.rx() = 0; p.x() < size_.width(); p.ry()++) {
      unsigned int pidx = index(p);
      if (FT_[pidx] != INVALID_FT) {
        for (const QPoint &o : Adj) {
          QPoint n = p + o;
          if (imgContains(n)) {
            unsigned int nidx = index(n);
            unsigned int diff = (FT_[nidx] == INVALID_FT) ? 0 :
              std::abs(static_cast<int>(FT_[nidx] - FT_[pidx]));
            if (diff >= thres) {
              skel[pidx] = true;
              break;
            }
          }
        }
      }
    }
  }

  return skel;
}

QPoint SkeletonComputer::point(unsigned int pidx) const 
{
  int minInt = std::numeric_limits<int>::min();
  if (pidx < size_.width()*size_.height()) {
    int w = size_.width();
    return QPoint { pidx % w, pidx / w};
  }
  else 
    return QPoint{ minInt, minInt };
}

unsigned int SkeletonComputer::index(const QPoint &p) const 
{
  if (imgContains(p)) 
    return p.y() * size_.width() + p.x();
  
  return std::numeric_limits<unsigned int>::max();
}

bool SkeletonComputer::imgContains(const QPoint &p) const 
{
  return 0 <= p.x() && p.x() < size_.width() && 0 <= p.y() && p.y() < size_.height();
}

NarrowBand SkeletonComputer::initTopToBottom()
{
  NarrowBand narrowband;
  QPoint p;
  for (p.ry() = 0; p.y() < size_.height(); p.ry()++) {
    for (p.rx() = 0; p.x() < size_.width(); p.rx()++)
    {
      unsigned int pidx = index(p);
      if (boundary(p)) {  // boundary
        state_[pidx] = Band;
        DT_[pidx] = 0.0f;
        narrowband.insert(DT_[pidx], pidx);
      }
      else if (bimg_[pidx]) { // foreground
        state_[pidx] = Unknown;
        DT_[pidx] = MAX_DIST;
        FT_[pidx] = INVALID_FT;
      }
      else { // background
        state_[pidx] = Known;
        DT_[pidx] = 0.0f;
        FT_[pidx] = INVALID_FT;
      }
    }
  }

  return narrowband;
}

NarrowBand SkeletonComputer::initBottomToTop()
{
  NarrowBand narrowband;

  QPoint p;
  for (p.ry() = size_.height()-1; p.y() >= 0; p.ry()--) {
    for (p.rx() = 0; p.x() < size_.width(); p.rx()++) {
      unsigned int pidx = index(p);
      if (boundary(p)) { // boundary
        state_[pidx] = Band;
        DT_[pidx] = 0.0f;
        narrowband.insert(DT_[pidx], pidx);
      }
      else if (bimg_[pidx]) { // foreground
        state_[pidx] = Unknown;
        DT_[pidx] = MAX_DIST;
        FT_[pidx] = INVALID_FT;
      }
      else { // background
        state_[pidx] = Known;
        DT_[pidx] = 0.0f;
        FT_[pidx] = INVALID_FT;
      }
    }
  }

  return narrowband;
}

// TODO: FINISH IT!!!!!