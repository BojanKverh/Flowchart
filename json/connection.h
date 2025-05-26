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
    /**
     * @brief fromJson Reads the data from object and creates a connection
     * @param obj JSON object to read the information from
     * @param diagram Read only reference to diagram object
     * @return Created connection
     */
    data::Connection fromJson(const QJsonObject& obj, const data::Diagram& diagram);

private:
    const QString m_cqsOut = "out";
    const QString m_cqsOutIndex = "index";
    const QString m_cqsIn = "in";
};

} // namespace
