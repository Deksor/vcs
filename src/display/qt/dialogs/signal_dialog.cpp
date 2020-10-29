/*
 * 2018 Tarpeeksi Hyvae Soft /
 * VCS video & color dialog
 *
 * A GUI dialog for allowing the user to adjust the capture card's video and color
 * parameters.
 *
 */

#include <QElapsedTimer>
#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>
#include <QToolBar>
#include <QMenuBar>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include "display/qt/subclasses/QTableWidget_property_table.h"
#include "display/qt/dialogs/signal_dialog.h"
#include "display/qt/persistent_settings.h"
#include "common/propagate/app_events.h"
#include "display/qt/utility.h"
#include "display/display.h"
#include "capture/capture_api.h"
#include "capture/capture.h"
#include "common/disk/disk.h"
#include "ui_signal_dialog.h"

/*
 * TODOS:
 *
 * - the dialog should communicate more clearly which mode the current GUI controls
 *   are editing, and which modes are available or known about.
 *
 */

static const QString BASE_WINDOW_TITLE = "VCS - Signal Info";

// Used to keep track of how long we've had a particular video mode set.
static QElapsedTimer VIDEO_MODE_UPTIME;
static QTimer INFO_UPDATE_TIMER;

// How many captured frames we've had to drop. This count is shown to the user
// in the signal dialog.
static unsigned NUM_DROPPED_FRAMES = 0;

// How many captured frames the capture API has had to drop, in total. We'll
// use this value to derive NUM_DROPPED_FRAMES.
static unsigned GLOBAL_NUM_DROPPED_FRAMES = 0;

SignalDialog::SignalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignalDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(BASE_WINDOW_TITLE);

    // Don't show the context help '?' button in the window bar.
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Set the GUI controls to their proper initial values.
    {
        // Initialize the table of information. Note that this also sets
        // the vertical order in which the table's parameters are shown.
        {
            ui->tableWidget_propertyTable->modify_property("Input channel", "No signal");
            ui->tableWidget_propertyTable->modify_property("Uptime",        "-");
            ui->tableWidget_propertyTable->modify_property("Resolution",    "-");
            ui->tableWidget_propertyTable->modify_property("Refresh rate",  "-");
            ui->tableWidget_propertyTable->modify_property("Frames dropped", "-");
        }

        // Start timers to keep track of the video mode's uptime and dropped
        // frames count.
        {
            VIDEO_MODE_UPTIME.start();
            INFO_UPDATE_TIMER.start(1000);

            GLOBAL_NUM_DROPPED_FRAMES = kc_capture_api().get_missed_frames_count();

            connect(&INFO_UPDATE_TIMER, &QTimer::timeout, [this]
            {
                // Update missed frames count.
                {
                    NUM_DROPPED_FRAMES += (kc_capture_api().get_missed_frames_count() - GLOBAL_NUM_DROPPED_FRAMES);
                    GLOBAL_NUM_DROPPED_FRAMES = kc_capture_api().get_missed_frames_count();

                    ui->tableWidget_propertyTable->modify_property("Frames dropped", QString::number(NUM_DROPPED_FRAMES));
                }

                // Update uptime.
                {
                    const unsigned seconds = unsigned(VIDEO_MODE_UPTIME.elapsed() / 1000);
                    const unsigned minutes = (seconds / 60);
                    const unsigned hours   = (minutes / 60);

                    if (kc_capture_api().has_no_signal())
                    {
                        ui->tableWidget_propertyTable->modify_property("Uptime", "-");
                    }
                    else
                    {
                        ui->tableWidget_propertyTable->modify_property("Uptime", QString("%1:%2:%3").arg(QString::number(hours).rightJustified(2, '0'))
                                                                                                    .arg(QString::number(minutes % 60).rightJustified(2, '0'))
                                                                                                    .arg(QString::number(seconds % 60).rightJustified(2, '0')));
                    }
                }
            });
        }
    }

    // Restore persistent settings.
    {
        this->resize(kpers_value_of(INI_GROUP_GEOMETRY, "signal", this->size()).toSize());
    }

    // Connect the GUI controls to consequences for changing their values.
    {
        connect(ui->pushButton_resetMissedFramesCount, &QPushButton::clicked, this, [this]
        {
            NUM_DROPPED_FRAMES = 0;
            ui->tableWidget_propertyTable->modify_property("Frames dropped", "0");
        });
    }

    // Subscribe to app events.
    {
        const auto update_info = [this]
        {
            if (kc_capture_api().has_signal())
            {
                VIDEO_MODE_UPTIME.restart();
            }

            update_information_table(kc_capture_api().has_signal());
        };

        ke_events().capture.invalidSignal.subscribe([this]
        {
            this->set_controls_enabled(false);
        });

        ke_events().capture.signalLost.subscribe([this]
        {
            this->set_controls_enabled(false);
        });

        ke_events().capture.signalGained.subscribe([this]
        {
            this->set_controls_enabled(true);
        });

        ke_events().capture.newVideoMode.subscribe(update_info);

        ke_events().capture.newInputChannel.subscribe(update_info);
    }

    return;
}

SignalDialog::~SignalDialog()
{
    // Save persistent settings.
    {
        kpers_set_value(INI_GROUP_GEOMETRY, "signal", this->size());
    }

    delete ui;
    ui = nullptr;

    return;
}

void SignalDialog::set_controls_enabled(const bool state)
{
    update_information_table(state);

    return;
}

void SignalDialog::update_information_table(const bool isReceivingSignal)
{
    const auto inputChannelIdx = (kc_capture_api().get_input_channel_idx() + 1);

    ui->tableWidget_propertyTable->modify_property("Input channel", QString::number(inputChannelIdx));

    if (isReceivingSignal)
    {
        const resolution_s resolution = kc_capture_api().get_resolution();
        const auto refreshRate = kc_capture_api().get_refresh_rate().value<double>();

        ui->tableWidget_propertyTable->modify_property("Refresh rate", QString("%1 Hz").arg(QString::number(refreshRate, 'f', 3)));
        ui->tableWidget_propertyTable->modify_property("Resolution", QString("%1 x %2").arg(resolution.w)
                                                                                       .arg(resolution.h));
    }
    else
    {
        ui->tableWidget_propertyTable->modify_property("Resolution", "-");
        ui->tableWidget_propertyTable->modify_property("Refresh rate", "-");
    }

    return;
}
