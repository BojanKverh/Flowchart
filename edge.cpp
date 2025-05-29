#include "edge.h"

namespace data {

std::tuple<double, double, double, double> EdgeHandler::move(Edge edge, QPointF oldPt, QPointF newPt)
{
    double dX = 0.0;
    double dY = 0.0;
    double dW = 0.0;
    double dH = 0.0;

    auto pt = newPt - oldPt;

    switch(edge) {
    case Edge::eTopLeft:
        dX = pt.x();
        dY = pt.y();
        dW = -dX;
        dH = -dY;
        break;

    case Edge::eTop:
        dY = pt.y();
        dH = -dY;
        break;

    case Edge::eTopRight:
        dY = pt.y();
        dW = pt.x();
        dH = -dY;
        break;

    case Edge::eRight:
        dW = pt.x();
        break;

    case Edge::eBottomRight:
        dW = pt.x();
        dH = pt.y();
        break;

    case Edge::eBottom:
        dH = pt.y();
        break;

    case Edge::eBottomLeft:
        dX = pt.x();
        dH = pt.y();
        dW = -dX;
        break;


    case Edge::eLeft:
        dX = pt.x();
        dW = -dX;
        break;

    default:
        break;
    }

    return std::make_tuple(dX, dY, dW, dH);
}

Qt::CursorShape EdgeHandler::shape(Edge edge)
{
    Qt::CursorShape shape = Qt::CursorShape::ArrowCursor;
    switch (edge) {
    case data::Edge::eNone:
        break;
    case data::Edge::eTopLeft:
    case data::Edge::eBottomRight:
        shape = Qt::CursorShape::SizeFDiagCursor;
        break;
    case data::Edge::eTopRight:
    case data::Edge::eBottomLeft:
        shape = Qt::CursorShape::SizeBDiagCursor;
        break;
    case data::Edge::eTop:
    case data::Edge::eBottom:
        shape = Qt::CursorShape::SizeVerCursor;
        break;
    case data::Edge::eLeft:
    case data::Edge::eRight:
        shape = Qt::CursorShape::SizeHorCursor;
        break;
    }

    return shape;
}

} // namespace
