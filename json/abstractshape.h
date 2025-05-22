#pragma once

#include <QJsonObject>

#include "../shapes/abstractshape.h"

namespace json {

/**
 * @brief The AbstractShape class This class is used to read/write common
 * shape properties into JSON object
 */
class AbstractShape
{
public:
    /**
     * @brief AbstractShape Default constructor
     */
    AbstractShape();
    /**
     * @brief toJson Converts shape to JSON
     * @param shape Pointer to the shape to read/write
     * @return Generated JSON object
     */
    virtual QJsonObject toJson(data::AbstractShape* shape) const;

private:
};

} // namespace
