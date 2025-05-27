#pragma once

#include <QDialog>

#include "shapes/abstractshape.h"

#include <QLineEdit>

class QPushButton;

/**
 * @brief The DialogShape class This dialog allows user to edit the shape properties
 */
class DialogShape : public QDialog
{
    Q_OBJECT

public:
    DialogShape(data::AbstractShape* shape, QWidget* parent = nullptr);

    QColor backgroundColor() const { return m_bg; }
    QColor textColor() const { return m_fg; }
    QString text() const { return m_pleText->text(); }

private slots:
    void editBg();
    void editFg();
    void updateUI();

private:
    QColor m_bg;
    QColor m_fg;
    QPushButton* m_pbBg;
    QPushButton* m_pbFg;
    QLineEdit* m_pleText;
};
