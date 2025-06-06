#include "manipulateshape.h"

namespace undo {

void ManipulateShape::updateConnections()
{
  for (auto index : m_shapes) {
    auto* shape = m_diagram.shapes().at(index).get();
    auto& cons  = m_diagram.connections();
    std::for_each(cons.begin(), cons.end(), [this, shape](auto& con) {
      if ((con.out() == shape) || (con.in() == shape))
        con.update();
    });
  }
}

} // namespace undo
