#include "connection.h"

namespace json {

Connection::Connection()
{}

QJsonObject Connection::toJson(const data::Connection &con, const data::Diagram& diagram) const
{
    QJsonObject obj;
    obj[m_cqsOut] = diagram.indexOf(con.out());
    obj[m_cqsOutIndex] = con.outIndex();
    obj[m_cqsIn] = diagram.indexOf(con.in());
    return obj;
}

data::Connection Connection::fromJson(const QJsonObject& obj, const data::Diagram &diagram)
{
    data::Connection con(
        diagram.shapes().at(obj[m_cqsOut].toInt()).get(),
        obj[m_cqsOutIndex].toInt(),
        diagram.shapes().at(obj[m_cqsIn].toInt()).get()
        );

    return con;
}

} // namespace
