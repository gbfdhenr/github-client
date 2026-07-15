#include "gui/github_ui.h"
#include "utils/browser_cookies.h"

#include <QApplication>
#include <QMessageBox>
#include <QFont>
#include <QColor>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QHeaderView>
#include <QScrollBar>
#include <QDialogButtonBox>
#include <QtMath>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

namespace GH = GitHubColors;

// ─── SpinnerWidget ──────────────────────────────────────────────────────────

SpinnerWidget::SpinnerWidget(QWidget *parent, int size)
    : QWidget(parent), m_angle(0), m_size(size), m_timer(new QTimer(this))
{
    setFixedSize(size, size);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        m_angle = (m_angle + 30) % 360;
        update();
    });
}

void SpinnerWidget::start() { m_timer->start(50); }
void SpinnerWidget::stop() { m_timer->stop(); }

void SpinnerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int w = m_size, n = 12;
    for (int i = 0; i < n; ++i) {
        qreal alpha = 1.0 - (qreal)i / n;
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(139, 148, 158, (int)(alpha * 255)));
        qreal rad = qDegreesToRadians((qreal)(m_angle + i * 360 / n));
        int cx = w / 2 + (int)((w / 2 - 6) * qCos(rad));
        int cy = w / 2 + (int)((w / 2 - 6) * qSin(rad));
        p.drawEllipse(QPoint(cx, cy), 3, 3);
    }
}

// ─── LoadingLabel ───────────────────────────────────────────────────────────

LoadingLabel::LoadingLabel(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    auto *lay = new QVBoxLayout(this);
    lay->setAlignment(Qt::AlignCenter);
    m_spinner = new SpinnerWidget(this, 32);
    m_label = new QLabel(text);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet(QString("color: %1; font-size: 15px;").arg(GH::TEXT_SECONDARY));
    lay->addWidget(m_spinner, 0, Qt::AlignCenter);
    lay->addWidget(m_label);
    m_spinner->start();
}

void LoadingLabel::setLoadingText(const QString &text) { m_label->setText(text); }

// ─── ClickableFrame ─────────────────────────────────────────────────────────

ClickableFrame::ClickableFrame(QWidget *parent) : QFrame(parent)
{
    setCursor(Qt::PointingHandCursor);
}

// ─── NavButton ──────────────────────────────────────────────────────────────

NavButton::NavButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    setFlat(true);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(QString(R"(
        QPushButton { color: %1; font-size: 14px; font-weight: 500; padding: 4px 8px; }
        QPushButton:hover { color: %2; }
    )").arg(GH::HEADER_TEXT, GH::TEXT_SECONDARY));
}

// ─── Common Styles ──────────────────────────────────────────────────────────

static const char *navDropdownStyle = R"(
    QPushButton { color: #e6edf3; font-size: 14px; font-weight: 500;
                  background: transparent; border: none; padding: 6px 10px; }
    QPushButton:hover { color: #8b949e; }
)";

static const char *blueBtn = R"(
    QPushButton { background: #1f6feb; color: #fff; border: none; border-radius: 6px;
                  padding: 7px 18px; font-size: 14px; font-weight: 600; }
    QPushButton:hover { background: #388bfd; }
)";

static const char *outlineBtn = R"(
    QPushButton { background: transparent; color: #c9d1d9; border: 1px solid #30363d;
                  border-radius: 6px; padding: 7px 18px; font-size: 14px; font-weight: 600; }
    QPushButton:hover { color: #f0f6fc; border-color: #8b949e; }
)";

static const char *greenBtn = R"(
    QPushButton { background: #238636; color: #fff; border: none; border-radius: 6px;
                  padding: 8px 20px; font-size: 15px; font-weight: 600; }
    QPushButton:hover { background: #2ea043; }
)";

static const char *largeGreenBtn = R"(
    QPushButton { background: #238636; color: #fff; border: none; border-radius: 6px;
                  padding: 12px 32px; font-size: 16px; font-weight: 600; }
    QPushButton:hover { background: #2ea043; }
)";

