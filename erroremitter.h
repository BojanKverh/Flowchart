#pragma once

#include <QObject>

#include "diagram.h"

namespace data
{

class ErrorEmitter : public QObject
{
    Q_OBJECT

public:
    ErrorEmitter() : QObject() {};

signals:
    /**
     * @brief diagramError Emitted when an error at diagram operation occured
     * @param error Error specification
     */
    void diagramError(data::Diagram::Error error);
};

} // namespace
