#pragma once

#include "switchselection.h"

#include "../diagram.h"
#include "undoid.h"

namespace undo {

/**
 * @brief The EditProperties class This class represents the edit properties command
 */
class EditProperties : public SwitchSelection
{
public:
    /**
     * @brief EditProperties Constructor
     * @param diagram Reference to the diagram
     * @param index Index of the shape to edit
     */
    EditProperties(data::Diagram& diagram, int index);
    /**
     * @brief recordProperties Records the new element properties
     */
    void recordProperties();
    /**
     * @brief id Returns the command id
     * @return command id
     */
    int id() const override { return undo::ciEditProperties; }

    /**
     * @brief undo Undoes the command
     */
    void undo() override;
    /**
     * @brief redo Redoes the command
     */
    void redo() override;


private:
    data::Diagram& m_diagram;
    int m_index;
    QColor m_oldBg;
    QColor m_oldFg;
    QString m_oldText;

    QColor m_newBg;
    QColor m_newFg;
    QString m_newText;
};

} // namespace
