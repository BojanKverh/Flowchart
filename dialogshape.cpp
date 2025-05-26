#include "dialogshape.h"

#include <QColorDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

DialogShape::DialogShape(data::AbstractShape* shape, QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Edit element properties"));
    auto* layout = new QVBoxLayout(this);

    auto* gb = new QGroupBox(tr("Colors"));
    auto* form = new QFormLayout(gb);
    m_pbBg = new QPushButton;
    m_pbBg->setFixedSize(32, 32);
    m_bg = shape->backgroundColor();
    form->addRow(tr("Background color"), m_pbBg);
    m_pbFg = new QPushButton;
    m_pbFg->setFixedSize(32, 32);
    m_fg = shape->textColor();
    form->addRow(tr("Text color"), m_pbFg);
    layout->addWidget(gb);

    gb = new QGroupBox(tr("Text"));
    form = new QFormLayout(gb);
    m_pleText = new QLineEdit;
    m_pleText->setText(shape->text());
    m_pleText->setEnabled((shape->type() != data::ShapeType::esStart) && (shape->type() != data::ShapeType::esEnd));
    form->addRow(tr("Text"), m_pleText);
    layout->addWidget(gb);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::StandardButton::Cancel
        );

    layout->addWidget(buttons);

    connect(m_pbBg, &QPushButton::clicked, this, &DialogShape::editBg);
    connect(m_pbFg, &QPushButton::clicked, this, &DialogShape::editFg);
    connect(buttons, &QDialogButtonBox::accepted, this, &DialogShape::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &DialogShape::reject);

    updateUI();
}

void DialogShape::editBg()
{
    QColorDialog dlg(m_bg);
    if (dlg.exec() == true) {
        m_bg = dlg.selectedColor();
        updateUI();
    }
}

void DialogShape::editFg()
{
    QColorDialog dlg(m_fg);
    if (dlg.exec() == true) {
        m_fg = dlg.selectedColor();
        updateUI();
    }
}

void DialogShape::updateUI()
{
    m_pbBg->setStyleSheet("background-color: " + m_bg.name());
    m_pbFg->setStyleSheet("background-color: " + m_fg.name());
}
