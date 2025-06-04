#include "moveshape.h"

#include "../utils/setcompare.h"

namespace undo {

MoveShape::MoveShape(data::Diagram& diagram, const std::unordered_set<int>& shapes, QPointF diffPos)
    : ManipulateShape(diagram, shapes)
    , m_diffPt(diffPos)
{}

void MoveShape::undo()
{
  for (auto index : shapes()) {
    auto& shape = diagram().shapes().at(index);
    shape->move(shape->position() - m_diffPt);
  }
  updateConnections();
}

void MoveShape::redo()
{
  for (auto index : shapes()) {
    auto& shape = diagram().shapes().at(index);
    shape->move(shape->position() + m_diffPt);
  }
  updateConnections();
}

bool MoveShape::mergeWith(const QUndoCommand* com)
{
  const auto* moveCom = dynamic_cast<const MoveShape*>(com);
  if ((moveCom == nullptr) || (utils::SetCompare::equal(shapes(), moveCom->shapes()) == false))
    return false;

  m_diffPt += moveCom->diff();
  return true;
}

} // namespace undo
