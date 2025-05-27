#pragma once

#include "../diagram.h"
#include "../erroremitter.h"

#include <QUndoCommand>

namespace undo {

/**
 * @brief The AddShape class This class represents the add shape command
 */
class AddShape : public QUndoCommand
{
public:
    /**
     * @brief AddShape Constructor
     * @param emitter Pointer to the error emitter
     * @param diagram Reference to the diagram
     * @param type Shape type
     * @param pt Topleft corner of the shape
     */
    AddShape(data::ErrorEmitter* emitter, data::Diagram& diagram, data::ShapeType type, const QPointF& pt) :
        m_emitter(emitter), m_diagram(diagram), m_type(type), m_pt(pt)
    {
        m_index = m_diagram.shapes().size();
    }
    /**
     * @brief undo Undoes the command
     */
    void undo() override;
    /**
     * @brief redo Redoes the command
     */
    void redo() override;

private:
    data::ErrorEmitter* m_emitter;
    data::Diagram& m_diagram;
    data::ShapeType m_type;
    QPointF m_pt;
    size_t m_index;
};

}