static const char *cardStyle = R"(
    QFrame { background: #161b22; border: 1px solid #30363d; border-radius: 6px; }
    QFrame:hover { border-color: #58a6ff; }
)";

static const char *inputDark = R"(
    QLineEdit { padding: 10px 14px; background: #0d1117; color: #e6edf3;
                border: 1px solid #30363d; border-radius: 6px; font-size: 15px; }
    QLineEdit:focus { border-color: #58a6ff; }
)";

static const char *signInInput = R"(
    QLineEdit { padding: 8px 14px; background: #0d1117; color: #e6edf3;
                border: 1px solid #30363d; border-radius: 6px; font-size: 14px; }
    QLineEdit:focus { border-color: #1f6feb; }
)";

static const char *tableDark = R"(
    QTableWidget { background: #161b22; border: 1px solid #30363d; border-radius: 6px;
                   gridline-color: #21262d; color: #e6edf3; }
    QHeaderView::section { background: #0d1117; color: #e6edf3; padding: 10px 14px;
                           border: none; border-bottom: 1px solid #30363d; font-weight: 600; }
    QTableWidget::item { padding: 10px 14px; }
)";

static const char *treeDark = R"(
    QTreeWidget { background: #161b22; border: 1px solid #30363d; border-radius: 6px;
                  color: #e6edf3; }
    QHeaderView::section { background: #0d1117; color: #e6edf3; padding: 10px 14px;
                           border: none; border-bottom: 1px solid #30363d; font-weight: 600; }
    QTreeWidget::item { padding: 8px 14px; }
    QTreeWidget::item:hover { background: #1c2128; }
)";

// ─── HomePage ───────────────────────────────────────────────────────────────

HomePage::HomePage(QWidget *parent) : QWidget(parent) { setupUi(); }

void HomePage::setupUi()
{
    auto *main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    auto *webView = new QWebEngineView(this);
    auto *page = new GitHubWebPage(webView);
    webView->setPage(page);

    QWebEngineProfile *profile = page->profile();
    profile->setHttpCacheType(QWebEngineProfile::DiskHttpCache);
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    webView->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);

    webView->load(QUrl("local:///"));

    main->addWidget(webView);
}

// ─── SignInPage ─────────────────────────────────────────────────────────────

SignInPage::SignInPage(QWidget *parent) : QWidget(parent) { setupUi(); }

void SignInPage::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *wrap = new QVBoxLayout(this);
    wrap->setAlignment(Qt::AlignCenter);

    auto *box = new QFrame();
    box->setFixedWidth(380);
    box->setStyleSheet("background: #161b22; border: 1px solid #30363d; border-radius: 12px;");
    auto *lay = new QVBoxLayout(box);
    lay->setContentsMargins(28, 28, 28, 28);
    lay->setSpacing(16);

    auto *title = new QLabel("Sign in to GitHub Client");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 16, QFont::Bold));
    title->setStyleSheet(QString("color: %1;").arg(GH::HEADER_TEXT));
    lay->addWidget(title);

    auto *tokenLabel = new QLabel("Personal access token");
    tokenLabel->setStyleSheet(QString("color: %1; font-size: 13px; font-weight: 600;").arg(GH::HEADER_TEXT));
    lay->addWidget(tokenLabel);

    m_tokenInput = new QLineEdit();
    m_tokenInput->setPlaceholderText("ghp_xxxxxxxxxxxx");
    m_tokenInput->setEchoMode(QLineEdit::Password);
    m_tokenInput->setStyleSheet(signInInput);
    lay->addWidget(m_tokenInput);

    auto *cookieRow = new QHBoxLayout();
    auto *cookieLabel = new QLabel("Cookie (optional)");
    cookieLabel->setStyleSheet(QString("color: %1; font-size: 13px;").arg(GH::TEXT_SECONDARY));
    cookieRow->addWidget(cookieLabel);
    cookieRow->addStretch();

    m_cookieInput = new QLineEdit();
    m_cookieInput->setPlaceholderText("Optional");
    m_cookieInput->setStyleSheet(signInInput);
    lay->addWidget(m_cookieInput);

    auto *importBtn = new QPushButton("Import from browser");
    importBtn->setStyleSheet("QPushButton { color: #58a6ff; background: transparent; "
                             "border: none; font-size: 12px; } QPushButton:hover { text-decoration: underline; }");
    connect(importBtn, &QPushButton::clicked, this, &SignInPage::importFromBrowser);
    lay->addWidget(importBtn, 0, Qt::AlignRight);

    auto *signInBtn = new QPushButton("Sign in");
    signInBtn->setStyleSheet(greenBtn);
    signInBtn->setFixedHeight(40);
    connect(signInBtn, &QPushButton::clicked, this, &SignInPage::signInRequested);
    lay->addWidget(signInBtn);

    m_statusLabel = new QLabel();
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet("font-size: 13px;");
    lay->addWidget(m_statusLabel);

    auto *tip = new QLabel("Generate token at:\ngithub.com/settings/tokens");
    tip->setAlignment(Qt::AlignCenter);
    tip->setStyleSheet(QString("color: %1; font-size: 12px;").arg(GH::TEXT_SECONDARY));
    tip->setWordWrap(true);
    lay->addWidget(tip);

    wrap->addWidget(box);

    auto *backRow = new QHBoxLayout();
    backRow->setAlignment(Qt::AlignCenter);
    auto *backBtn = new QPushButton("← Back to home");
    backBtn->setFlat(true);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(QString("color: %1; font-size: 13px;").arg(GH::LINK));
    connect(backBtn, &QPushButton::clicked, this, &SignInPage::backRequested);
    backRow->addWidget(backBtn);
    wrap->addLayout(backRow);
}

void SignInPage::importFromBrowser()
{
    BrowserCookieDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted && !dlg.selectedCookies().isEmpty()) {
        m_cookieInput->setText(BrowserCookieExtractor::formatCookieString(dlg.selectedCookies()));
    }
}

QString SignInPage::getToken() const { return m_tokenInput->text().trimmed(); }
QString SignInPage::getCookie() const { return m_cookieInput->text().trimmed(); }
QString SignInPage::getLanguage() const {     return "zh"; }

void SignInPage::setStatus(const QString &msg, bool error)
{
    m_statusLabel->setText(msg);
    m_statusLabel->setStyleSheet(QString("color: %1; font-size: 13px;").arg(error ? GH::DANGER : GH::TEXT_SECONDARY));
}

// ─── BrowserCookieDialog ────────────────────────────────────────────────────

BrowserCookieDialog::BrowserCookieDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Import Cookies");
    setMinimumSize(400, 280);
    setStyleSheet("background: #161b22;");

    auto *lay = new QVBoxLayout(this);
    auto *t = new QLabel("Select a browser to import GitHub cookies");
    t->setStyleSheet("color: #e6edf3; font-size: 14px;");
    t->setAlignment(Qt::AlignCenter);
    lay->addWidget(t);

    m_statusLabel = new QLabel();
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #8b949e;");
    lay->addWidget(m_statusLabel);

    for (const auto &b : {"Chrome", "Edge", "Firefox"}) {
        auto *btn = new QPushButton(b);
        btn->setStyleSheet("QPushButton { color: #e6edf3; border: 1px solid #30363d; border-radius: 6px; "
                           "padding: 10px; font-size: 14px; } QPushButton:hover { border-color: #58a6ff; }");
        connect(btn, &QPushButton::clicked, this, [this, b = QString(b).toLower()]() { onBrowserSelected(b); });
        lay->addWidget(btn);
    }

    auto *cancel = new QPushButton("Cancel");
    cancel->setStyleSheet("QPushButton { color: #8b949e; border: 1px solid #30363d; border-radius: 6px; "
                          "padding: 8px; } QPushButton:hover { color: #e6edf3; }");
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
    lay->addWidget(cancel);
}

