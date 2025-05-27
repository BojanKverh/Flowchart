#include "shapefactory.h"

#include "shapetype.h"
#include "shapestart.h"
#include "shapeend.h"
#include "shapeprocess.h"
#include "shapedecision.h"
#include "shapeio.h"

namespace data {

std::unique_ptr<data::AbstractShape> ShapeFactory::shape(int type)
{
    using namespace data;

    auto eType = static_cast<ShapeType>(type);

    switch(eType) {
    case ShapeType::esStart:
        return std::make_unique<ShapeStart>();
    case ShapeType::esEnd:
        return std::make_unique<ShapeEnd>();
    case ShapeType::esProcess:
        return std::make_unique<ShapeProcess>();
    case ShapeType::esDecision:
        return std::make_unique<ShapeDecision>();
    case ShapeType::esIO:
        return std::make_unique<ShapeIO>();
    default:
        return {};
    }
}

} // namespace
