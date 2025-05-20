#include "diagram.h"

#include <algorithm>

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

Diagram::Error Diagram::addConnection(const Connection& con)
{
    m_vConnections.push_back(con);
    return Error::eNone;
}

void Diagram::select(int n)
{
    for (size_t i = 0; i < m_vShapes.size(); ++i)
        m_vShapes[i]->setSelected(static_cast<int>(i) == n);
}

int Diagram::findShape(QPointF pt) const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [pt](const auto& shape) {
        return shape->contains(pt);
    });
    return (it == m_vShapes.end()? -1 : std::distance(m_vShapes.begin(), it));
}

void Diagram::moveSelected(QPointF pt)
{
    for (auto& shape : m_vShapes) {
        if (shape->isSelected() == true)
            shape->move(shape->position() + pt);
    }
}

Connection Diagram::findConnector(QPointF pt) const
{
    for (const auto& shape : m_vShapes) {
        int outIndex = shape->findOutput(pt);
        if (outIndex >= 0)
            return Connection(shape.get(), outIndex, nullptr);
        if (shape->isOnInput(pt) == true)
            return Connection(nullptr, -1, shape.get());
    }

    return Connection(nullptr, -1, nullptr);
}

bool Diagram::hasStart() const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [](const auto& shape) {
        return shape->type() == ShapeType::esStart;
    });
    return it != m_vShapes.end();
}

bool Diagram::hasEnd() const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [](const auto& shape) {
        return shape->type() == ShapeType::esEnd;
    });
    return it != m_vShapes.end();
}