void BrowserCookieDialog::onBrowserSelected(const QString &browser)
{
    m_statusLabel->setText("Extracting from " + browser + "...");
    m_statusLabel->setStyleSheet("color: #58a6ff;");

    BrowserCookieExtractor extractor;
    QMap<QString, QString> cookies;
    if (browser == "chrome") cookies = extractor.getGithubCookieFromChrome();
    else if (browser == "edge") cookies = extractor.getGithubCookieFromEdge();
    else if (browser == "firefox") cookies = extractor.getGithubCookieFromFirefox();

    if (!cookies.isEmpty()) {
        m_selectedCookies = cookies;
        m_statusLabel->setText("Successfully imported from " + browser);
        m_statusLabel->setStyleSheet("color: #3fb950;");
        QTimer::singleShot(600, this, &QDialog::accept);
    } else {
        m_statusLabel->setText("No GitHub cookies found in " + browser);
        m_statusLabel->setStyleSheet("color: #f85149;");
    }
}

// ─── RepoCard ───────────────────────────────────────────────────────────────

RepoCard::RepoCard(const QJsonObject &repoData, QWidget *parent)
    : ClickableFrame(parent), m_repoData(repoData) { setupUi(); }

void RepoCard::setupUi()
{
    setStyleSheet(cardStyle);
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(18, 18, 18, 18);
    lay->setSpacing(8);

    auto *name = new QLabel(m_repoData["full_name"].toString("Unknown"));
    name->setFont(QFont("Arial", 15, QFont::Bold));
    name->setStyleSheet(QString("color: %1;").arg(GH::LINK));
    lay->addWidget(name);

    QString desc = m_repoData["description"].toString();
    if (desc.isEmpty()) desc = "No description";
    auto *d = new QLabel(desc);
    d->setWordWrap(true);
    d->setStyleSheet(QString("color: %1; font-size: 13px;").arg(GH::TEXT_SECONDARY));
    lay->addWidget(d);

    auto *stats = new QHBoxLayout();
    stats->setSpacing(16);
    auto add = [&](const QString &prefix, int v) {
        auto *l = new QLabel(QString("%1: %2").arg(prefix).arg(v));
        l->setStyleSheet(QString("color: %1; font-size: 12px;").arg(GH::TEXT_SECONDARY));
        stats->addWidget(l);
    };
    add("Stars", m_repoData["stargazers_count"].toInt());
    add("Forks", m_repoData["forks_count"].toInt());
    add("Issues", m_repoData["open_issues_count"].toInt());
    stats->addStretch();
    lay->addLayout(stats);

    connect(this, &ClickableFrame::clicked, this, [this]() { emit clicked(m_repoData); });
}

// ─── RepoFilesView ──────────────────────────────────────────────────────────

RepoFilesView::RepoFilesView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QWidget(parent), m_api(api), m_owner(owner), m_repo(repo), m_branch("main") { setupUi(); loadFiles(); }

