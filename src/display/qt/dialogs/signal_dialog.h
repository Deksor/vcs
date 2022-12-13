/*
 * 2018 Tarpeeksi Hyvae Soft /
 * VCS
 *
 */

#ifndef VCS_DISPLAY_QT_DIALOGS_SIGNAL_DIALOG_H
#define VCS_DISPLAY_QT_DIALOGS_SIGNAL_DIALOG_H

#include "display/qt/subclasses/QDialog_vcs_base_dialog.h"

class QMenuBar;

namespace Ui {
class SignalDialog;
}

struct capture_video_settings_s;
struct capture_color_settings_s;
struct resolution_s;

class SignalDialog : public VCSBaseDialog
{
    Q_OBJECT

public:
    explicit SignalDialog(QWidget *parent = 0);
    ~SignalDialog(void);

    void set_controls_enabled(const bool state);

    void update_controls(void);

private:
    void update_information_table(const bool isReceivingSignal);

    Ui::SignalDialog *ui;
};

#endif
