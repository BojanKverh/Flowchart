#pragma once

#include "abstractshape.h"

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
    /**
     * @brief fromJson Parses the shape data from JSON object and creates a proper shape
     * @param obj JSON object to parse
     * @return Pointer to the created shape. Caller is responsible to delete the returned object
     */
    data::AbstractShape* fromJson(const QJsonObject& obj) const override;

protected:
    const QString m_cqsText = "text";
};

} // namespace
