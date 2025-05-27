#include "addshape.h"

#include "../shapes/shapefactory.h"

#include <QDebug>

namespace undo {

void AddShape::undo()
{
    m_diagram.removeShape(m_index);
}

void AddShape::redo()
{
    data::ShapeFactory factory;
    auto shape = factory.shape(static_cast<int>(m_type));
    shape->moveCenter(m_pt);
    auto err = m_diagram.addShape(std::move(shape));
    if (err != data::Diagram::Error::eNone)
        emit m_emitter->diagramError(err);
}

} // namespace
