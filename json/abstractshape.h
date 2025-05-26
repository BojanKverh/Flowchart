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
    /**
     * @brief fromJson Parses the shape data from JSON object and creates a proper shape
     * @param obj JSON object to parse
     * @return Pointer to the created shape. Caller is responsible to delete the returned object
     */
    virtual data::AbstractShape* fromJson(const QJsonObject& obj) const;

protected:
    /**
     * @brief apply Applies the information from JSON object to the shape
     * @param shape Pointer to the shape, where information will be stored
     * @param obj JSON object containing shape information
     */
    void apply(data::AbstractShape* shape, const QJsonObject& obj) const;

protected:
    const QString m_cqsType = "type";
    const QString m_cqsX = "x";
    const QString m_cqsY = "y";
    const QString m_cqsPosition = "pos";
    const QString m_cqsWidth = "w";
    const QString m_cqsHeight = "h";
    const QString m_cqsSize = "size";
    const QString m_cqsBackgroundColor = "bg";
    const QString m_cqsTextColor = "fg";
};

} // namespace
