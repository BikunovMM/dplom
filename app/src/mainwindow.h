#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstdio>
#include <cstdlib>

#include <future>
#include <thread>
#include <chrono>
#include <functional>
#include <string>

#include <QMainWindow>
#include <QFont>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QStringListModel>
#include <QListView>
#include <QLineEdit>
#include <QComboBox>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QStandardItemModel>
#include <QTableView>
#include <QFileDialog>
#include <QFont>
#include <QHeaderView>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QTranslator>

#include "utils.h"
#include "converter.h"
#include "ui/slicedbtn.h"
#include "ui/traplabel.h"
#include "ui/skipbtn.h"
#include "ui/playstopbtn.h"
#include "ui/sliderstyle.h"
#include "ui/line.h"
#include "ui/showhidebtn.h"
#include "serverrequester.h"
#include "sessionsmanager.h"

#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

#define MIN_LOGIN_LEN 3
#define MAX_LOGIN_LEN 32

#define MIN_PASSWORD_LEN 6
#define MAX_PASSWORD_LEN 32

#define MIN_EMAIL_LEN 5
#define MAX_EMAIL_LEN 256

constexpr char *MAIN_WIN_TITLE = (char*)"converter";
constexpr int   MAIN_WIN_MIN_W = 800;
constexpr int   MAIN_WIN_MIN_H = 450;

class MainWindow : public QWidget
{
    Q_OBJECT    

public:
    explicit MainWindow(QTranslator *trans,
                        const QString& lang,
                        QWidget *parent = nullptr);
    ~MainWindow();

private:    
    void uplFile();
    void downlFile();

    void setDefFn();
    void setRandFn();

    void openPage(int ind);

    void openLogWin();
    void openRegWin();

    void playStopMedia();
    void skipMediaTime(int millisecs);

    void playingChanged(bool playing);
    void mediaPosChanged(qint64 position);

    void tlSliderMoved(int val);
    void tlSliderPressed();

    void outFmtChanged();
    void vidCodecChanged();
    void audCodecChanged();

    void openProfWin();
    void openSettWin();

    void validateRegData(QLineEdit *log_ed, QLineEdit *pass_ed,
                         QLineEdit *email_ed, QLabel *err_log_lb,
                         QLabel *err_pass_lb, QLabel *err_email_lb,
                         QDialog *diag);
    void validateLogData(QLineEdit *log_ed, QLineEdit *pass_ed,
                         QLabel *err_log_lb, QLabel *err_pass_lb,
                         QDialog *diag);

    int regUser(QLineEdit *log_ed, QLineEdit *pass_ed,
                QLineEdit *email_ed);
    int logUser(QLineEdit *log_ed, QLineEdit *pass_ed);

    void convert();
    void paramsToDef();

    void logoutUser(QDialog *diag);

    void confirmEmail(QLineEdit *log_ed, QLineEdit *pass_ed,
                      QLineEdit *email_ed, QDialog *reg_diag,
                      QDialog *diag);

    void showBanner(unsigned char *data, size_t len);
    void changeLanguage(QComboBox *lang_cbox,
                        QDialog *diag,
                        QLabel *lb,
                        QPushButton *btn,
                        TrapLabel *tlb);

    /* DATA */
    conv::FmtData  fmtdata;
    conv::ConvData convdata;
    QString upl_path;
    QString upl_fn;
    QString downl_path;
    std::future<void> conv_fr;
    QFuture<void> convfr;

    QTranslator *translator;
    QString language;


    /* MAIN_LAYOUTS */
    QGridLayout    *main_grid;
    QHBoxLayout    *header_box;
    QVBoxLayout    *page_btns_box;
    QStackedWidget *pages_stw;

    /* HEADER_BOX */
    QLabel      *header_logo_lb;
    QPixmap      header_pixmap;
    //QVBoxLayout *auth_btns_box;
    QGridLayout *auth_btns_grid;

    /* AUTH_BTNS_BOX */
    QPushButton *log_btn;
    QPushButton *reg_btn;
    QPushButton *prof_btn;
    QPushButton *sett_btn;

    /* PAGES_BTNS_BOX */
    QPushButton *page0_btn;
    QPushButton *page1_btn;
    QPushButton *page2_btn;

    /* PAGE WIDGETS */
    QWidget *page0;
    QWidget *page1;
    QWidget *page2;

    /* PAGE0 */
    QGridLayout      *p0_grid;

    SlicedBtn        *p0_upl_downl_sbtn;

    QStringList       p0_fn_list;
    QStringListModel *p0_fn_lmodel;
    QListView        *p0_fn_view;
    QGridLayout      *p0_upl_fn_grid;
    SlicedBtn        *p0_clear_rm_sbtn;

    QGridLayout      *p0_fn_grid;
    QLineEdit        *p0_fn_edit;
    QComboBox        *p0_fn_cbox;
    SlicedBtn        *p0_fn_def_rand_sbtn;

    /* PAGE1 */
    QGridLayout        *p1_grid;

    QAudioOutput       *p1_aout;
    QMediaPlayer       *p1_mplayer;
    QVideoWidget       *p1_vwidget;

    QGridLayout        *p1_media_grid;
    QHBoxLayout        *p1_btns_box;
    QHBoxLayout        *p1_tms_box;
    QSlider            *p1_tl_sl;
    QLabel             *p1_curtm_lb;
    QLabel             *p1_dur_lb;
    SkipBtn            *p1_rew_sbtn;
    SkipBtn            *p1_ff_sbtn;
    PlayStopBtn        *p1_psbtn;
    QLabel             *p1_img;

    QStandardItemModel *p1_sti_model;
    QTableView         *p1_tview;

    /* PAGE2 */
    QGridLayout *p2_grid;
    QGridLayout *p2_left_grid;
    QGridLayout *p2_right_grid;

    QLabel      *p2_vcodec_lb;
    QComboBox   *p2_vcodec_cbox;
    QLabel      *p2_pixfmt_lb;
    QComboBox   *p2_pixfmt_cbox;

    QLabel      *p2_acodec_lb;
    QComboBox   *p2_acodec_cbox;
    QLabel      *p2_smplfmt_lb;
    QComboBox   *p2_smplfmt_cbox;
    QLabel      *p2_smplrate_lb;
    QComboBox   *p2_smplrate_cbox;
    QLabel      *p2_bitrate_lb;
    QComboBox   *p2_bitrate_cbox;
    QLabel      *p2_nbchannels_lb;
    QComboBox   *p2_nbchannels_cbox;

    QPushButton *p2_def_btn;
};

#endif /* MAINWINDOW_H */
