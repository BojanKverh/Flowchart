#include "connection.h"

namespace json {

Connection::Connection()
{}

QJsonObject Connection::toJson(const data::Connection &con, const data::Diagram& diagram) const
{
    QJsonObject obj;
    obj["out"] = diagram.indexOf(con.out());
    obj["index"] = con.outIndex();
    obj["in"] = diagram.indexOf(con.in());
    return obj;
}


} // namespace
