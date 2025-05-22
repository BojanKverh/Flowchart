#pragma once

#include "../connection.h"
#include "../diagram.h"

#include <QJsonObject>

namespace json {

/**
 * @brief The Connection class This class is used to read/write connections
 * from file
 */
class Connection
{
public:
    /**
     * @brief Connection Default constructor
     */
    Connection();
    /**
     * @brief toJson Converts given connection into JSON object
     * @param con Connection to convert
     * @param diagram Read only reference to the diagram to
     * calculate connector indices
     * @return Generated JSON
     */
    QJsonObject toJson(const data::Connection& con, const data::Diagram &diagram) const;
};

} // namespace