void RepoFilesView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 10, 0, 0);

    auto *hdr = new QHBoxLayout();
    m_pathLabel = new QLabel("/");
    m_pathLabel->setFont(QFont("Arial", 13, QFont::Bold));
    m_pathLabel->setStyleSheet(QString("color: %1;").arg(GH::HEADER_TEXT));
    hdr->addWidget(m_pathLabel);
    hdr->addStretch();
    lay->addLayout(hdr);

    m_fileTree = new QTreeWidget();
    m_fileTree->setColumnCount(3);
    m_fileTree->setHeaderLabels({"Name", "Size", "Updated"});
    m_fileTree->setColumnWidth(0, 350);
    m_fileTree->setColumnWidth(1, 90);
    m_fileTree->setStyleSheet(treeDark);
    connect(m_fileTree, &QTreeWidget::itemDoubleClicked, this, &RepoFilesView::onItemDoubleClicked);
    lay->addWidget(m_fileTree);

    m_loading = new LoadingLabel("Loading files...", this);
    lay->addWidget(m_loading);
    m_loading->hide();
}

void RepoFilesView::loadFiles()
{
    m_fileTree->hide();
    m_loading->show();
    m_api->getRepositoryContents(m_owner, m_repo, m_currentPath, m_branch,
        [this](const QJsonArray &c) { onFilesLoaded(c); },
        [this](const QString &e) { onError(e); });
}

void RepoFilesView::onFilesLoaded(const QJsonArray &contents)
{
    m_loading->hide();
    m_fileTree->show();
    m_fileTree->clear();
    for (const auto &v : contents) {
        auto obj = v.toObject();
        auto *it = new QTreeWidgetItem();
        bool isDir = obj["type"].toString() == "dir";
        it->setText(0, QString(isDir ? "[D] " : "[F] ") + obj["name"].toString());
        it->setText(1, formatSize(obj["size"].toInt()));
        it->setText(2, obj["html_url"].toString().section('/', -1));
        it->setData(0, Qt::UserRole, QJsonDocument(obj).toJson(QJsonDocument::Compact));
        if (isDir) it->setForeground(0, QColor("#58a6ff"));
        m_fileTree->addTopLevelItem(it);
    }
}

void RepoFilesView::onItemDoubleClicked(QTreeWidgetItem *item, int)
{
    QJsonObject obj = QJsonDocument::fromJson(item->data(0, Qt::UserRole).toByteArray()).object();
    if (obj["type"].toString() == "dir") {
        m_currentPath = obj["path"].toString();
        m_pathLabel->setText("/" + m_currentPath);
        loadFiles();
    } else {
        showFileContent(obj);
    }
}

void RepoFilesView::showFileContent(const QJsonObject &fd)
{
    auto *dlg = new QDialog(this);
    dlg->setWindowTitle(fd["name"].toString());
    dlg->setMinimumSize(800, 600);
    dlg->setStyleSheet("background: #0d1117;");
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    auto *lay = new QVBoxLayout(dlg);
    auto *te = new QTextEdit();
    te->setReadOnly(true);
    te->setFont(QFont("Consolas", 11));
    te->setStyleSheet("background: #0d1117; color: #e6edf3; border: none;");
    te->setPlainText("Loading...");

    auto *closeBtn = new QPushButton("Close");
    closeBtn->setStyleSheet(outlineBtn);
    connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::close);

    lay->addWidget(te);
    lay->addWidget(closeBtn);
    dlg->exec();

    m_api->getFileContent(m_owner, m_repo, fd["path"].toString(), m_branch,
        [te](const QJsonObject &data) {
            QByteArray b64 = data["content"].toString().toUtf8();
            te->setPlainText(QString::fromUtf8(QByteArray::fromBase64(b64)));
        },
        [te](const QString &e) { te->setPlainText("Error: " + e); }
    );
}

void RepoFilesView::onError(const QString &e) { m_loading->hide(); m_fileTree->hide(); }
QString RepoFilesView::formatSize(int s) {
    if (s <= 0) return "-";
    if (s < 1024) return QString::number(s) + " B";
    if (s < 1048576) return QString::number(s / 1024.0, 'f', 1) + " KB";
    return QString::number(s / 1048576.0, 'f', 1) + " MB";
}

// ─── RepoIssuesView ─────────────────────────────────────────────────────────

RepoIssuesView::RepoIssuesView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QWidget(parent), m_api(api), m_owner(owner), m_repo(repo) { setupUi(); loadIssues(); }

void RepoIssuesView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 10, 0, 0);

    m_stack = new QStackedWidget();
    m_loading = new LoadingLabel("Loading issues...");
    m_table = new QTableWidget();
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Title", "Author", "State", "Updated"});
    m_table->setStyleSheet(tableDark);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setAlternatingRowColors(true);

    m_stack->addWidget(m_loading);
    m_stack->addWidget(m_table);
    lay->addWidget(m_stack);
}

void RepoIssuesView::loadIssues()
{
    m_stack->setCurrentIndex(0);
    m_api->getIssues(m_owner, m_repo, "open",
        [this](const QJsonArray &i) { onIssuesLoaded(i); },
        [this](const QString &e) { onError(e); });
}

