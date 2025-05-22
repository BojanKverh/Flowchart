#pragma once

#include "abstractshape.h"

#include "../shapes/shapeprocess.h"

namespace json {

/**
 * @brief The ShapeProcess class This class is used to read/write the process shape in JSON format
 */
class ShapeProcess : public AbstractShape
{
public:
    /**
     * @brief ShapeProcess Default constructor
     */
    ShapeProcess();
    /**
     * @brief toJson Converts shape to JSON
     * @param shape Pointer to the shape to read/write
     * @return Generated JSON object
     */
    QJsonObject toJson(data::AbstractShape *shape) const override;
};

} // namespace
