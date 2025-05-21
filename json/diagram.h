#pragma once

#include "../diagram.h"

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
     */
    Diagram(data::Diagram& diagram);

private:
    data::Diagram& m_diagram;
};

} // namespace
