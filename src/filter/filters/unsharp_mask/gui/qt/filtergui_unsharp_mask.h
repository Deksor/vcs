/*
 * 2021 Tarpeeksi Hyvae Soft
 *
 * Software: VCS
 *
 */

#ifndef VCS_FILTER_FILTERS_UNSHARP_MASK_GUI_FILTERGUI_UNSHARP_MASK_H
#define VCS_FILTER_FILTERS_UNSHARP_MASK_GUI_FILTERGUI_UNSHARP_MASK_H

#include "filter/filters/qt_filtergui.h"

class filtergui_unsharp_mask_c : public filtergui_c
{
    Q_OBJECT

public:
    filtergui_unsharp_mask_c(filter_c *const filter);
};

#endif