void RepoIssuesView::onIssuesLoaded(const QJsonArray &issues)
{
    m_table->setRowCount(issues.size());
    for (int r = 0; r < issues.size(); ++r) {
        auto obj = issues[r].toObject();
        auto add = [&](int c, const QString &t, const QString &color = "") {
            auto *it = new QTableWidgetItem(t);
            it->setFlags(it->flags() & ~Qt::ItemIsEditable);
            if (!color.isEmpty()) it->setForeground(QColor(color));
            m_table->setItem(r, c, it);
        };
        add(0, obj["title"].toString());
        add(1, obj["user"].toObject()["login"].toString());
        bool open = obj["state"].toString() == "open";
        add(2, open ? "Open" : "Closed", open ? GH::SUCCESS : GH::DANGER);
        add(3, obj["updated_at"].toString().left(10));
    }
    m_stack->setCurrentIndex(1);
}

void RepoIssuesView::onError(const QString &) { m_loading->setLoadingText("Failed to load issues"); }

// ─── RepoPRsView ────────────────────────────────────────────────────────────

RepoPRsView::RepoPRsView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QWidget(parent), m_api(api), m_owner(owner), m_repo(repo) { setupUi(); loadPRs(); }

void RepoPRsView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 10, 0, 0);

    m_stack = new QStackedWidget();
    m_loading = new LoadingLabel("Loading pull requests...");
    m_table = new QTableWidget();
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Title", "Author", "State", "Updated"});
    m_table->setStyleSheet(tableDark);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setAlternatingRowColors(true);

    m_stack->addWidget(m_loading);
    m_stack->addWidget(m_table);
    lay->addWidget(m_stack);
}

void RepoPRsView::loadPRs()
{
    m_stack->setCurrentIndex(0);
    m_api->getPullRequests(m_owner, m_repo, "open",
        [this](const QJsonArray &p) { onPRsLoaded(p); },
        [this](const QString &e) { onError(e); });
}

void RepoPRsView::onPRsLoaded(const QJsonArray &prs)
{
    m_table->setRowCount(prs.size());
    for (int r = 0; r < prs.size(); ++r) {
        auto obj = prs[r].toObject();
        auto add = [&](int c, const QString &t, const QString &color = "") {
            auto *it = new QTableWidgetItem(t);
            it->setFlags(it->flags() & ~Qt::ItemIsEditable);
            if (!color.isEmpty()) it->setForeground(QColor(color));
            m_table->setItem(r, c, it);
        };
        add(0, obj["title"].toString());
        add(1, obj["user"].toObject()["login"].toString());

        QString state, color;
        if (!obj["merged_at"].toString().isEmpty()) { state = "Merged"; color = GH::PURPLE; }
        else if (obj["draft"].toBool()) { state = "Draft"; color = GH::TEXT_SECONDARY; }
        else { state = "Open"; color = GH::SUCCESS; }
        add(2, state, color);
        add(3, obj["updated_at"].toString().left(10));
    }
    m_stack->setCurrentIndex(1);
}

void RepoPRsView::onError(const QString &) { m_loading->setLoadingText("Failed to load PRs"); }

// ─── RepositoryDetailView ───────────────────────────────────────────────────

RepositoryDetailView::RepositoryDetailView(GitHubAPI *api, const QJsonObject &repoData, QWidget *parent)
    : QWidget(parent), m_api(api), m_repoData(repoData) { setupUi(); }

void RepositoryDetailView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *main = new QVBoxLayout(this);
    main->setContentsMargins(24, 16, 24, 16);

    auto *hdr = new QHBoxLayout();
    auto *back = new QPushButton("← Back");
    back->setStyleSheet(outlineBtn);
    connect(back, &QPushButton::clicked, this, &RepositoryDetailView::backRequested);
    hdr->addWidget(back);

    QString owner = m_repoData["owner"].toObject()["login"].toString();
    QString name = m_repoData["name"].toString();
    auto *title = new QLabel(owner + "/" + name);
    title->setFont(QFont("Arial", 22, QFont::Bold));
    title->setStyleSheet(QString("color: %1;").arg(GH::LINK));
    hdr->addWidget(title);
    hdr->addStretch();

    auto *open = new QPushButton("Open in browser");
    open->setStyleSheet(blueBtn);
    connect(open, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl(m_repoData["html_url"].toString()));
    });
    hdr->addWidget(open);
    main->addLayout(hdr);

    auto *tabs = new QTabWidget();
    tabs->setStyleSheet(QString(R"(
        QTabWidget::pane { border: none; }
        QTabBar::tab { color: %1; padding: 8px 18px; border: none; font-size: 14px; }
        QTabBar::tab:selected { color: %2; border-bottom: 2px solid #f78166; }
        QTabBar::tab:hover { color: %2; }
    )").arg(GH::TEXT_SECONDARY, GH::HEADER_TEXT));
    QString o = m_repoData["owner"].toObject()["login"].toString();
    QString r = m_repoData["name"].toString();
    tabs->addTab(new RepoFilesView(m_api, o, r), "[F] Files");
    tabs->addTab(new RepoIssuesView(m_api, o, r), "[I] Issues");
    tabs->addTab(new RepoPRsView(m_api, o, r), "[P] Pull Requests");
    main->addWidget(tabs);
}

// ─── RepositoriesView ───────────────────────────────────────────────────────

RepositoriesView::RepositoriesView(GitHubAPI *api, QWidget *parent)
    : QWidget(parent), m_api(api) { setupUi(); loadRepositories(); }

void RepositoriesView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(24, 16, 24, 16);

    auto *hdr = new QHBoxLayout();
    auto *t = new QLabel("Public Repositories");
    t->setFont(QFont("Arial", 20, QFont::Bold));
    t->setStyleSheet(QString("color: %1;").arg(GH::HEADER_TEXT));
    hdr->addWidget(t);
    hdr->addStretch();

    auto *refresh = new QPushButton("Refresh");
    refresh->setStyleSheet(outlineBtn);
    connect(refresh, &QPushButton::clicked, this, &RepositoriesView::loadRepositories);
    hdr->addWidget(refresh);
    lay->addLayout(hdr);

    m_stack = new QStackedWidget();
    m_loading = new LoadingLabel("Loading repositories...");
    auto *cardsW = new QWidget();
    m_cardsLayout = new QVBoxLayout(cardsW);
    m_cardsLayout->setSpacing(14);
    m_cardsLayout->addStretch();

    m_scroll = new QScrollArea();
    m_scroll->setWidget(cardsW);
    m_scroll->setWidgetResizable(true);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_stack->addWidget(m_loading);
    m_stack->addWidget(m_scroll);
    lay->addWidget(m_stack);
}

