#include "diagram.h"

Diagram::Diagram() {}

Diagram::Error Diagram::addShape(std::unique_ptr<AbstractShape> shape)
{
    if ((shape->type() == ShapeType::esStart) && (hasStart() == true))
        return Error::eStartExists;
    if ((shape->type() == ShapeType::esEnd) && (hasEnd() == true))
        return Error::eEndExists;

    m_vShapes.push_back(std::move(shape));
    return Error::eNone;
}

void Diagram::select(int n)
{
    for (size_t i = 0; i < m_vShapes.size(); ++i)
        m_vShapes[i]->setSelected(static_cast<int>(i) == n);
}

void Diagram::moveSelected(QPointF pt)
{
    for (auto& shape : m_vShapes) {
        if (shape->isSelected() == true)
            shape->move(shape->position() + pt);
    }
}

bool Diagram::hasStart() const
{
    for (const auto& shape : m_vShapes)
        if (shape->type() == ShapeType::esStart)
            return true;

    return false;
}

bool Diagram::hasEnd() const
{
    for (const auto& shape : m_vShapes)
        if (shape->type() == ShapeType::esEnd)
            return true;

    return false;
}
