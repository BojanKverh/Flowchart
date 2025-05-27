#pragma once

#include "switchselection.h"
#include "../diagram.h"
#include "../erroremitter.h"

namespace undo {
/**
 * @brief The AddConnection class This class represents the add connection command
 */
class AddConnection : public SwitchSelection
{
public:
    /**
     * @brief AddConnection Constructor
     * @param emitter Pointer to the error emitter
     * @param diagram Referencet to the diagram
     * @param out index of the shape containing the out connector
     * @param outIndex Out connector index
     * @param in index of the shape containing the in connector
     */
    AddConnection(
        data::ErrorEmitter* emitter,
        data::Diagram& diagram,
        int out,
        int outIndex,
        int in) :
        SwitchSelection(diagram), m_emitter(emitter), m_diagram(diagram), m_out(out), m_outIndex(outIndex), m_in(in)
    {
        m_index = m_diagram.connections().size();
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
    int m_out;
    int m_outIndex;
    int m_in;
    size_t m_index;
};

}   // namespace
