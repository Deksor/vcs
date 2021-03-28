/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#include <cmath>
#include "filter/filters/output_gate/filter_output_gate.h"
#include "filter/filters/output_gate/gui/qt/filtergui_output_gate.h"

filtergui_output_gate_c::filtergui_output_gate_c(filter_c *const filter) : filtergui_c(filter, 180)
{
    QLabel *widthLabel = new QLabel("Width", this->widget);
    QSpinBox *widthSpin = new QSpinBox(this->widget);
    widthSpin->setRange(0, MAX_OUTPUT_WIDTH);
    widthSpin->setValue(this->filter->parameter(filter_output_gate_c::PARAM_WIDTH));

    QLabel *heightLabel = new QLabel("Height", this->widget);
    QSpinBox *heightSpin = new QSpinBox(this->widget);
    heightSpin->setRange(0, MAX_OUTPUT_HEIGHT);
    heightSpin->setValue(this->filter->parameter(filter_output_gate_c::PARAM_HEIGHT));

    QFormLayout *l = new QFormLayout(this->widget);
    l->addRow(widthLabel, widthSpin);
    l->addRow(heightLabel, heightSpin);

    connect(widthSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int newValue)
    {
        this->set_parameter(filter_output_gate_c::PARAM_WIDTH, newValue);
    });

    connect(heightSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int newValue)
    {
        this->set_parameter(filter_output_gate_c::PARAM_HEIGHT, newValue);
    });

    this->widget->adjustSize();

    return;
}
