#include "mainwindow.h"

MainWindow::MainWindow(QTranslator *trans,
                       const QString& lang,
                       QWidget *parent)
    : QWidget(parent)
{
    //QFont webdings_ft("Webdings", QFont::Normal);

    //
    //  MAIN_LAYOUTS
    //

    main_grid = new QGridLayout(this);

    //
    //  HEADER
    //

    header_box = new QHBoxLayout();
    header_box->setContentsMargins(8, 0, 0, 0);
    main_grid->addLayout(header_box, 0, 0, 1, 2);

    header_logo_lb = new QLabel();
    header_logo_lb->setMaximumSize(60, 60);
    header_box->addWidget(header_logo_lb, 0);
    header_box->setAlignment(header_logo_lb,
                             Qt::AlignLeft | Qt::AlignBottom);

    header_pixmap = QPixmap(":/images/app_logo.png");
    header_logo_lb->setPixmap(
        header_pixmap.scaled(header_logo_lb->size(),
                             Qt::KeepAspectRatio,
                             Qt::FastTransformation));

    //auth_btns_box = new QVBoxLayout();
    //header_box->addLayout(auth_btns_box, 0);
    //header_box->setAlignment(auth_btns_box,
    //                         Qt::AlignRight | Qt::AlignBottom);

    auth_btns_grid = new QGridLayout();
    header_box->addLayout(auth_btns_grid, 0);

    //
    //  PAGES_PANELS
    //

    page_btns_box = new QVBoxLayout();
    main_grid->addLayout(page_btns_box, 1, 0);

    QPalette pages_stw_pal;
    pages_stw_pal.setColor(QPalette::Window,
                           QColor(220, 220, 220, 255));

    pages_stw = new QStackedWidget();
    pages_stw->setPalette(pages_stw_pal);
    pages_stw->setAutoFillBackground(true);
    main_grid->addWidget(pages_stw, 1, 1);

    //
    //  AUTH_BTNS_BOX
    //

    log_btn = new QPushButton(tr("login")); //"login"
    log_btn->setMinimumWidth(100);
    log_btn->setMaximumWidth(100);
    connect(log_btn, &QPushButton::clicked,
            this, &MainWindow::openLogWin);
    //auth_btns_box->addWidget(log_btn);
    auth_btns_grid->addWidget(log_btn, 0, 1, 1, 1);

    reg_btn = new QPushButton(tr("registration"));  //"register"
    reg_btn->setMinimumWidth(100);
    reg_btn->setMaximumWidth(100);
    connect(reg_btn, &QPushButton::clicked,
            this, &MainWindow::openRegWin);
    //auth_btns_box->addWidget(reg_btn);
    auth_btns_grid->addWidget(reg_btn, 1, 1, 1, 1);

    prof_btn = new QPushButton(tr("profile"));  //"profile"
    prof_btn->setMinimumWidth(100);
    prof_btn->setMaximumWidth(100);
    connect(prof_btn, &QPushButton::clicked,
            this, &MainWindow::openProfWin);
    //auth_btns_box->addWidget(prof_btn);
    auth_btns_grid->addWidget(prof_btn, 0, 0, 1, 1);

    sett_btn = new QPushButton(tr("settings")); //"settings"
    sett_btn->setMinimumWidth(100);
    sett_btn->setMaximumWidth(100);
    connect(sett_btn, &QPushButton::clicked,
            this, &MainWindow::openSettWin);
    //auth_btns_box->addWidget(sett_btn);
    auth_btns_grid->addWidget(sett_btn, 1, 0, 1, 1);

    if (SessionsManager::containsSession()) {
        char token[ServerRequester::TOKEN_STR_LEN] = {};

        SessionsManager::getToken(token);

        int ret = ServerRequester::validateToken(token);
        if (ret != ServerRequester::HTTP_200_INT) {
            //SessionsManager::deleteSession();
            prof_btn->setVisible(false);
        }
        else {
            log_btn->setVisible(false);
            reg_btn->setVisible(false);

            /* todo: ServerRequester::validateToken(...); */
            log_str("[*][MainWindow::ctor] loading_cur_session!\n");
        }
    }
    else {
        prof_btn->setVisible(false);
        log_str("[*][MainWindow::ctor] no_sessions_were_found.\n");
    }

    //
    //  PAGE_BTNS_BOX
    //

    page0_btn = new QPushButton(tr("main")); //"main"
    connect(page0_btn, &QPushButton::clicked,
            this, [this](){openPage(0);});
    page_btns_box->addWidget(page0_btn, 0,
                             Qt::AlignTop | Qt::AlignLeft);

    page1_btn = new QPushButton(tr("data")); //"data"
    connect(page1_btn, &QPushButton::clicked,
            this, [this](){openPage(1);});
    page_btns_box->addWidget(page1_btn, 0,
                             Qt::AlignTop | Qt::AlignLeft);

    page2_btn = new QPushButton(tr("parameters")); //"params"
    connect(page2_btn, &QPushButton::clicked,
            this, [this](){openPage(2);});
    page_btns_box->addWidget(page2_btn, 1,
                             Qt::AlignTop | Qt::AlignLeft);

    page1_btn->setVisible(false);
    page2_btn->setVisible(false);

    //
    //  PAGES WIDGETS
    //

    page0 = new QWidget();
    pages_stw->addWidget(page0);

    page1 = new QWidget();
    pages_stw->addWidget(page1);

    page2 = new QWidget();
    pages_stw->addWidget(page2);

    //
    //  PAGE0
    //

    p0_grid = new QGridLayout(page0);
    p0_grid->setRowMinimumHeight(0, 35);
    p0_grid->setColumnStretch(0, 1);
    p0_grid->setColumnStretch(1, 1);
    p0_grid->setHorizontalSpacing(0);

    QFont segoeui12font("Segoe UI", 12);

    p0_upl_downl_sbtn = new SlicedBtn(tr("open"), tr("download"));
    p0_upl_downl_sbtn->setFont(segoeui12font);
    p0_upl_downl_sbtn->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Minimum);
    connect(p0_upl_downl_sbtn, &SlicedBtn::leftClicked,
            this, &MainWindow::uplFile);
    connect(p0_upl_downl_sbtn, &SlicedBtn::rightClicked,
            this, &MainWindow::downlFile);
    p0_grid->addWidget(p0_upl_downl_sbtn, 0, 0, 1, 2);

    //p0_downl_btn = new QPushButton("download");
    //p0_downl_btn->setSizePolicy(QSizePolicy::Expanding,
    //                            QSizePolicy::Minimum);
    //connect(p0_downl_btn, &QPushButton::clicked,
    //        this, &MainWindow::downlFile);
    //p0_grid->addWidget(p0_downl_btn, 0, 1);

    //
    p0_upl_fn_grid = new QGridLayout();
    p0_upl_fn_grid->setContentsMargins(0, 0, 8, 0);
    p0_upl_fn_grid->setVerticalSpacing(0);
    p0_grid->addLayout(p0_upl_fn_grid, 1, 0);    

    p0_fn_list = {"..."};

    p0_fn_lmodel = new QStringListModel(p0_fn_list);

    p0_fn_view = new QListView();
    p0_fn_view->setModel(p0_fn_lmodel);
    p0_fn_view->setAlternatingRowColors(true);
    p0_upl_fn_grid->addWidget(p0_fn_view, 0, 0, 1, 2);

    p0_clear_rm_sbtn = new SlicedBtn(tr("clear"), tr("delete")); //"clear", "remove"
    p0_upl_fn_grid->addWidget(p0_clear_rm_sbtn, 1, 0, 1, 2);

    //

    p0_fn_grid = new QGridLayout();
    p0_fn_grid->setRowStretch(2, 1);
    p0_fn_grid->setContentsMargins(8, 0, 0, 0);
    p0_fn_grid->setSpacing(6);
    p0_grid->addLayout(p0_fn_grid, 1, 1);

    p0_fn_edit = new QLineEdit();
    p0_fn_grid->addWidget(p0_fn_edit, 0, 0);

    p0_fn_cbox = new QComboBox();
    p0_fn_cbox->setMinimumWidth(75);
    p0_fn_cbox->addItem("...");
    connect(p0_fn_cbox, &QComboBox::currentIndexChanged,
            this, &MainWindow::outFmtChanged);
    p0_fn_grid->addWidget(p0_fn_cbox, 0, 1);

    p0_fn_def_rand_sbtn = new SlicedBtn(tr("default"),
                                        tr("random")); //"default", "random"
    connect(p0_fn_def_rand_sbtn, &SlicedBtn::leftClicked,
            this, &MainWindow::setDefFn);
    connect(p0_fn_def_rand_sbtn, &SlicedBtn::rightClicked,
            this, &MainWindow::setRandFn);
    p0_fn_grid->addWidget(p0_fn_def_rand_sbtn, 1, 0, 1, 2);

    //
    //  PAGE1
    //

    p1_grid = new QGridLayout(page1);
    p1_grid->setColumnMinimumWidth(0, 230);
    p1_grid->setColumnStretch(1, 1);
    p1_grid->setContentsMargins(8, 8, 8, 8);
    p1_grid->setVerticalSpacing(6);

    p1_aout = new QAudioOutput;
    p1_aout->setVolume(0.5f);

    p1_mplayer = new QMediaPlayer;
    connect(p1_mplayer, &QMediaPlayer::playingChanged,
            this, &MainWindow::playingChanged);
    connect(p1_mplayer, &QMediaPlayer::positionChanged,
            this, &MainWindow::mediaPosChanged);
    p1_mplayer->setAudioOutput(p1_aout);

    p1_media_grid = new QGridLayout();
    p1_media_grid->setVerticalSpacing(4);
    p1_media_grid->setRowStretch(3, 1);
    p1_grid->addLayout(p1_media_grid, 2, 0, 1, 1,
                       Qt::AlignTop | Qt::AlignHCenter);

    p1_tl_sl = new QSlider(Qt::Horizontal);
    p1_tl_sl->setPageStep(0);
    p1_tl_sl->setStyle(new SliderStyle);
    p1_tl_sl->setMaximumWidth(230);
    connect(p1_tl_sl, &QSlider::sliderMoved,
            this, &MainWindow::tlSliderMoved);
    connect(p1_tl_sl, &QSlider::sliderPressed,
            this, &MainWindow::tlSliderPressed);
    p1_media_grid->addWidget(p1_tl_sl, 0, 0, 1, 1);

    p1_tms_box = new QHBoxLayout();
    p1_media_grid->addLayout(p1_tms_box, 1, 0);

    p1_curtm_lb = new QLabel("00:00");
    p1_tms_box->addWidget(p1_curtm_lb, 0,
                          Qt::AlignVCenter | Qt::AlignLeft);

    p1_dur_lb = new QLabel("00:00");
    p1_tms_box->addWidget(p1_dur_lb, 0,
                          Qt::AlignVCenter | Qt::AlignRight);

    p1_btns_box = new QHBoxLayout();
    p1_media_grid->addLayout(p1_btns_box, 2, 0);

    p1_rew_sbtn = new SkipBtn(false);
    p1_rew_sbtn->setMinimumSize(35, 35);
    p1_rew_sbtn->setMaximumSize(35, 35);
    connect(p1_rew_sbtn, &SkipBtn::clicked,
            this, [this](){skipMediaTime(-10000);});
    p1_btns_box->addWidget(p1_rew_sbtn, 0,
                           Qt::AlignVCenter | Qt::AlignRight);

    p1_psbtn = new PlayStopBtn();
    p1_psbtn->setMinimumSize(40, 40);
    p1_psbtn->setMaximumSize(40, 40);
    connect(p1_psbtn, &PlayStopBtn::clicked,
            this, &MainWindow::playStopMedia);
    p1_btns_box->addWidget(p1_psbtn, 0, Qt::AlignCenter);

    p1_ff_sbtn = new SkipBtn(true);
    p1_ff_sbtn->setMinimumSize(35, 35);
    p1_ff_sbtn->setMaximumSize(35, 35);
    connect(p1_ff_sbtn, &SkipBtn::clicked,
            this, [this](){skipMediaTime(10000);});
    p1_btns_box->addWidget(p1_ff_sbtn, 0,
                           Qt::AlignVCenter | Qt::AlignLeft);

    p1_img = new QLabel();
    p1_img->setMinimumSize(250, 250);
    p1_img->resize(250, 250);
    p1_grid->addWidget(p1_img, 0, 0, 1, 1,
                       Qt::AlignTop | Qt::AlignHCenter);

    p1_vwidget = new QVideoWidget();
    p1_vwidget->setMinimumSize(250, 250);
    p1_vwidget->resize(250, 250);
    p1_mplayer->setVideoOutput(p1_vwidget);
    p1_grid->addWidget(p1_vwidget, 1, 0, 1, 1,
                       Qt::AlignTop | Qt::AlignHCenter);

    p1_sti_model = new QStandardItemModel();

    p1_tview = new QTableView();
    p1_tview->setModel(p1_sti_model);
    p1_tview->setStatusTip(tr("input file\'s metadata"));//"input file\'s metadata"
    p1_tview->setAlternatingRowColors(true);
    p1_tview->setEditTriggers(QAbstractItemView::DoubleClicked);
    p1_grid->addWidget(p1_tview, 0, 1, 3, 1);

    //
    //  PAGE2
    //

    p2_grid = new QGridLayout(page2);
    p2_grid->setHorizontalSpacing(0);
    p2_grid->setVerticalSpacing(0);
    p2_grid->setContentsMargins(16, 16, 16, 16);

    p2_left_grid = new QGridLayout();
    p2_left_grid->setRowStretch(2, 1);
    p2_left_grid->setHorizontalSpacing(8);
    p2_left_grid->setVerticalSpacing(32);
    p2_grid->addLayout(p2_left_grid, 0, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    p2_right_grid = new QGridLayout();
    p2_right_grid->setRowStretch(5, 1);
    p2_right_grid->setHorizontalSpacing(8);
    p2_right_grid->setVerticalSpacing(32);
    p2_grid->addLayout(p2_right_grid, 0, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    /* left_column (video) */
    p2_vcodec_lb = new QLabel(tr("video codec:")); //"video_codec:"
    p2_left_grid->addWidget(p2_vcodec_lb, 0, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    p2_vcodec_cbox = new QComboBox();
    p2_vcodec_cbox->setMinimumWidth(125);
    p2_vcodec_cbox->setMaximumWidth(125);
    connect(p2_vcodec_cbox, &QComboBox::currentIndexChanged,
            this, &MainWindow::vidCodecChanged);
    p2_left_grid->addWidget(p2_vcodec_cbox, 0, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    p2_pixfmt_lb = new QLabel(tr("pixels format:")); //"pixel_format:"
    p2_left_grid->addWidget(p2_pixfmt_lb, 1, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    p2_pixfmt_cbox = new QComboBox();
    p2_pixfmt_cbox->setMinimumWidth(125);
    p2_pixfmt_cbox->setMaximumWidth(125);
    p2_left_grid->addWidget(p2_pixfmt_cbox, 1, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignLeft);

    /* right_column (audio) */
    p2_acodec_lb = new QLabel(tr("audio codec:")); //"audio_codec:"
    p2_right_grid->addWidget(p2_acodec_lb, 0, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_acodec_cbox = new QComboBox();
    p2_acodec_cbox->setMinimumWidth(125);
    p2_acodec_cbox->setMaximumWidth(125);
    connect(p2_acodec_cbox, &QComboBox::currentIndexChanged,
            this, &MainWindow::audCodecChanged);
    p2_right_grid->addWidget(p2_acodec_cbox, 0, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_smplfmt_lb = new QLabel(tr("samples format:")); //"sample_format:"
    p2_right_grid->addWidget(p2_smplfmt_lb, 1, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_smplfmt_cbox = new QComboBox();
    p2_smplfmt_cbox->setMinimumWidth(125);
    p2_smplfmt_cbox->setMaximumWidth(125);
    p2_right_grid->addWidget(p2_smplfmt_cbox, 1, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_smplrate_lb = new QLabel(tr("sample rate:")); //"sample_rate:"
    p2_right_grid->addWidget(p2_smplrate_lb, 2, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_smplrate_cbox = new QComboBox();
    p2_smplrate_cbox->setMinimumWidth(125);
    p2_smplrate_cbox->setMaximumWidth(125);
    p2_right_grid->addWidget(p2_smplrate_cbox, 2, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_bitrate_lb = new QLabel("bit rate:"); //"bit_rate:"
    p2_right_grid->addWidget(p2_bitrate_lb, 3, 0, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    p2_bitrate_cbox = new QComboBox();
    p2_bitrate_cbox->setMinimumWidth(125);
    p2_bitrate_cbox->setMaximumWidth(125);
    p2_right_grid->addWidget(p2_bitrate_cbox, 3, 1, 1, 1,
                       Qt::AlignVCenter | Qt::AlignRight);

    /* */
    p2_nbchannels_lb = new QLabel(tr("channels quantity:")); //"nb_channels:"
    p2_right_grid->addWidget(p2_nbchannels_lb, 4, 0, 1, 1,
                             Qt::AlignVCenter | Qt::AlignRight);

    p2_nbchannels_cbox = new QComboBox();
    p2_nbchannels_cbox->setMinimumWidth(125);
    p2_nbchannels_cbox->setMaximumWidth(125);
    p2_right_grid->addWidget(p2_nbchannels_cbox, 4, 1, 1, 1,
                             Qt::AlignVCenter | Qt::AlignRight);

    p2_def_btn = new QPushButton(tr("default")); //"default"
    p2_def_btn->setObjectName("colored-btn");
    p2_def_btn->setMinimumSize(100, 25);
    p2_def_btn->setMaximumSize(100, 25);
    connect(p2_def_btn, &QPushButton::clicked,
            this, &MainWindow::paramsToDef);
    p2_grid->addWidget(p2_def_btn, 1, 0, 1, 2,
                       Qt::AlignBottom | Qt::AlignHCenter);

    //pages_stw->setCurrentIndex(1);

    //
    //  SETTING SRAND FOR rand() in setRandFn
    //

    unsigned char seed;
    srand((long long)&seed);

    //
    //  WINDOW_STUFF
    //

    this->setWindowTitle(MAIN_WIN_TITLE);
    this->setMinimumSize(MAIN_WIN_MIN_W, MAIN_WIN_MIN_H);
    this->resize(MAIN_WIN_MIN_W, MAIN_WIN_MIN_H);

    translator = trans;
    language = lang;
}

MainWindow::~MainWindow()
{
    /* page2 */
    delete(p2_def_btn);
    delete(p2_nbchannels_cbox);
    delete(p2_nbchannels_lb);
    delete(p2_bitrate_cbox);
    delete(p2_bitrate_lb);
    delete(p2_smplrate_cbox);
    delete(p2_smplrate_lb);
    delete(p2_smplfmt_cbox);
    delete(p2_smplfmt_lb);
    delete(p2_pixfmt_cbox);
    delete(p2_pixfmt_lb);
    delete(p2_acodec_cbox);
    delete(p2_acodec_lb);
    delete(p2_vcodec_cbox);
    delete(p2_vcodec_lb);
    delete(p2_right_grid);
    delete(p2_left_grid);
    delete(p2_grid);
    /* page1 */
    delete(p1_tview);
    delete(p1_sti_model);
    delete(p1_img);
    delete(p1_psbtn);
    delete(p1_ff_sbtn);
    delete(p1_rew_sbtn);
    delete(p1_dur_lb);
    delete(p1_curtm_lb);
    delete(p1_tl_sl);
    delete(p1_btns_box);
    delete(p1_tms_box);
    delete(p1_media_grid);
    delete(p1_vwidget);
    delete(p1_mplayer);
    delete(p1_aout);
    delete(p1_grid);
    /* page0 */
    delete(p0_fn_def_rand_sbtn);
    delete(p0_fn_cbox);
    delete(p0_fn_edit);
    delete(p0_fn_grid);
    delete(p0_clear_rm_sbtn);
    delete(p0_fn_view);
    delete(p0_fn_lmodel);
    delete(p0_upl_fn_grid);
    delete(p0_upl_downl_sbtn);
    delete(p0_grid);
    /* pages */
    delete(page2);
    delete(page1);
    delete(page0);
    /* page_btns_box */
    delete(page2_btn);
    delete(page1_btn);
    delete(page0_btn);
    /* auth_btns_box */
    delete(sett_btn);
    delete(prof_btn);
    delete(reg_btn);
    delete(log_btn);
    /* MAIN_LAYOUTS */
    delete(pages_stw);
    delete(page_btns_box);
    //delete(auth_btns_box);
    delete(auth_btns_grid);
    delete(main_grid);
}

void MainWindow::uplFile()
{
    int ret = 0;

    QString uplpath = QFileDialog::getOpenFileName(this,
                                            tr("Select file"), //"Select file"
                                            "/",
                                            conv::exts_filter);

    if (uplpath.isEmpty()) { /* No files were selected */
        return;
    }

    upl_path = uplpath;

    if (p1_mplayer->isPlaying()) {
        p1_mplayer->stop();
        p1_psbtn->setState(false);
    }    

    //
    //  SETTNG p0_fn_edit text
    //

    qsizetype fn_pos = upl_path.lastIndexOf('/') + 1;
    qsizetype ext_pos = upl_path.lastIndexOf('.');

    upl_fn = upl_path.mid(fn_pos, ext_pos - fn_pos);

    p0_fn_edit->setText(upl_fn);

    //
    //  GETTING SUPP EXTENSIONS
    //

    /* pointer will be destroyed after uplFile's end, but it's ok */
    const char *full_fn = upl_path.mid(fn_pos).toStdString().c_str();

    ret = conv::getFmtData(full_fn, &fmtdata);
    if (ret < 0) {
        log_err("[!] [uplFile] Failed to conv::getFmtData.\n");
        return;
    }

    p0_fn_cbox->clear();

    for (int i = 0; i < fmtdata.exts_len; ++i) {
        p0_fn_cbox->addItem(fmtdata.exts[i]);
    }

    //
    //  ADDING FILENAME(S) TO THE p0_fn_lmodel
    //

    p0_fn_lmodel->setData(p0_fn_lmodel->index(0, 0),
                          upl_path);

    //
    //  GETTING AND DISPLAYING INPUT FILE'S METADATA
    //

    conv::FileData filedata;
    ret = conv::getFileData(upl_path.toStdString().c_str(), &filedata);
    if (ret < 0) {
        log_err("[!] [uplFile] Failed to conv::getFileData/\n");
        return;
    }

    QList<QStandardItem*> keys_list;
    QList<QStandardItem*> values_list;

    for (const std::pair<const std::string, const std::string>& pair : filedata.metadata) {
        QStandardItem *head_item = new QStandardItem(pair.first.c_str());
        head_item->setToolTip(pair.first.c_str());
        keys_list.append(head_item);

        QStandardItem *val_item = new QStandardItem(pair.second.c_str());
        val_item->setToolTip(pair.second.c_str());
        values_list.append(val_item);
    }

    p1_sti_model->clear();
    p1_sti_model->appendColumn(keys_list);
    p1_sti_model->appendColumn(values_list);

    p1_sti_model->setHeaderData(0, Qt::Horizontal, "Keys"); //"Keys"
    p1_sti_model->setHeaderData(1, Qt::Horizontal, "Values"); //"Values"
    p1_tview->horizontalHeader()->setStretchLastSection(true);

    if (p1_tview->isHidden()) {
        p1_tview->show();
    }

    //
    // Showing page1 and page2 buttons
    //

    if (page1_btn->isHidden()) {
        page1_btn->setVisible(true);
        page2_btn->setVisible(true);
    }

    //
    //  DISPLAYING MEDIA_GRID WIDGETS FOR FILE'S TYPE
    //

    QPixmap p1_pixmap;

    switch (fmtdata.type) {
    case conv::MediaType::IMG:
        //
        //  PAGE1 WIDGETS
        //
        p2_vcodec_lb->setVisible(true);
        p2_vcodec_cbox->setVisible(true);
        p2_pixfmt_lb->setVisible(true);
        p2_pixfmt_cbox->setVisible(true);

        p2_acodec_lb->setVisible(false);
        p2_acodec_cbox->setVisible(false);
        p2_smplfmt_lb->setVisible(false);
        p2_smplfmt_cbox->setVisible(false);
        p2_smplrate_lb->setVisible(false);
        p2_smplrate_cbox->setVisible(false);
        p2_bitrate_lb->setVisible(false);
        p2_bitrate_cbox->setVisible(false);
        p2_nbchannels_lb->setVisible(false);
        p2_nbchannels_cbox->setVisible(false);

        p1_psbtn->setVisible(false);
        p1_ff_sbtn->setVisible(false);
        p1_rew_sbtn->setVisible(false);
        p1_curtm_lb->setVisible(false);
        p1_tl_sl->setVisible(false);
        p1_dur_lb->setVisible(false);
        p1_img->setVisible(true);
        p1_vwidget->setVisible(false);

        p1_pixmap = QPixmap(upl_path);
        p1_img->setPixmap(p1_pixmap.scaled(p1_img->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::FastTransformation));
        p1_img->setToolTip(upl_path);
        break;
    case conv::MediaType::VID:
        //
        //  PAGE1 WIDGETS
        //

        p1_img->setVisible(false);
        p1_vwidget->setVisible(true);

        //
        //  PAGE2 WIDGETS
        //

        p2_vcodec_lb->setVisible(true);
        p2_vcodec_cbox->setVisible(true);
        p2_pixfmt_lb->setVisible(true);
        p2_pixfmt_cbox->setVisible(true);

        if (0) {
    default: /* conv::MediaType::AUD */
        p1_img->setVisible(true);
        p1_vwidget->setVisible(false);

        p2_vcodec_lb->setVisible(false);
        p2_vcodec_cbox->setVisible(false);
        p2_pixfmt_lb->setVisible(false);
        p2_pixfmt_cbox->setVisible(false);

        p1_pixmap = QPixmap(":/images/audio_icon.png");
        p1_img->setPixmap(p1_pixmap.scaled(p1_img->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::FastTransformation));
        p1_img->setToolTip(upl_path);
        }

        //
        //  PAGE1 WIDGETS
        //

        p1_psbtn->setVisible(true);
        p1_ff_sbtn->setVisible(true);
        p1_rew_sbtn->setVisible(true);
        p1_curtm_lb->setVisible(true);
        p1_tl_sl->setVisible(true);
        p1_dur_lb->setVisible(true);

        //
        //  PAGE2 WIDGETS
        //

        p2_acodec_lb->setVisible(true);
        p2_acodec_cbox->setVisible(true);
        p2_smplfmt_lb->setVisible(true);
        p2_smplfmt_cbox->setVisible(true);
        p2_smplrate_lb->setVisible(true);
        p2_smplrate_cbox->setVisible(true);
        p2_bitrate_lb->setVisible(true);
        p2_bitrate_cbox->setVisible(true);
        p2_nbchannels_lb->setVisible(true);
        p2_nbchannels_cbox->setVisible(true);

        const int64_t dur = filedata.duration / 1000;
        const int64_t mins = dur / 60;
        const int64_t secs = dur % 60;
        char buf[8] = {0};

        p1_mplayer->setSource(QUrl(upl_path));
        p1_tl_sl->setRange(0, (int)dur);
        p1_tl_sl->setSingleStep(1);
        p1_tl_sl->setPageStep(0);

        snprintf(buf, sizeof(buf),
                 "%02" PRId64 ":%02" PRId64, mins, secs);

        p1_dur_lb->setText(buf);
    }

    //
    //  GETTING PAGE2 CBOXES PARAMS
    //

    /*

    if (fmtdata.type == conv::MediaType::IMG) {
        conv::ImgData *idata = conv::getImgParams(fmtdata);

        for (int i = 0; i < idata->len; ++i) {
            conv::VidCodec *codec = &idata->codecs[i];

            p2_vcodec_cbox->addItem(codec->name);

            for (int j = 0; j < codec->pixfmts_len; ++j) {
                p2_pixfmt_cbox->addItem(codec->pixfmts[j].name);
            }
        }
    }
    else if (fmtdata.type == conv::MediaType::AUD) {
        conv::AudData *adata = conv::getAudParams(fmtdata);

        for (int i = 0; i < adata->len; ++i) {
            conv::AudCodec *codec = &adata->codecs[i];

            p2_acodec_cbox->addItem(codec->name);

            for (int j = 0; j < codec->)
        }
    }
    else {

    }
    */


}

//static inline void



void MainWindow::downlFile()
{
    if (upl_path.isEmpty()) {
        log_str("upl_path is empty!\n");
        return;
    }

    QString downl_fn = p0_fn_edit->text();

    if (fmtdata.type == conv::MediaType::IMG) {
        downl_path = QFileDialog::getSaveFileName(this,
                                                 "Выберите путь", //"Select path:",
                                                 "/" + downl_fn + "." + p0_fn_cbox->currentText(),
                                                 conv::img_filter);
    }
    else if (fmtdata.type == conv::MediaType::VID) {
        downl_path = QFileDialog::getSaveFileName(this,
                                                 "Выберите путь", //"Select path:",
                                                 "/" + downl_fn + "." + p0_fn_cbox->currentText(),
                                                 conv::vid_filter);
    }
    else { /* conv::MediaType::AUD */
        downl_path = QFileDialog::getSaveFileName(this,
                                                 "Выберите путь", //"Select path:",
                                                 "/" + downl_fn + "." + p0_fn_cbox->currentText(),
                                                 conv::aud_filter);
    }

    if (downl_path.isEmpty()) {
        return;
    }

    //
    //  GETTING DATA FROM PAGE2's CBOXES
    //

    if (fmtdata.type == conv::MediaType::IMG) {
        convdata.acodec      = AV_CODEC_ID_NONE;
        convdata.smplfmt     = AV_SAMPLE_FMT_NONE;
        convdata.smplrate    = 0;
        convdata.bitrate     = 0;
        convdata.nb_channels = 0;
        convdata.frame_size  = 0;

        const conv::ImgData *idata =
            conv::getImgParams(p0_fn_cbox->currentIndex());
        const conv::VidCodec *vcodec =
            &idata->codecs[p2_vcodec_cbox->currentIndex()];
        const conv::PixFmt *pixfmt =
            &vcodec->pixfmts[p2_pixfmt_cbox->currentIndex()];

        convdata.vcodec    = vcodec->id;
        convdata.pixfmt    = pixfmt->id;
        convdata.max_width = idata->max_width;
    }
    else if (fmtdata.type == conv::MediaType::VID) {
        convdata.max_width = 0;

        const conv::VidData *vdata =
            conv::getVidParams(p0_fn_cbox->currentIndex());

        const conv::VidCodec *vcodec =
            &vdata->vcodecs[p2_vcodec_cbox->currentIndex()];
        const conv::PixFmt *pixfmt =
            &vcodec->pixfmts[p2_pixfmt_cbox->currentIndex()];

        convdata.vcodec    = vcodec->id;
        convdata.pixfmt    = pixfmt->id;

        const conv::AudCodec *acodec =
            &vdata->acodecs[p2_acodec_cbox->currentIndex()];
        const conv::SmplFmt  *smplfmt =
            &acodec->smplfmts[p2_smplfmt_cbox->currentIndex()];
        const conv::SmplRate *smplrate =
            &acodec->smplrates[p2_smplrate_cbox->currentIndex()];
        const conv::BitRate  *bitrate =
            &acodec->bitrates[p2_bitrate_cbox->currentIndex()];
        const conv::NbChannels *nb_channels =
            &acodec->nb_channels[p2_nbchannels_cbox->currentIndex()];

        convdata.acodec = acodec->id;
        convdata.smplfmt = smplfmt->id;
        convdata.smplrate = smplrate->val;
        convdata.bitrate = bitrate->val;
        convdata.nb_channels = nb_channels->val;
        convdata.frame_size = acodec->frame_size;
    }
    else { /* conv::MediaType::AUD */
        convdata.vcodec    = AV_CODEC_ID_NONE;
        convdata.pixfmt    = AV_PIX_FMT_NONE;
        convdata.max_width = 0;

        const conv::AudData *adata =
            conv::getAudParams(p0_fn_cbox->currentIndex());

        const conv::AudCodec *acodec = &adata->codecs[p2_acodec_cbox->currentIndex()];
        const conv::SmplFmt  *smplfmt =
            &acodec->smplfmts[p2_smplfmt_cbox->currentIndex()];
        const conv::SmplRate *smplrate =
            &acodec->smplrates[p2_smplrate_cbox->currentIndex()];
        const conv::BitRate  *bitrate =
            &acodec->bitrates[p2_bitrate_cbox->currentIndex()];
        const conv::NbChannels *nb_channels =
            &acodec->nb_channels[p2_nbchannels_cbox->currentIndex()];

        convdata.acodec = acodec->id;
        convdata.smplfmt = smplfmt->id;
        convdata.smplrate = smplrate->val;
        convdata.bitrate = bitrate->val;
        convdata.nb_channels = nb_channels->val;
        convdata.frame_size = acodec->frame_size;
    }

    convfr = QtConcurrent::run(&MainWindow::convert, this);
    //launchConvert();

    //convfr = QtConcurrent::run(this, &MainWindow::convert);

    //conv_fr = std::async(std::launch::async, [this](){convert();});
}

void MainWindow::convert()
{
    std::string inpath_str = upl_path.toStdString();
    std::string outpath_str = downl_path.toStdString();
    const char *inpath  = inpath_str.c_str();
    const char *outpath = outpath_str.c_str();

    int res = conv::convert(inpath, outpath, &convdata);
    if (res < 0) {
        log_err("[!][convert] Failed to conv::convert.\n");

        QMetaObject::invokeMethod(this,
            [this]()
            {
                QMessageBox::critical(this, "ошибка", //"convert_error",
                                      "Неудалось конвертировать файл.", //"Failed to convert file.",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton);
            },
            Qt::QueuedConnection
        );

        return;
    }

    if (!SessionsManager::containsSession()) {
        return;
    }

    char token[ServerRequester::TOKEN_STR_LEN] = {};
    SessionsManager::getToken(token);

    unsigned char *banner_data = nullptr;
    size_t banner_len = 0;

    res = ServerRequester::addToTheHistory(token, inpath,
                                           outpath, &banner_data,
                                           &banner_len);
    if (res != ServerRequester::HTTP_201_INT) {
        log_err("[!][convert] Failed to "
                "ServerRequester::addToTheHistory.\n");

        QMetaObject::invokeMethod(this,
            [this]()
            {
                QMessageBox::critical(this, "ошибка", //"convert_error",
                                     "Неудалось отправить данные на сервер.", //"Failed to convert file.",
                                     QMessageBox::Ok,
                                     QMessageBox::NoButton);
            },
            Qt::QueuedConnection
            );

        return;
    }

    QMetaObject::invokeMethod(this,
                              [this, banner_data, banner_len]()
                              {showBanner(banner_data, banner_len);},
                              Qt::QueuedConnection);
}

void MainWindow::setDefFn()
{
    if (upl_path.isEmpty()) {
        return;
    }
    p0_fn_edit->setText(upl_fn);
}

void MainWindow::setRandFn()
{
    char rand_str[37] = {0};

    for (int i = 0; i < 37; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            rand_str[i] = '-';
            continue;
        }
        char val = rand() % 15;

        /* ASCII. 97 - 'a', 48 - '0' */
        rand_str[i] = (val > 9) ? val + 97 : val + 48;
    }

    p0_fn_edit->setText(rand_str);
}

void MainWindow::openPage(int ind) {
    pages_stw->setCurrentIndex(ind);
}

void MainWindow::openLogWin()
{
    QDialog *diag = new QDialog(this);
    diag->setFixedSize(302, 232);
    diag->setWindowTitle(tr("login")); //"authorization"

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setContentsMargins(0, 0, 0, 8);
    diag->setLayout(main_grid);

    QFont segoeui20font("Segoe UI", 20);

    TrapLabel *title_tlb = new TrapLabel(tr("Login"), false); //"login"
    title_tlb->setMinimumHeight(54);
    title_tlb->setMaximumHeight(54);
    title_tlb->setFont(segoeui20font);
    main_grid->addWidget(title_tlb, 0, 0);

    QVBoxLayout *main_vbox = new QVBoxLayout();
    main_vbox->setContentsMargins(24, 0, 24, 8);
    main_vbox->setSpacing(2);
    main_grid->addLayout(main_vbox, 1, 0);

    QPalette err_pl = QPalette();
    err_pl.setColor(QPalette::WindowText, Qt::red);

    //
    //  Login widgets
    //

    QLabel *log_lb = new QLabel(tr("login:")); //"login:"
    main_vbox->addWidget(log_lb);

    QLineEdit *log_ed = new QLineEdit();
    main_vbox->addWidget(log_ed);

    QLabel *log_err_lb = new QLabel("");
    log_err_lb->setPalette(err_pl);
    main_vbox->addWidget(log_err_lb);

    //
    //  Password widgets
    //

    QLabel *pass_lb = new QLabel(tr("password:")); //"password:"
    main_vbox->addWidget(pass_lb);

    QHBoxLayout *pass_hbox = new QHBoxLayout();
    pass_hbox->setSpacing(8);
    main_vbox->addLayout(pass_hbox);

    QLineEdit *pass_ed = new QLineEdit();
    pass_ed->setEchoMode(QLineEdit::Password);
    pass_hbox->addWidget(pass_ed);

    ShowHideBtn *pass_shbtn = new ShowHideBtn();
    pass_shbtn->setMinimumSize(23, 23);
    pass_shbtn->setMaximumSize(23, 23);
    connect(pass_shbtn, &ShowHideBtn::showHide, this,
            [pass_ed](bool hidden)
            {
                pass_ed->setEchoMode(hidden ? QLineEdit::Password :
                                              QLineEdit::Normal);
            }
    );
    pass_hbox->addWidget(pass_shbtn);

    QLabel *pass_err_lb = new QLabel("");
    pass_err_lb->setPalette(err_pl);
    main_vbox->addWidget(pass_err_lb);

    //
    //  LOGIN_BTN
    //

    QPushButton *log_btn = new QPushButton(tr("login")); //"login"
    log_btn->setMinimumSize(128, 26);
    log_btn->setMaximumSize(128, 26);
    log_btn->setAutoFillBackground(true);
    log_btn->setObjectName("colored-btn");
    connect(log_btn, &QPushButton::clicked, this,
            [this, log_ed, pass_ed, log_err_lb, pass_err_lb, diag]()
            {
                validateLogData(log_ed, pass_ed,
                                log_err_lb, pass_err_lb, diag);
            }
    );
    main_grid->addWidget(log_btn, 2, 0,
                         Qt::AlignBottom | Qt::AlignHCenter);

    diag->exec();

    /* cleanup */
    delete(pass_err_lb);
    delete(pass_shbtn);
    delete(pass_ed);
    delete(pass_hbox);
    delete(pass_lb);

    delete(log_err_lb);
    delete(log_ed);
    delete(log_lb);

    delete(log_btn);
    delete(main_vbox);
    delete(main_grid);
    delete(diag);
}

void MainWindow::openRegWin()
{
    QDialog *reg_diag = new QDialog(this);
    reg_diag->setFixedSize(302, 312);
    reg_diag->setWindowTitle(tr("registration")); //"registration"

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setContentsMargins(0, 0, 0, 8);
    reg_diag->setLayout(main_grid);

    QFont segoeui20font("Segoe UI", 20);

    TrapLabel *title_tlb = new TrapLabel(tr("Registration"), true); //"registration", true
    title_tlb->setMinimumHeight(54);
    title_tlb->setMaximumHeight(54);
    title_tlb->setFont(segoeui20font);
    main_grid->addWidget(title_tlb, 0, 0);

    QVBoxLayout *main_vbox = new QVBoxLayout();
    main_vbox->setContentsMargins(24, 0, 24, 8);
    main_vbox->setSpacing(2);
    main_grid->addLayout(main_vbox, 1, 0);

    QPalette err_pl = QPalette();
    err_pl.setColor(QPalette::WindowText, Qt::red);
    err_pl.setColor(QPalette::Window, Qt::lightGray);
    err_pl.setColor(QPalette::AlternateBase, Qt::lightGray);

    //
    //  Login widgets
    //

    QLabel *log_lb = new QLabel(tr("login:")); //"login:"
    main_vbox->addWidget(log_lb);

    QLineEdit *log_ed = new QLineEdit();
    main_vbox->addWidget(log_ed);

    QLabel *log_err_lb = new QLabel("");
    log_err_lb->setPalette(err_pl);
    main_vbox->addWidget(log_err_lb);

    //
    //  Password widgets
    //

    QLabel *pass_lb = new QLabel(tr("password:")); //"password:"
    main_vbox->addWidget(pass_lb);

    QHBoxLayout *pass_hbox = new QHBoxLayout();
    pass_hbox->setSpacing(8);
    main_vbox->addLayout(pass_hbox);

    QLineEdit *pass_ed = new QLineEdit();
    pass_ed->setEchoMode(QLineEdit::Password);
    pass_hbox->addWidget(pass_ed);

    ShowHideBtn *pass_shbtn = new ShowHideBtn();
    pass_shbtn->setMinimumSize(23, 23);
    pass_shbtn->setMaximumSize(23, 23);
    connect(pass_shbtn, &ShowHideBtn::showHide, this,
        [pass_ed](bool hidden)
        {
            pass_ed->setEchoMode(hidden ? QLineEdit::Password :
                                          QLineEdit::Normal);
        }
    );
    pass_hbox->addWidget(pass_shbtn);

    QLabel *pass_err_lb = new QLabel("");
    pass_err_lb->setPalette(err_pl);
    main_vbox->addWidget(pass_err_lb);

    //
    //  Email widgets
    //

    QLabel *email_lb = new QLabel(tr("email:")); //"email:"
    main_vbox->addWidget(email_lb);

    QLineEdit *email_ed = new QLineEdit();
    main_vbox->addWidget(email_ed);

    QLabel *email_err_lb = new QLabel("");
    email_err_lb->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    email_err_lb->setPalette(err_pl);
    email_err_lb->setWordWrap(true);
    email_err_lb->setMinimumHeight(34);
    email_err_lb->setMaximumHeight(34);
    main_vbox->addWidget(email_err_lb);

    //
    //  REGISTER_BTN
    //

    QPushButton *reg_btn = new QPushButton(tr("register")); //"register"
    reg_btn->setMinimumSize(135, 26);
    reg_btn->setMaximumSize(135, 26);
    reg_btn->setAutoFillBackground(true);
    reg_btn->setObjectName("colored-btn");
    connect(reg_btn, &QPushButton::clicked, this,
            [this, log_ed, pass_ed, email_ed, log_err_lb,
             pass_err_lb, email_err_lb,  reg_diag]()
            {validateRegData(log_ed, pass_ed,
                             email_ed, log_err_lb, pass_err_lb,
                             email_err_lb, reg_diag);});
    main_grid->addWidget(reg_btn, 2, 0,
                         Qt::AlignBottom | Qt::AlignHCenter);

    reg_diag->exec();

    /* cleanup */
    delete(email_err_lb);
    delete(email_ed);
    delete(email_lb);

    delete(pass_err_lb);
    delete(pass_shbtn);
    delete(pass_ed);
    delete(pass_hbox);
    delete(pass_lb);

    delete(log_err_lb);
    delete(log_ed);
    delete(log_lb);

    delete(reg_btn);
    delete(main_vbox);
    delete(main_grid);
    delete(reg_diag);
}

void MainWindow::playStopMedia()
{
    if (p1_mplayer->isPlaying()) {
        p1_mplayer->pause();
    }
    else {
        p1_mplayer->play();
    }
}

void MainWindow::skipMediaTime(int millisecs)
{
    qint64 pos = p1_mplayer->position();
    p1_mplayer->setPosition(pos + millisecs);
}

void MainWindow::playingChanged(bool playing)
{
    if (!playing &&
        p1_mplayer->playbackState() == QMediaPlayer::StoppedState) {
        p1_psbtn->setState(false);
    }
}

void MainWindow::mediaPosChanged(qint64 position)
{
    static qint64 pos = 0;

    if ((position - pos < 1000) && (position > pos)) { /* making func work only on new second. */
        return;
    }

    pos       = position;
    position /= 1000;

    if (!p1_tl_sl->isSliderDown()) {
        p1_tl_sl->setValue(position);
    }

    const qint64 mins = position / 60;
    const qint64 secs = position % 60;
    char         buf[8] = {'\0'};    

    snprintf(buf, sizeof(buf), "%02" PRId64 ":%02" PRId64, mins, secs);
    p1_curtm_lb->setText(buf);
}

void MainWindow::tlSliderMoved(int val)
{
    p1_mplayer->setPosition(val * 1000); /* secs to millisecs */
}

void MainWindow::tlSliderPressed()
{
    p1_mplayer->setPosition(p1_tl_sl->value() * 1000); /* secs to millisecs */
}

void MainWindow::outFmtChanged()
{
    int index = p0_fn_cbox->currentIndex();
    if (index < 0) {
        return;
    }

    if (fmtdata.type == conv::MediaType::IMG) {
        conv::ImgData *idata = conv::getImgParams(index);

        p2_vcodec_cbox->clear();

        for (int i = 0; i < idata->len; ++i) {
            conv::VidCodec *codec = &idata->codecs[i];
            p2_vcodec_cbox->addItem(codec->name);
        }
    }
    else if (fmtdata.type == conv::MediaType::AUD) {
        conv::AudData *adata = conv::getAudParams(index);

        p2_acodec_cbox->clear();

        for (int i = 0; i < adata->len; ++i) {
            conv::AudCodec *codec = &adata->codecs[i];
            p2_acodec_cbox->addItem(codec->name);
        }
    }
    else {
        conv::VidData *vdata = conv::getVidParams(index);

        p2_vcodec_cbox->clear();
        p2_acodec_cbox->clear();

        for (int i = 0; i < vdata->vlen; ++i) {
            conv::VidCodec *codec = &vdata->vcodecs[i];
            p2_vcodec_cbox->addItem(codec->name);
        }

        for (int i = 0; i < vdata->alen; ++i) {
            conv::AudCodec *codec = &vdata->acodecs[i];
            p2_acodec_cbox->addItem(codec->name);
        }
    }
}

void MainWindow::vidCodecChanged()
{    
    int index = p0_fn_cbox->currentIndex();
    if (index < 0) {
        return;
    }

    int vcodec_index = p2_vcodec_cbox->currentIndex();
    if (vcodec_index < 0) {
        return;
    }

    conv::VidCodec *codec = fmtdata.type == conv::MediaType::IMG ?
        &conv::getImgParams(index)->codecs[vcodec_index] :
        &conv::getVidParams(index)->vcodecs[vcodec_index];

    p2_pixfmt_cbox->clear();

    for (int i = 0; i < codec->pixfmts_len; ++i) {
        p2_pixfmt_cbox->addItem(codec->pixfmts[i].name);
    }
}

void MainWindow::audCodecChanged()
{
    int index = p0_fn_cbox->currentIndex();
    if (index < 0) {
        return;
    }

    int acodec_index = p2_acodec_cbox->currentIndex();
    if (acodec_index < 0) {
        return;
    }

    p2_smplfmt_cbox->clear();
    p2_smplrate_cbox->clear();
    p2_bitrate_cbox->clear();
    p2_nbchannels_cbox->clear();

    conv::AudCodec *codec = fmtdata.type == conv::MediaType::AUD ?
        &conv::getAudParams(index)->codecs[acodec_index] :
        &conv::getVidParams(index)->acodecs[acodec_index];

    for (int i = 0; i < codec->smplfmts_len; ++i) {
        p2_smplfmt_cbox->addItem(codec->smplfmts[i].name);
    }

    for (int i = 0; i < codec->smplrates_len; ++i) {
        p2_smplrate_cbox->addItem(codec->smplrates[i].str);
    }

    for (int i = 0; i < codec->bitrates_len; ++i) {
        p2_bitrate_cbox->addItem(codec->bitrates[i].str);
    }

    for (int i = 0; i < codec->nb_channels_len; ++i) {
        p2_nbchannels_cbox->addItem(codec->nb_channels[i].str);
    }

    p2_smplrate_cbox->setCurrentIndex(codec->def_smplrate_ind);
    p2_bitrate_cbox->setCurrentIndex(codec->def_bitrate_ind);
    p2_nbchannels_cbox->setCurrentIndex(1); /* Stereo index */
}

void MainWindow::openProfWin()
{
    QDialog *prof_diag = new QDialog(this);
    prof_diag->setFixedSize(565, 620);
    prof_diag->setWindowTitle(tr("profile")); //"profile"

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setVerticalSpacing(0);
    main_grid->setHorizontalSpacing(8);
    main_grid->setContentsMargins(0, 0, 0, 8);
    prof_diag->setLayout(main_grid);

    QFont segoeui20font("Segoe UI", 20);
    QFont segoeui14font("Segoe UI", 14);

    TrapLabel *title_tlb = new TrapLabel(tr("Profile"), true); //"profile"
    title_tlb->setMinimumHeight(54);
    title_tlb->setMaximumHeight(54);
    title_tlb->setFont(segoeui20font);
    main_grid->addWidget(title_tlb, 0, 0, 1, 2);

    QGridLayout *margin_grid = new QGridLayout();
    margin_grid->setContentsMargins(8, 8, 8, 8);
    margin_grid->setVerticalSpacing(18);
    margin_grid->setHorizontalSpacing(8);
    main_grid->addLayout(margin_grid, 1, 0, 1, 2);

    QPixmap prof_pixmap = QPixmap(":/images/profile.png");

    QHBoxLayout *prof_hbox = new QHBoxLayout();
    prof_hbox->setContentsMargins(0, 0, 0, 0);
    prof_hbox->setSpacing(12);
    margin_grid->addLayout(prof_hbox, 0, 0, 1, 1,
                           Qt::AlignVCenter | Qt::AlignLeft);

    QLabel *prof_img_lb = new QLabel();
    prof_img_lb->setMaximumSize(230, 230);
    prof_img_lb->setPixmap(prof_pixmap.scaled(prof_img_lb->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::FastTransformation));
    prof_hbox->addWidget(prof_img_lb);

    QVBoxLayout *prof_box = new QVBoxLayout();
    prof_box->setContentsMargins(0, 0, 0, 0);
    prof_box->setSpacing(8);
    prof_hbox->addLayout(prof_box);

    QLabel *login_lb = new QLabel(tr("login:") + "\nisp422");
    login_lb->setFont(segoeui14font);
    prof_box->addWidget(login_lb);

    Line *line0 = new Line(0, 0, 275, 2);
    prof_box->addWidget(line0);

    QLabel *mail_lb = new QLabel(tr("email:") + "\nmamedova6.milana@yandex.ru");
    mail_lb->setFont(segoeui14font);
    prof_box->addWidget(mail_lb);

    Line *line1 = new Line(0, 0, 275, 2);
    prof_box->addWidget(line1);

    QLabel *reg_date_lb = new QLabel(tr("registration date:") + "\n07.05.2026");
    reg_date_lb->setFont(segoeui14font);
    prof_box->addWidget(reg_date_lb);

    //
    //
    //

    QStandardItemModel *sti_model = new QStandardItemModel();
    sti_model->setColumnCount(6);
    sti_model->setHeaderData(0, Qt::Horizontal, tr("id"));
    sti_model->setHeaderData(1, Qt::Horizontal, tr("input file")); //"input file"
    sti_model->setHeaderData(2, Qt::Horizontal, tr("output file")); //"output file"
    sti_model->setHeaderData(3, Qt::Horizontal, tr("input format")); //"input format"
    sti_model->setHeaderData(4, Qt::Horizontal, tr("output format")); //"output format"
    sti_model->setHeaderData(5, Qt::Horizontal, tr("date")); //"date"

    QList<QStandardItem*> row0;

    QStandardItem *item0 = new QStandardItem("0");
    QStandardItem *item1 = new QStandardItem("C:\\Users\\user2006\\Downloads\\img00.png");
    QStandardItem *item2 = new QStandardItem("C:\\Users\\user2006\\Downloads\\img01.ico");
    QStandardItem *item3 = new QStandardItem("png");
    QStandardItem *item4 = new QStandardItem("ico");
    QStandardItem *item5 = new QStandardItem("2026-05-07 17:00:02+03");

    row0.append(item0);
    row0.append(item1);
    row0.append(item2);
    row0.append(item3);
    row0.append(item4);
    row0.append(item5);

    sti_model->appendRow(row0);

    QTableView *tview = new QTableView();
    tview->setModel(sti_model);
    tview->setAlternatingRowColors(true);
    tview->setStatusTip(tr("convertations history")); //"convertations data"
    tview->setEditTriggers(QTableView::NoEditTriggers);
    tview->setColumnWidth(0, 30);
    tview->setColumnWidth(1, 230);
    tview->setColumnWidth(2, 230);
    tview->setColumnWidth(3, 115);
    tview->setColumnWidth(4, 115);
    tview->setColumnWidth(5, 150);
    margin_grid->addWidget(tview, 1, 0, 1, 2);

    QPushButton *logout_btn = new QPushButton(tr("logout")); //"logout"
    logout_btn->setObjectName("colored-btn");
    logout_btn->setMinimumSize(100, 25);
    logout_btn->setMaximumSize(100, 25);
    connect(logout_btn, &QPushButton::clicked, this,
            [this, prof_diag](){logoutUser(prof_diag);});
    main_grid->addWidget(logout_btn, 2, 0, 1, 2,
                         Qt::AlignBottom | Qt::AlignHCenter);

    prof_diag->exec();

    /* cleanup */
    delete(logout_btn);
    delete(tview);
    delete(sti_model);
    delete(reg_date_lb);
    delete(line1);
    delete(mail_lb);
    delete(line0);
    delete(login_lb);
    delete(prof_box);
    delete(prof_hbox);
    delete(prof_img_lb);
    delete(margin_grid);
    delete(title_tlb);
    delete(main_grid);
    delete(prof_diag);
}

void MainWindow::validateRegData(QLineEdit *log_ed,
                                 QLineEdit *pass_ed,
                                 QLineEdit *email_ed,
                                 QLabel *err_log_lb,
                                 QLabel *err_pass_lb,
                                 QLabel *err_email_lb,
                                 QDialog *reg_diag)
{
    /* declared here, cause there are shered */
    int ret = 0;
    QString ed_str;
    qsizetype ed_len = -1;
    qsizetype doggy_pos = -1;
    qsizetype dot_pos = -1;

    //
    //  LOGIN
    //

    ed_str = log_ed->text();
    ed_len = ed_str.length();

    if (ed_str.isEmpty()) {
        err_log_lb->setText("логин не может быть пустым!"); //"login can\'t be empty!");
        ret = -1;
    }
    else if (ed_len < MIN_LOGIN_LEN) {
        err_log_lb->setText("длина логина должна быть больше " //"login length has to be greater than "
                           TO_STR(MIN_LOGIN_LEN) "!");
        ret = -1;
    }
    else if (ed_len > MAX_LOGIN_LEN) {
        err_log_lb->setText("длина логина должна быть меньше " //"login length has to be less than "
                           TO_STR(MAX_LOGIN_LEN) "!");
        ret = -1;
    }
    else if (ed_str.startsWith(' ')) {
        err_log_lb->setText("логин не может начинаться с пробела!"); //"login can\'t start with whitespace!");
        ret = -1;
    }
    else if (ed_str.endsWith(' ')) {
        err_log_lb->setText("логин не может заканчиваться пробелом!"); //"login can\'t end with whitespace!");
        ret = -1;
    }
    else if (!ed_str[0].isLetter()) {
        err_log_lb->setText("логин должен начинаться с буквы!"); //"login first charecter has to be letter!");
        ret = -1;
    }
    else {
        for (int i = 0; i < ed_len; ++i) {
            if (ed_str[i] == ' ') {
                err_log_lb->setText("логин не может содержать "
                                    "пробелы!");
                ret = -1;
                break;
            }
            else if (!ed_str[i].isLetterOrNumber()) {
                err_log_lb->setText("логин может состоять "
                                    "только из букв и цифр!");
                ret = -1;
                break;
            }
        }

        if (ret == 0) {
            err_log_lb->setText("");
        }
    }

    //
    //  PASSWORD
    //

    ed_str = pass_ed->text();
    ed_len = ed_str.length();

    if (ed_str.isEmpty()) {
        err_pass_lb->setText("пароль не может быть пустым!"); //"password can\'t be empty!");
        ret += -10;
    }
    else if (ed_len < MIN_PASSWORD_LEN) {
        err_pass_lb->setText("длина пароля должна быть больше " //"password length has to be greater than "
                           TO_STR(MIN_PASSWORD_LEN) "!");
        ret += -10;
    }
    else if (ed_len > MAX_PASSWORD_LEN) {
        err_pass_lb->setText("длина пароля должна быть меньше " //"password length has to be less than "
                           TO_STR(MAX_PASSWORD_LEN) "!");
        ret += -10;
    }
    else if (ed_str.startsWith(' ')) {
        err_pass_lb->setText("пароль не может начинаться с пробела!"); //"password can\'t start with whitespace!");
        ret += -10;
    }
    else if (ed_str.endsWith(' ')) {
        err_pass_lb->setText("пароль не может заканчиваться пробелом!"); //"password can\'t end with whitespace!");
        ret += -10;
    }
    else {
        for (int i = 0; i < ed_len; ++i) {
            if (ed_str[i] == ' ') {
                err_pass_lb->setText("пароль не может содержать "
                                     "пробелы!");
                ret += -10;
                break;
            }
            else if (!ed_str[i].isLetterOrNumber()) {
                err_pass_lb->setText("пароль может состоять "
                                     "только из букв и цифр!");
                ret += -10;
                break;
            }
        }

        if (ret > -10) {
            err_pass_lb->setText("");
        }
    }

    //
    //  EMAIL
    //

    ed_str = email_ed->text();
    ed_len = ed_str.length();

    if (ed_str.isEmpty()) {
        err_email_lb->setText("почта не может быть пустой!"); //"email can\'t be empty!");
        ret += -100;
    }
    else if (ed_len < MIN_EMAIL_LEN) {
        err_email_lb->setText("длина почты должна быть больше " //"email length has to be greater than "
                           TO_STR(MIN_EMAIL_LEN) "!");
        ret += -100;
    }
    else if (ed_len > MAX_EMAIL_LEN) {
        err_email_lb->setText("длина почты должна быть меньше " //"email length has to be less than "
                           TO_STR(MAX_EMAIL_LEN) "!");
        ret += -100;
    }
    else if (ed_str.startsWith(' ')) {
        err_email_lb->setText("почта не может начинаться с пробела!"); //"email can\'t start with whitespace!");
        ret += -100;
    }
    else if (ed_str.endsWith(' ')) {
        err_email_lb->setText("почта не может заканчиваться пробелом!"); //"email can\'t end with whitespace!");
        ret += -100;
    }
    else if (!ed_str[0].isLetterOrNumber()) {
        err_email_lb->setText("email first charecter has to be "
                           "letter or number!");
        ret += -100;
    }
    else if ((doggy_pos = ed_str.indexOf('@')) == -1) {
        err_email_lb->setText("email has to contain "
                              "\'@\' charecter!");
        ret += -100;
    }
    else if (ed_str.lastIndexOf('@') != doggy_pos) {
        err_email_lb->setText("email has to contain only 1 \'@\'"
                           " charecter!");
        ret += -100;
    }
    else if ((dot_pos = ed_str.indexOf('.')) == -1) {
        err_email_lb->setText("email has to contain "
                              "\'.\' charecter!");
        ret += -100;
    }
    else if (doggy_pos + 1 == dot_pos) {
        err_email_lb->setText("email has to contain at least 1 "
                           "char between \'@\' and \'.\'!");
        ret += -100;
    }
    else if (dot_pos == ed_len - 1) {
        err_email_lb->setText("email has to contain at least 1 "
                           "char after \'.\'!");
        ret += -100;
    }
    else {
        QString special_chars = "@.!#$%^&?*_-+=\'|~`";

        for (int i = 0; i < ed_len; ++i) {
            if (ed_str[i] == ' ') {
                err_email_lb->setText("email can\'t contain "
                                    "whitespaces!");
                ret += -100;
                break;
            }
            else if (!ed_str[i].isLetterOrNumber() &&
                     !special_chars.contains(ed_str[i])) {
                err_email_lb->setText("email can contain "
                                   "only letters or numbers "
                                   "and special (@.!#$%^&?*_-+=\'|~`) chars!");
                ret += -100;
                break;
            }
        }
    }

    if (ret > -100) {
        err_email_lb->setText("");
    }

    if (ret != 0) {
        return;
    }

    /* all_fileds_is_ok */

    QDialog *diag = new QDialog(this);
    diag->setWindowTitle(tr("confirm")); //"email_confirm"
    diag->setMinimumSize(300, 230);
    diag->setMaximumSize(300, 230);

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setContentsMargins(0, 0, 0, 8);
    main_grid->setRowStretch(1, 1);
    diag->setLayout(main_grid);

    QFont segoeui20font("Segoe UI", 20);
    QFont segoeui14font("Segoe UI", 14);

    TrapLabel *title_tlb = new TrapLabel("Подтверждение почты"); //"confirm email"
    title_tlb->setMinimumHeight(54);
    title_tlb->setMaximumHeight(54);
    title_tlb->setFont(segoeui20font);
    main_grid->addWidget(title_tlb, 0, 0);

    QVBoxLayout *main_vbox = new QVBoxLayout();
    main_vbox->setContentsMargins(24, 0, 24, 8);
    main_grid->addLayout(main_vbox, 1, 0);

    QVBoxLayout *info_vbox = new QVBoxLayout();
    info_vbox->setSpacing(8);
    info_vbox->setContentsMargins(0, 0, 0, 8);
    main_vbox->addLayout(info_vbox);

    QLabel *info_lb = new QLabel("Введите код подтверждения, отправленный на указанную ранее почту."); //"Enter the confirm code, sent to the specified email."
    info_lb->setWordWrap(true);
    info_lb->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    info_lb->setFont(segoeui14font);
    info_vbox->addWidget(info_lb, 0,
                         Qt::AlignTop | Qt::AlignHCenter);

    //QLabel *email_lb = new QLabel("mamedova6.milana@yandex.ru");
    //email_lb->setWordWrap(true);
    //email_lb->setFont(segoeui14font);
    //info_vbox->addWidget(email_lb, 0,
    //                     Qt::AlignTop | Qt::AlignHCenter);

    QVBoxLayout *code_vbox = new QVBoxLayout();
    code_vbox->setSpacing(2);
    code_vbox->setContentsMargins(0, 0, 0, 8);
    main_vbox->addLayout(code_vbox);

    QLabel *code_lb = new QLabel("код:"); //"code:"
    code_vbox->addWidget(code_lb);

    QLineEdit *code_ed = new QLineEdit();
    code_vbox->addWidget(code_ed);

    QPushButton *confirm_btn = new QPushButton("подтвердить"); //"confirm"
    confirm_btn->setMinimumSize(120, 25);
    confirm_btn->setMaximumSize(120, 25);
    confirm_btn->setObjectName("colored-btn");
    connect(confirm_btn, &QPushButton::clicked, this,
            [this, log_ed, pass_ed, email_ed, reg_diag, diag]()
            {confirmEmail(log_ed, pass_ed,
                          email_ed, reg_diag, diag);}
    );
    main_grid->addWidget(confirm_btn, 2, 0, 1, 1,
                         Qt::AlignBottom | Qt::AlignHCenter);

    diag->exec();

    /* cleanup */

    delete(confirm_btn);
    delete(code_ed);
    delete(code_lb);
    delete(code_vbox);
    //delete(email_lb);
    delete(info_lb);
    delete(main_vbox);
    delete(title_tlb);
    delete(main_grid);
}

void MainWindow::validateLogData(QLineEdit *log_ed,
                                 QLineEdit *pass_ed,
                                 QLabel *err_log_lb,
                                 QLabel *err_pass_lb,
                                 QDialog *diag)
{
    /* declared here, cause there are shered */
    int ret = 0;
    QString ed_str;
    qsizetype ed_len = -1;

    //
    //  LOGIN
    //

    ed_str = log_ed->text();
    ed_len = ed_str.length();

    if (ed_str.isEmpty()) {
        err_log_lb->setText("логин не может быть пустым!"); //login can\'t be empty!");
        ret = -1;
    }
    else if (ed_len < MIN_LOGIN_LEN) {
        err_log_lb->setText("длина логина должна быть больше " //login length has to be greater than "
                            TO_STR(MIN_LOGIN_LEN) "!");
        ret = -1;
    }
    else if (ed_len > MAX_LOGIN_LEN) {
        err_log_lb->setText("длина логина должна быть меньше " //login length has to be less than "
                            TO_STR(MAX_LOGIN_LEN) "!");
        ret = -1;
    }
    else if (ed_str.startsWith(' ')) {
        err_log_lb->setText("логин не может начинаться с пробела!"); //login can\'t start with whitespace!");
        ret = -1;
    }
    else if (ed_str.endsWith(' ')) {
        err_log_lb->setText("логин не может заканчиваться пробелом!");
        ret = -1;
    }
    else if (!ed_str[0].isLetter()) {
        err_log_lb->setText("логин должен начинаться с буквы!"); //login first charecter has to be letter!");
        ret = -1;
    }
    else {
        for (int i = 0; i < ed_len; ++i) {
            if (ed_str[i] == ' ') {
                err_log_lb->setText("логин не может содержать "
                                    "пробелы!"); //"login can\'t contain whitespaces!");
                ret = -1;
                break;
            }
            else if (!ed_str[i].isLetterOrNumber()) {
                err_log_lb->setText("логин может состоять "
                                    "только из букв и цифр!"); //"login can contain only letters or numbers!");
                ret = -1;
                break;
            }
        }

        if (ret == 0) {
            err_log_lb->setText("");
        }
    }

    //
    //  PASSWORD
    //

    ed_str = pass_ed->text();
    ed_len = ed_str.length();

    if (ed_str.isEmpty()) {
        err_pass_lb->setText("пароль не может быть пустым!");
        ret += -10;
    }
    else if (ed_len < MIN_PASSWORD_LEN) {
        err_pass_lb->setText("длина пароля должна быть больше "
                             TO_STR(MIN_PASSWORD_LEN) "!");
        ret += -10;
    }
    else if (ed_len > MAX_PASSWORD_LEN) {
        err_pass_lb->setText("длина пароля должна быть меньше "
                             TO_STR(MAX_PASSWORD_LEN) "!");
        ret += -10;
    }
    else if (ed_str.startsWith(' ')) {
        err_pass_lb->setText("пароль не может начинаться с пробела!");
        ret += -10;
    }
    else if (ed_str.endsWith(' ')) {
        err_pass_lb->setText("пароль не может заканчиваться пробелом!");
        ret += -10;
    }
    else {
        for (int i = 0; i < ed_len; ++i) {
            if (ed_str[i] == ' ') {
                err_pass_lb->setText("пароль не может содержать "
                                     "пробелы!");
                ret += -10;
                break;
            }
            else if (!ed_str[i].isLetterOrNumber()) {
                err_pass_lb->setText("пароль может состоять "
                                     "только из букв и цифр!");
                ret += -10;
                break;
            }
        }

        if (ret > -10) {
            err_pass_lb->setText("");
        }
    }

    if (ret != 0) {
        return;
    }

    ret = logUser(log_ed, pass_ed);
    if (ret < 0) {
        QMessageBox::critical(this, "ошибка", //"login_error",
                              "Не получилось войти в аккаунт.", //"Failed to login user.",
                              QMessageBox::Ok,
                              QMessageBox::NoButton);
        return;
    }

    //SessionsManager::saveSession(log_ed->text().toStdString().c_str(),
    //                             pass_ed->text().toStdString().c_str(),
    //                             userid);

    QMessageBox::information(this, "иформация", //"login_info",
                             "Вход успешен!", //"Login sucessed!",
                             QMessageBox::Ok);

    /* DISPLAYING AUTHED USER's BTNS */
    log_btn->setVisible(false);
    reg_btn->setVisible(false);
    prof_btn->setVisible(true);

    diag->close();
}

int MainWindow::regUser(QLineEdit *log_ed, QLineEdit *pass_ed,
                        QLineEdit *email_ed)
{    
    char token[ServerRequester::TOKEN_STR_LEN] = {};
    int ret = 0;

    /* add async, couse now it stucks on sleep_for */

    ret = ServerRequester::registerUser(
        log_ed->text().toStdString().c_str(),
        pass_ed->text().toStdString().c_str(),
        email_ed->text().toStdString().c_str(),
        token);
    if (ret != ServerRequester::HTTP_201_INT) {
        if (ret == ServerRequester::HTTP_409_INT) {
            QMessageBox::critical(this, "ошибка", //"registration_error",
                                  "Пользователь с указанным логином уже существует.",
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton);
        }
        else { /* ret == ServerRequester::HTTP_500_INT */
            QMessageBox::critical(this, "ошибка",
                                  "Ошибка сервера. Повторите попытку позже.",
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton);
        }
        log_err("[!][regUser] Failed to "
                "ServerRequester::registerUser.\n");
        return -1;
    }

    SessionsManager::saveSession(token);
    log_str("[*][regUser] token: ^%s^.\n", token);

    return 0;
}
int MainWindow::logUser(QLineEdit *log_ed, QLineEdit *pass_ed)
{
    char token[ServerRequester::TOKEN_STR_LEN] = {};
    char email[ServerRequester::EMAIL_STR_LEN] = {};
    char created_at[ServerRequester::DATE_STR_LEN] = {};
    int ret = 0;

    log_str("[*][logUser] before ServerRequester::loginUser.\n");
    ret = ServerRequester::loginUser(
        log_ed->text().toStdString().c_str(),
        pass_ed->text().toStdString().c_str(),
        token, email, created_at);
    log_str("[*][logUser] after ServerRequester::loginUser.\n"
            "\ttoken: %s\n\temail: %s\n\tcreated_at: %s.\n",
            token, email, created_at);
    if (ret != ServerRequester::HTTP_200_INT) {
        if (ret == ServerRequester::HTTP_401_INT) {
            QMessageBox::critical(this, "ошибка",
                                  "Пользователя с указанным логином не существует.",
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton);
        }
        else { /* ret == ServerRequester::HTTP_500_INT */
            QMessageBox::critical(this, "ошибка",
                                  "Ошибка сервера. Повторите попытку позже.",
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton);
        }
        log_str("[!][logUser] Failed to "
                "ServerRequester::loginUser.\n");
        return -1;
    }
    log_str("[*][logUser] before SessionsManager::saveSession.\n");

    SessionsManager::saveSession(token);
    log_str("[*][logUser] token: ^%s^.\n", token);

    return 0;
}

void MainWindow::paramsToDef()
{
    this->outFmtChanged();

    if (fmtdata.type == conv::MediaType::IMG) {
        this->vidCodecChanged();
    }
    else if (fmtdata.type == conv::MediaType::VID) {
        this->vidCodecChanged();
        this->audCodecChanged();
    }
    else { /* conv::MediaType::AUD */
        this->audCodecChanged();
    }
}

void MainWindow::logoutUser(QDialog *diag)
{
    SessionsManager::deleteSession();

    prof_btn->setVisible(false);
    log_btn->setVisible(true);
    reg_btn->setVisible(true);

    diag->close();
}

void MainWindow::confirmEmail(QLineEdit *log_ed, QLineEdit *pass_ed,
                              QLineEdit *email_ed, QDialog *reg_diag,
                              QDialog *diag)
{
    log_str("[*][confirmEmail] going to regUser.\n");

    int ret = regUser(log_ed, pass_ed, email_ed);
    if (ret < 0) {
        QMessageBox::critical(this, "ошибка", //"registration_error",
                              "Не получилось зарегистрировать аккаунт.", //"Failed to register user.",
                              QMessageBox::Ok,
                              QMessageBox::NoButton);
        return;
    }

    log_str("[*][confirmEmail] regUser successed!\n");

    QMessageBox::information(this, "информация", //"registration_info",
                             "Регистрация  успешна!", //"Registration sucessed!",
                             QMessageBox::Ok);

    /* DISPLAYING AUTHED USER's BTNS */
    log_btn->setVisible(false);
    reg_btn->setVisible(false);
    prof_btn->setVisible(true);

    diag->close();
    reg_diag->close();
}

void MainWindow::showBanner(unsigned char *data, size_t len)
{
    QDialog *diag = new QDialog(this);
    diag->setWindowTitle("рекламная рекомендация"); //"banner"
    diag->setMinimumSize(480, 270);
    diag->setMaximumSize(480, 270);

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setContentsMargins(0, 0, 0, 0);
    diag->setLayout(main_grid);

    QPixmap pixmap;
    if (!pixmap.loadFromData(data, len)) {
        log_err("[!][showBanner] Failed to loadFromData.\n");
        delete(main_grid);
        delete(diag);
        return;
    }

    QLabel *img_lb = new QLabel();
    img_lb->setMaximumSize(480, 270);
    img_lb->setPixmap(pixmap.scaled(img_lb->size(),
                                    Qt::IgnoreAspectRatio,
                                    Qt::FastTransformation));
    main_grid->addWidget(img_lb, 0, 0);

    diag->exec();

    /* cleanup */
    free(data);
    delete(img_lb);
    delete(main_grid);
    delete(diag);
}

void MainWindow::openSettWin()
{
    QDialog *diag = new QDialog(this);
    diag->setWindowTitle(tr("settings")); //"banner"
    diag->setMinimumSize(250, 175);
    diag->setMaximumSize(250, 175);

    QGridLayout *main_grid = new QGridLayout();
    main_grid->setContentsMargins(0, 0, 0, 12);
    diag->setLayout(main_grid);

    QFont segoeui20font("Segoe UI", 20);

    TrapLabel *title_tlb = new TrapLabel(tr("Settings"));
    title_tlb->setMinimumHeight(54);
    title_tlb->setMaximumHeight(54);
    title_tlb->setFont(segoeui20font);
    main_grid->addWidget(title_tlb, 0, 0, 1, 1);

    QGridLayout *lang_grid = new QGridLayout();
    lang_grid->setContentsMargins(12, 12, 12, 0);
    lang_grid->setHorizontalSpacing(16);
    lang_grid->setColumnStretch(2, 1);
    main_grid->addLayout(lang_grid, 1, 0, 1, 1,
                         Qt::AlignTop | Qt::AlignLeft);

    QLabel *lang_lb = new QLabel(tr("language:"));
    lang_grid->addWidget(lang_lb, 0, 0, 1, 1,
                         Qt::AlignVCenter | Qt::AlignLeft);

    QComboBox *lang_cbox = new QComboBox();
    lang_cbox->setMinimumWidth(125);
    lang_cbox->setMaximumWidth(125);
    lang_cbox->addItem("russian");
    lang_cbox->addItem("english");
    lang_grid->addWidget(lang_cbox, 0, 1, 1, 1,
                         Qt::AlignVCenter | Qt::AlignLeft);

    QPushButton *save_btn = new QPushButton(tr("apply"));
    save_btn->setMinimumSize(120, 25);
    save_btn->setMaximumSize(120, 25);
    save_btn->setObjectName("colored-btn");
    connect(save_btn, &QPushButton::clicked, this,
            [this, lang_cbox, diag, lang_lb, save_btn, title_tlb]()
            {
                changeLanguage(lang_cbox, diag, lang_lb,
                       save_btn, title_tlb);
            }
    );
    main_grid->addWidget(save_btn, 2, 0, 1, 1,
                         Qt::AlignBottom | Qt::AlignHCenter);

    diag->exec();

    /* clenaup */
    delete(save_btn);
    delete(lang_cbox);
    delete(lang_lb);
    delete(title_tlb);
    delete(main_grid);
}

void MainWindow::changeLanguage(QComboBox *lang_cbox,
                                QDialog *diag,
                                QLabel *lb,
                                QPushButton *btn,
                                TrapLabel *tlb)
{
    QString new_lang = lang_cbox->currentText();

    log_str("[*][changeLanguage] old: %s, new: %s.\n",
            language.toStdString().c_str(),
            new_lang.toStdString().c_str());

    if (language == new_lang) {
        return;
    }

    language = new_lang;

    if (!translator->isEmpty()) {
        qApp->removeTranslator(translator);
        log_str("removing\n");
    }

    QString lang_sub = language.left(2);

    log_str("left: %s.\n", lang_sub.toStdString().c_str());

    if (lang_sub == "ru") {
        if (!translator->load(":/translations/" + lang_sub + ".qm")) {
            QMessageBox::critical(this, tr("error"),
                                  tr("Failed to change language."),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton);
            return;
        }

        qApp->installTranslator(translator);
    }

    SessionsManager::setLanguage(language);

    diag->setWindowTitle(tr("settings"));
    lb->setText(tr("language:"));
    btn->setText(tr("apply"));
    tlb->setText(tr("Settings"));

    log_btn->setText(tr("login"));
    reg_btn->setText(tr("registration"));
    prof_btn->setText(tr("profile"));
    sett_btn->setText(tr("settings"));
    page0_btn->setText(tr("main"));
    page1_btn->setText(tr("data"));
    page2_btn->setText(tr("parameters"));
    p0_upl_downl_sbtn->setText(tr("open"), tr("download"));
    p0_clear_rm_sbtn->setText(tr("clear"), tr("delete"));
    p0_fn_def_rand_sbtn->setText(tr("default"), tr("random"));
    p1_tview->setStatusTip(tr("input file\'s metadata"));
    p2_vcodec_lb->setText(tr("video codec:"));
    p2_pixfmt_lb->setText(tr("pixels format:"));
    p2_acodec_lb->setText(tr("audio codec:"));
    p2_smplfmt_lb->setText(tr("samples format:"));
    p2_smplrate_lb->setText(tr("sample rate:"));
    p2_bitrate_lb->setText("bit rate:");
    p2_nbchannels_lb->setText(tr("channels quantity:"));
    p2_def_btn->setText(tr("default"));
}
