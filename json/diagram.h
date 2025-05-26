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
    /**
     * @brief fromJson Parses the diagram data from given JSON document
     * @param doc Document to parse
     */
    void fromJson(const QJsonDocument& doc);
    /**
     * @brief width Returns the parsed width
     * @return parsed width
     */
    int width() const { return m_w; }
    /**
     * @brief height Returns the parsed height
     * @return parsed height
     */
    int height() const { return m_h; }

private:
    data::Diagram& m_diagram;
    int m_w;
    int m_h;

    const QString m_cqsWidth = "w";
    const QString m_cqsHeight = "h";
    const QString m_cqsShapes = "shapes";
    const QString m_cqsConnections = "cons";
};

} // namespace