void RepositoriesView::loadRepositories()
{
    m_stack->setCurrentIndex(0);
    // Remove old cards (keep stretch)
    while (m_cardsLayout->count() > 1) {
        auto *item = m_cardsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    m_api->getRepositories("", "updated",
        [this](const QJsonArray &r) { onReposLoaded(r); },
        [this](const QString &e) { onError(e); });
}

void RepositoriesView::onReposLoaded(const QJsonArray &repos)
{
    while (m_cardsLayout->count() > 0) {
        auto *item = m_cardsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    for (const auto &v : repos) {
        auto *card = new RepoCard(v.toObject());
        connect(card, &RepoCard::clicked, this, &RepositoriesView::repoClicked);
        m_cardsLayout->addWidget(card);
    }
    m_cardsLayout->addStretch();
    m_stack->setCurrentIndex(1);
}

void RepositoriesView::onError(const QString &e) { m_loading->setLoadingText("Error: " + e); }

// ─── ProfileView ────────────────────────────────────────────────────────────

ProfileView::ProfileView(GitHubAPI *api, const QJsonObject &userData, QWidget *parent)
    : QWidget(parent), m_api(api), m_userData(userData) { setupUi(); }

void ProfileView::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(40, 30, 40, 30);

    auto *top = new QHBoxLayout();
    top->setSpacing(30);

    auto *avatar = new QLabel();
    avatar->setFixedSize(200, 200);
    avatar->setStyleSheet("background: #161b22; border: 1px solid #30363d; border-radius: 100px;");
    avatar->setAlignment(Qt::AlignCenter);

    QString aUrl = m_userData["avatar_url"].toString();
    if (!aUrl.isEmpty()) {
        auto *nm = new QNetworkAccessManager(this);
        QNetworkReply *rep = nm->get(QNetworkRequest(QUrl(aUrl)));
        connect(rep, &QNetworkReply::finished, this, [rep, avatar]() {
            QPixmap pm;
            if (pm.loadFromData(rep->readAll()))
                avatar->setPixmap(pm.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            rep->deleteLater();
        });
    }
    top->addWidget(avatar);

    auto *info = new QWidget();
    auto *il = new QVBoxLayout(info);
    QString nm = m_userData["name"].toString();
    if (nm.isEmpty()) nm = m_userData["login"].toString();
    auto *nl = new QLabel(nm);
    nl->setFont(QFont("Arial", 26, QFont::Bold));
    nl->setStyleSheet(QString("color: %1;").arg(GH::HEADER_TEXT));
    il->addWidget(nl);

    auto *ll = new QLabel("@" + m_userData["login"].toString());
    ll->setStyleSheet(QString("color: %1; font-size: 18px;").arg(GH::TEXT_SECONDARY));
    il->addWidget(ll);

    QString bio = m_userData["bio"].toString();
    if (!bio.isEmpty()) {
        auto *bl = new QLabel(bio);
        bl->setWordWrap(true);
        bl->setStyleSheet(QString("color: %1;").arg(GH::TEXT_SECONDARY));
        il->addWidget(bl);
    }
    il->addStretch();
    top->addWidget(info);
    lay->addLayout(top);

    auto *stats = new QHBoxLayout();
    stats->setSpacing(24);
    auto addStat = [&](const QString &label, int v) {
        auto *l = new QLabel(QString("%1 %2").arg(v).arg(label));
        l->setStyleSheet(QString("color: %1; font-size: 14px;").arg(GH::TEXT_SECONDARY));
        stats->addWidget(l);
    };
    addStat("followers", m_userData["followers"].toInt());
    addStat("following", m_userData["following"].toInt());
    addStat("repos", m_userData["public_repos"].toInt());
    stats->addStretch();
    lay->addLayout(stats);

    lay->addStretch();
}

// ─── DashboardPage ──────────────────────────────────────────────────────────

DashboardPage::DashboardPage(GitHubAPI *api, const QJsonObject &user, QWidget *parent)
    : QWidget(parent), m_api(api), m_user(user) { setupUi(); }

void DashboardPage::setupUi()
{
    setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);

    m_contentStack = new QStackedWidget();
    m_contentStack->setStyleSheet(QString("background: %1;").arg(GH::MAIN_BG));

    auto *mainWidget = new QWidget();
    auto *ml = new QVBoxLayout(mainWidget);
    ml->setContentsMargins(0, 0, 0, 0);

    auto *tabs = new QTabWidget();
    tabs->setStyleSheet(QString(R"(
        QTabWidget::pane { border: none; }
        QTabBar::tab { color: %1; padding: 10px 22px; border: none; font-size: 14px;
                       border-bottom: 2px solid transparent; }
        QTabBar::tab:selected { color: %2; border-bottom-color: #f78166; }
        QTabBar::tab:hover { color: %2; }
    )").arg(GH::TEXT_SECONDARY, GH::HEADER_TEXT));

    auto *rv = new RepositoriesView(m_api);
    connect(rv, &RepositoriesView::repoClicked, this, &DashboardPage::repoClicked);
    tabs->addTab(rv, "[R] Repositories");
    tabs->addTab(new ProfileView(m_api, m_user), "[P] Profile");
    ml->addWidget(tabs);
    m_contentStack->addWidget(mainWidget);
    m_contentStack->setCurrentIndex(0);

    lay->addWidget(m_contentStack);
}

void DashboardPage::openRepositoryDetail(const QJsonObject &repo)
{
    auto *dv = new RepositoryDetailView(m_api, repo);
    connect(dv, &RepositoryDetailView::backRequested, this, &DashboardPage::goBack);
    m_repoStack.append(m_contentStack->currentWidget());
    m_contentStack->addWidget(dv);
    m_contentStack->setCurrentIndex(m_contentStack->count() - 1);
}

void DashboardPage::goBack()
{
    if (!m_repoStack.isEmpty()) {
        auto *cur = m_contentStack->currentWidget();
        m_contentStack->removeWidget(cur);
        cur->deleteLater();
        auto *prev = m_repoStack.takeLast();
        m_contentStack->setCurrentWidget(prev);
    }
}

// ─── MainWindow ─────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_api(nullptr) { setupUi(); }

void MainWindow::setupUi()
{
    setWindowTitle("GitHub Client");
    setMinimumSize(1200, 800);
    setStyleSheet(QString("QMainWindow { background: %1; }").arg(GH::MAIN_BG));

    auto *central = new QWidget();
    setCentralWidget(central);
    auto *main = new QVBoxLayout(central);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    // Create headers
    m_headerLoggedOut = createHeaderLoggedOut();
    m_headerLoggedIn = createHeaderLoggedIn();
    main->addWidget(m_headerLoggedOut);
    m_headerLoggedIn->hide();

    // Page stack
    m_pageStack = new QStackedWidget();
    m_pageStack->setObjectName("pageStack");

    m_homePage = new HomePage();
    m_signInPage = new SignInPage();

    m_pageStack->addWidget(m_homePage);     // 0
    m_pageStack->addWidget(m_signInPage);   // 1

    connect(m_homePage, &HomePage::signInClicked, this, &MainWindow::goToSignIn);
    connect(m_homePage, &HomePage::signUpClicked, this, &MainWindow::goToSignIn);
    connect(m_signInPage, &SignInPage::signInRequested, this, &MainWindow::doLogin);
    connect(m_signInPage, &SignInPage::backRequested, this, &MainWindow::goToHome);

    main->addWidget(m_pageStack);
}

QWidget* MainWindow::createHeaderLoggedOut()
{
    auto *hdr = new QFrame();
    hdr->setStyleSheet(QString("background: %1; padding: 10px 24px;").arg(GH::HEADER_BG));
    auto *lay = new QHBoxLayout(hdr);
    lay->setContentsMargins(0, 0, 0, 0);

    auto *logo = new QPushButton("GitHub Client");
    logo->setFlat(true);
    logo->setCursor(Qt::PointingHandCursor);
    logo->setFont(QFont("Arial", 18, QFont::Bold));
    logo->setStyleSheet(QString("color: %1; font-weight: bold;").arg(GH::HEADER_TEXT));
    connect(logo, &QPushButton::clicked, this, &MainWindow::goToHome);
    lay->addWidget(logo);

    QStringList navs = {"Product", "Solutions", "Resources", "Open Source", "Enterprise", "Pricing"};
    for (const auto &n : navs) {
        auto *btn = new NavButton(n);
        lay->addWidget(btn);
    }

    lay->addStretch();

    auto *searchBtn = new NavButton("Search");
    searchBtn->setStyleSheet(QString("QPushButton { color: %1; font-size: 14px; background: transparent; "
                                     "border: 1px solid %2; border-radius: 6px; padding: 4px 12px; } "
                                     "QPushButton:hover { border-color: %3; }")
                             .arg(GH::TEXT_SECONDARY, GH::BORDER, GH::TEXT_SECONDARY));
    connect(searchBtn, &QPushButton::clicked, this, []() {});
    lay->addWidget(searchBtn);

    auto *signIn = new QPushButton("Sign in");
    signIn->setStyleSheet(outlineBtn);
    connect(signIn, &QPushButton::clicked, this, &MainWindow::goToSignIn);
    lay->addWidget(signIn);

    auto *signUp = new QPushButton("Sign up");
    signUp->setStyleSheet(blueBtn);
    connect(signUp, &QPushButton::clicked, this, []() {});
    lay->addWidget(signUp);

    return hdr;
}

QWidget* MainWindow::createHeaderLoggedIn()
{
    auto *hdr = new QFrame();
    hdr->setStyleSheet(QString("background: %1; padding: 10px 24px;").arg(GH::HEADER_BG));
    auto *lay = new QHBoxLayout(hdr);
    lay->setContentsMargins(0, 0, 0, 0);

    auto *logo = new QPushButton("GitHub Client");
    logo->setFlat(true);
    logo->setFont(QFont("Arial", 18, QFont::Bold));
    logo->setStyleSheet(QString("color: %1;").arg(GH::HEADER_TEXT));
    connect(logo, &QPushButton::clicked, this, &MainWindow::goToDashboard);
    lay->addWidget(logo);

    auto *search = new QLineEdit();
    search->setPlaceholderText("Search or jump to...");
    search->setFixedWidth(280);
    search->setStyleSheet(QString("QLineEdit { background: #0d1117; color: #e6edf3; border: 1px solid %1; "
                                  "border-radius: 6px; padding: 5px 12px; font-size: 13px; } "
                                  "QLineEdit:focus { border-color: #1f6feb; width: 400px; }").arg(GH::BORDER));
    connect(search, &QLineEdit::returnPressed, this, []() {});
    lay->addWidget(search);

    QStringList navs = {"Pull requests", "Issues", "Marketplace", "Explore"};
    for (const auto &n : navs) {
        auto *btn = new NavButton(n);
        lay->addWidget(btn);
    }

    lay->addStretch();

    m_headerUserLabel = new QLabel();
    m_headerUserLabel->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: 600;").arg(GH::HEADER_TEXT));
    lay->addWidget(m_headerUserLabel);

    auto *logout = new QPushButton("Sign out");
    logout->setStyleSheet(outlineBtn);
    connect(logout, &QPushButton::clicked, this, &MainWindow::doLogout);
    lay->addWidget(logout);

    return hdr;
}

void MainWindow::goToHome()
{
    m_pageStack->setCurrentIndex(0);
    m_headerLoggedOut->show();
    m_headerLoggedIn->hide();
}

void MainWindow::goToSignIn()
{
    m_pageStack->setCurrentIndex(1);
}

void MainWindow::doLogin()
{
    QString token = m_signInPage->getToken();
    QString cookie = m_signInPage->getCookie();
    QString lang = m_signInPage->getLanguage();

    if (token.isEmpty()) {
        m_signInPage->setStatus("Please enter a token", true);
        return;
    }

    m_signInPage->setStatus("Signing in...");
    setLanguage(lang);
    m_api = new GitHubAPI(token, cookie, this);

    m_api->getUser(
        [this](const QJsonObject &user) {
            m_user = user;
            m_signInPage->setStatus("");

            // Remove old dashboard
            while (m_pageStack->count() > 2) {
                auto *w = m_pageStack->widget(2);
                m_pageStack->removeWidget(w);
                w->deleteLater();
            }

            m_dashboardPage = new DashboardPage(m_api, m_user);
            connect(m_dashboardPage, &DashboardPage::repoClicked, this, &MainWindow::openRepoDetail);
            m_pageStack->addWidget(m_dashboardPage);

            goToDashboard();
        },
        [this](const QString &e) {
            m_api->deleteLater();
            m_api = nullptr;
            m_signInPage->setStatus("Invalid token: " + e, true);
        }
    );
}

void MainWindow::goToDashboard()
{
    m_pageStack->setCurrentIndex(2);
    m_headerLoggedOut->hide();
    m_headerLoggedIn->show();
    m_headerUserLabel->setText(m_user["login"].toString());
}

void MainWindow::doLogout()
{
    auto reply = QMessageBox::question(this, "Sign out", "Are you sure you want to sign out?",
                                        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    if (m_api) { m_api->deleteLater(); m_api = nullptr; }
    m_user = QJsonObject();
    goToHome();
}

void MainWindow::openRepoDetail(const QJsonObject &repo)
{
    if (m_dashboardPage) {
        m_dashboardPage->openRepositoryDetail(repo);
    }
}
