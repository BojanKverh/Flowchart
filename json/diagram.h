#pragma once

#include "../diagram.h"

#include <QJsonDocument>

namespace json
{
/**
 * @brief The Diagram class This class is used to read and write diagrams in JSON format
 */
class Diagram
{
public:
    /**
     * @brief Diagram Default constructor
     * @param diagram Reference to the diagram data object
     * @param w canvas width
     * @param h canvas height
     */
    Diagram(data::Diagram& diagram, int w, int h);
    /**
     * @brief toJson Exports the diagram to JSON document
     * @return Generated JSON document
     */
    QJsonDocument toJson() const;

private:
    data::Diagram& m_diagram;
    int m_w;
    int m_h;
};

} // namespace
