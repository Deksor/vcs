/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#include <cmath>
#include "filter/filters/decimate/filter_decimate.h"
#include "filter/filters/decimate/gui/qt/filtergui_decimate.h"

filtergui_decimate_c::filtergui_decimate_c(filter_c *const filter) : filtergui_c(filter)
{
    // Factor.
    QLabel *factorLabel = new QLabel("Factor", this->widget);
    QComboBox *factorList = new QComboBox(this->widget);
    factorList->addItem("2");
    factorList->addItem("4");
    factorList->addItem("8");
    factorList->addItem("16");
    factorList->setCurrentIndex((std::round(std::sqrt(this->filter->parameter(filter_decimate_c::PARAM_FACTOR))) - 1));

    // Sampling.
    QLabel *radiusLabel = new QLabel("Sampling", this->widget);
    QComboBox *samplingList = new QComboBox(this->widget);
    samplingList->addItem("Nearest");
    samplingList->addItem("Average");
    samplingList->setCurrentIndex(this->filter->parameter(filter_decimate_c::PARAM_TYPE));

    QFormLayout *l = new QFormLayout(this->widget);
    l->addRow(factorLabel, factorList);
    l->addRow(radiusLabel, samplingList);

    connect(factorList, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), [this](const QString &newIndexText)
    {
        this->set_parameter(filter_decimate_c::PARAM_FACTOR, newIndexText.toUInt());
    });

    connect(samplingList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](const int newIndex)
    {
        this->set_parameter(filter_decimate_c::PARAM_TYPE, newIndex);
    });

    this->widget->adjustSize();

    return;
}
