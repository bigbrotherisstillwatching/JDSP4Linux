/*
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  ThePBone <tim.schneeberger(at)outlook.de> (c) 2020
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QFrame>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "interface/fragment/AppManagerFragment.h"
#include "interface/fragment/FragmentHost.h"
#include "data/PresetProvider.h"

class IAudioService;
class AppConfig;
class EELParser;
class ConfigContainer;
class StyleHelper;
class DBusProxy;
class OverlayMsgProxy;
class EELEditor;
class PresetFragment;
class SettingsFragment;
class AudioStreamEngine;
class Spectrograph;
class QJsonTableModel;
class TrayIcon;
class QVBoxLayout;
class AudioManager;
class SingleInstanceMonitor;
class StatusFragment;

using namespace std;
namespace Ui
{
	class MainWindow;
}

class MainWindow :
	public QMainWindow
{
	Q_OBJECT
	enum class Context;

public:
	explicit MainWindow(QString  exepath,
	                    bool     statupInTray,
                        bool     allowMultipleInst,
	                    QWidget *parent = nullptr);
	~MainWindow();

	static void loadPresetFile(const QString &);
	static void savePresetFile(const QString &);
	QString     executablePath();
	void        setEq(const QVector<double> &data);
	void        setReverbData(PresetProvider::Reverb::sf_reverb_preset_data data);
	void        setEqMode(int mode);
	void        updateDDCSelection();

    Ui::MainWindow *ui;

    EELEditor *eelEditor() const;

protected:
    void        resizeEvent(QResizeEvent* event) override;
    void        closeEvent(QCloseEvent *event) override;

public slots:
	void        reset();
	void        restart();
	void        raiseWindow();
	void        applyConfig();
	void        trayIconActivated();
	int         extractDefaultEelScripts(bool allowOverride,
	                                     bool user);
	void        launchFirstRunSetup();

private slots:
	void        disableFx();
    void        onUpdate();
	void        resetEQ();
	void        eqPresetSelectionUpdated();
	void        updateUnitLabel(int,
	                            QObject *alt = nullptr);
	void        updateAllUnitLabels();
	void        loadExternalFile();
	void        saveExternalFile();
    void        dialogHandler();
	void        updateEqStringFromWidget();
	void        bs2bPresetSelectionUpdated();
	void        reverbPresetSelectionUpdated();
	void        reloadDDC();
	void        reloadIRS();
	void        reloadIRSFav();
	void        reloadLiveprog();
	void        restoreGraphicEQView();
	void        saveGraphicEQView();
	void        updateEQMode();
	void        fireTimerSignal();
	void        resetLiveprogParams();
	void        updateFromEelEditor(QString path);
	void        convolverWaveformEdit();

private:
	StyleHelper *m_stylehelper;
	QTimer *refreshTick;

	bool m_startupInTraySwitch;
	TrayIcon *trayIcon;

	QAction *spectrum;
	EELParser *_eelparser;

    SingleInstanceMonitor* _singleInstance;

    FragmentHost<AppManagerFragment*>* appMgrFragment = nullptr;
    FragmentHost<StatusFragment*>* statusFragment = nullptr;
    FragmentHost<SettingsFragment*>* settingsFragment = nullptr;
    FragmentHost<PresetFragment*>* presetFragment = nullptr;

	EELEditor *m_eelEditor;

	QScopedPointer<QFrame> analysisLayout;
	Spectrograph *m_spectrograph;
	AudioStreamEngine *m_audioengine;

    IAudioService* audioService = nullptr;

	bool spectrumReloadSignalQueued   = false;
	bool lockapply                    = false;
	bool lockddcupdate                = false;
	bool lockirsupdate                = false;
	bool lockliveprogupdate           = false;

	QString activeirs                 = "";
	QString activeddc                 = "";
	QString activeliveprog            = "";
	QString irAdvancedWaveformEditing = "";

	QJsonTableModel *model;

	void updateTooltipLabelUnit(QObject       *sender,
	                            const QString &text,
	                            bool);
	void loadConfig();
	void connectActions();

	void reloadDDCDB();
	void setLiveprogSelection(QString path);

	void updateIrsSelection();

};

#endif // MAINWINDOW_H
