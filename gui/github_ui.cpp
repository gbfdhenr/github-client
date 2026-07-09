#include "gui/github_ui.h"
#include "utils/browser_cookies.h"

#include <QApplication>
#include <QMessageBox>
#include <QFont>
#include <QColor>
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

static const char *cardStyle = R"(
    QFrame {
        background-color: white;
        border: 1px solid #d0d7de;
        border-radius: 6px;
    }
    QFrame:hover {
        border-color: #0969da;
    }
)";

static const char *treeStyle = R"(
    QTreeWidget {
        border: 1px solid #d0d7de;
        border-radius: 6px;
    }
    QHeaderView::section {
        background-color: #f6f8fa;
        padding: 8px;
        border: none;
        border-bottom: 1px solid #d0d7de;
        font-weight: bold;
    }
)";

static const char *tableStyle = R"(
    QTableWidget {
        border: 1px solid #d0d7de;
        border-radius: 6px;
        gridline-color: #f6f8fa;
    }
    QHeaderView::section {
        background-color: #f6f8fa;
        padding: 8px;
        border: none;
        border-bottom: 1px solid #d0d7de;
        font-weight: bold;
    }
)";

static const char *primaryBtnStyle = R"(
    QPushButton {
        background-color: #2da44e;
        color: white;
        padding: 10px 20px;
        border: none;
        border-radius: 6px;
        font-size: 14px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #2c974b;
    }
)";

static const char *secondaryBtnStyle = R"(
    QPushButton {
        padding: 8px 16px;
        border: 1px solid #d0d7de;
        border-radius: 6px;
        background-color: #f6f8fa;
    }
    QPushButton:hover {
        background-color: #e5e7eb;
    }
)";

static const char *inputStyle = R"(
    QLineEdit {
        padding: 8px;
        border: 1px solid #d0d7de;
        border-radius: 6px;
        font-size: 14px;
    }
    QLineEdit:focus {
        border-color: #0969da;
        outline: none;
    }
)";

static const char *blueBtnStyle = R"(
    QPushButton {
        padding: 5px 15px;
        border: 1px solid #0969da;
        border-radius: 6px;
        background-color: #0969da;
        color: white;
    }
    QPushButton:hover {
        background-color: #0856b6;
    }
)";

static const char *greenBtnStyle = R"(
    QPushButton {
        background-color: #2da44e;
        color: white;
        padding: 8px 16px;
        border: none;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #2c974b;
    }
)";

// ─── AvatarLabel ────────────────────────────────────────────────────────────

AvatarLabel::AvatarLabel(QWidget *parent)
    : QLabel(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

void AvatarLabel::loadImage(const QString &url)
{
    if (url.isEmpty()) return;
    QNetworkRequest request{QUrl(url)};
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { onImageLoaded(reply); });
}

void AvatarLabel::onImageLoaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QPixmap pixmap;
        if (pixmap.loadFromData(reply->readAll())) {
            setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    reply->deleteLater();
}

// ─── BrowserCookieDialog ────────────────────────────────────────────────────

BrowserCookieDialog::BrowserCookieDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void BrowserCookieDialog::setupUi()
{
    setWindowTitle("从浏览器导入 Cookie");
    setMinimumSize(450, 300);

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 30, 30, 30);

    auto *title = new QLabel("从浏览器导入 GitHub Cookie");
    title->setFont(QFont("Arial", 16, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    auto *desc = new QLabel("选择一个浏览器以自动提取 GitHub Cookie\n无需手动输入 Token 或 Cookie");
    desc->setAlignment(Qt::AlignCenter);
    desc->setStyleSheet("color: #57606a; margin-bottom: 10px;");
    layout->addWidget(desc);

    auto *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(10);

    struct BrowserInfo { QString id; QString name; QString icon; };
    QVector<BrowserInfo> browsers = {
        {"chrome", "Google Chrome", "[C]"},
        {"edge", "Microsoft Edge", "[E]"},
        {"firefox", "Mozilla Firefox", "[F]"}
    };

    for (const auto &b : browsers) {
        auto *btn = new QPushButton(b.icon + " " + b.name);
        btn->setStyleSheet(R"(
            QPushButton {
                padding: 12px 20px;
                border: 1px solid #d0d7de;
                border-radius: 6px;
                background-color: #f6f8fa;
                font-size: 14px;
                text-align: left;
            }
            QPushButton:hover {
                background-color: #e5e7eb;
                border-color: #0969da;
            }
        )");
        connect(btn, &QPushButton::clicked, this, [this, name = b.id]() {
            onBrowserSelected(name);
        });
        btnLayout->addWidget(btn);
    }
    layout->addLayout(btnLayout);

    auto *cancelBtn = new QPushButton("取消");
    cancelBtn->setStyleSheet(R"(
        QPushButton {
            padding: 10px;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            background-color: white;
        }
        QPushButton:hover {
            background-color: #f6f8fa;
        }
    )");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(cancelBtn);

    m_statusLabel = new QLabel("");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #cf222e;");
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);
}

void BrowserCookieDialog::onBrowserSelected(const QString &browser)
{
    m_statusLabel->setText("正在从 " + browser + " 提取 Cookie...");
    m_statusLabel->setStyleSheet("color: #0969da;");

    BrowserCookieExtractor extractor;
    QMap<QString, QString> cookies;

    if (browser == "chrome")
        cookies = extractor.getGithubCookieFromChrome();
    else if (browser == "edge")
        cookies = extractor.getGithubCookieFromEdge();
    else if (browser == "firefox")
        cookies = extractor.getGithubCookieFromFirefox();

    if (!cookies.isEmpty()) {
        m_selectedCookies = cookies;
        m_statusLabel->setText("成功从 " + browser + " 提取 Cookie!");
        m_statusLabel->setStyleSheet("color: #1a7f37;");
        QTimer::singleShot(800, this, &QDialog::accept);
    } else {
        m_statusLabel->setText("未从 " + browser + " 中找到 GitHub Cookie");
        m_statusLabel->setStyleSheet("color: #cf222e;");
    }
}

// ─── LoginDialog ────────────────────────────────────────────────────────────

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void LoginDialog::setupUi()
{
    setWindowTitle(getText("app_title"));
    setMinimumSize(450, 350);

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 30, 30, 30);

    auto *title = new QLabel("GitHub Client");
    title->setFont(QFont("Arial", 18, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #24292f; margin-bottom: 10px;");
    layout->addWidget(title);

    auto *tokenLabel = new QLabel(getText("token_label"));
    tokenLabel->setStyleSheet("color: #24292f; font-weight: bold;");
    layout->addWidget(tokenLabel);

    m_tokenInput = new QLineEdit();
    m_tokenInput->setPlaceholderText("ghp_xxxxxxxxxxxx");
    m_tokenInput->setStyleSheet(inputStyle);
    layout->addWidget(m_tokenInput);

    auto *cookieLayout = new QHBoxLayout();
    auto *cookieLabel = new QLabel(getText("cookie_label"));
    cookieLabel->setStyleSheet("color: #24292f;");
    cookieLayout->addWidget(cookieLabel);
    cookieLayout->addStretch();

    m_cookieInput = new QLineEdit();
    m_cookieInput->setPlaceholderText("可选，从浏览器导入");
    m_cookieInput->setStyleSheet(inputStyle);
    cookieLayout->addWidget(m_cookieInput);

    auto *importBtn = new QPushButton("[Web] 从浏览器导入");
    importBtn->setStyleSheet(blueBtnStyle);
    connect(importBtn, &QPushButton::clicked, this, &LoginDialog::importFromBrowser);
    cookieLayout->addWidget(importBtn);
    layout->addLayout(cookieLayout);

    auto *langLayout = new QHBoxLayout();
    auto *langLabel = new QLabel(getText("language_label"));
    m_langCombo = new QComboBox();
    m_langCombo->addItems({"中文", "English"});
    m_langCombo->setCurrentIndex(0);
    langLayout->addWidget(langLabel);
    langLayout->addWidget(m_langCombo);
    layout->addLayout(langLayout);

    auto *tipLabel = new QLabel("提示：可通过 Token 页面生成 https://github.com/settings/tokens");
    tipLabel->setStyleSheet("color: #57606a; font-size: 12px;");
    tipLabel->setWordWrap(true);
    layout->addWidget(tipLabel);

    auto *loginBtn = new QPushButton(getText("login_button"));
    loginBtn->setStyleSheet(primaryBtnStyle);
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(loginBtn);

    m_statusLabel = new QLabel("");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_statusLabel);
}

void LoginDialog::importFromBrowser()
{
    BrowserCookieDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted && !dialog.selectedCookies().isEmpty()) {
        QString cookieStr = BrowserCookieExtractor::formatCookieString(dialog.selectedCookies());
        m_cookieInput->setText(cookieStr);
        m_statusLabel->setText("Cookie 已成功导入");
        m_statusLabel->setStyleSheet("color: #1a7f37;");
    }
}

QString LoginDialog::getToken() const { return m_tokenInput->text().trimmed(); }
QString LoginDialog::getCookie() const { return m_cookieInput->text().trimmed(); }
QString LoginDialog::getLanguage() const { return m_langCombo->currentIndex() == 0 ? "zh" : "en"; }

// ─── RepoCard ───────────────────────────────────────────────────────────────

RepoCard::RepoCard(const QJsonObject &repoData, std::function<void(const QJsonObject &)> onClick)
    : QFrame()
    , m_repoData(repoData)
    , m_onClick(std::move(onClick))
{
    setupUi();
}

void RepoCard::setupUi()
{
    setStyleSheet(cardStyle);

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 16, 16, 16);

    auto *nameLabel = new QLabel(m_repoData["full_name"].toString("Unknown"));
    nameLabel->setFont(QFont("Arial", 16, QFont::Bold));
    nameLabel->setStyleSheet("color: #0969da;");
    nameLabel->setCursor(Qt::PointingHandCursor);
    layout->addWidget(nameLabel);

    QString desc = m_repoData["description"].toString();
    if (desc.isEmpty()) desc = getText("no_description");
    auto *descLabel = new QLabel(desc);
    descLabel->setStyleSheet("color: #57606a;");
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    auto *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(15);

    auto addStat = [&](const QString &label, int value) {
        auto *lbl = new QLabel(label + ": " + QString::number(value));
        lbl->setStyleSheet("color: #57606a;");
        statsLayout->addWidget(lbl);
    };

    addStat("Stars", m_repoData["stargazers_count"].toInt());
    addStat("Forks", m_repoData["forks_count"].toInt());
    addStat("Issues", m_repoData["open_issues_count"].toInt());

    statsLayout->addStretch();
    layout->addLayout(statsLayout);

    QString updated = m_repoData["updated_at"].toString().left(10);
    if (!updated.isEmpty()) {
        auto *timeLabel = new QLabel("更新于：" + updated);
        timeLabel->setStyleSheet("color: #57606a; font-size: 12px;");
        layout->addWidget(timeLabel);
    }

    // Make the entire card clickable via the name label
    connect(nameLabel, &QLabel::linkActivated, this, [this]() {});
    // Use mouse press event on the card itself
    setMouseTracking(true);
    nameLabel->installEventFilter(this);
}

// ─── RepoFilesView ──────────────────────────────────────────────────────────

RepoFilesView::RepoFilesView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QWidget(parent)
    , m_api(api)
    , m_owner(owner)
    , m_repo(repo)
    , m_currentPath("")
    , m_branch("main")
{
    setupUi();
    loadFiles();
}

void RepoFilesView::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    auto *header = new QHBoxLayout();
    m_pathLabel = new QLabel("/");
    m_pathLabel->setFont(QFont("Arial", 12, QFont::Bold));
    header->addWidget(m_pathLabel);
    header->addStretch();

    auto *branchLabel = new QLabel("分支：" + m_branch);
    branchLabel->setStyleSheet("color: #57606a;");
    header->addWidget(branchLabel);
    layout->addLayout(header);

    m_fileTree = new QTreeWidget();
    m_fileTree->setColumnCount(3);
    m_fileTree->setHeaderLabels({"文件名", "大小", "更新时间"});
    m_fileTree->setColumnWidth(0, 300);
    m_fileTree->setColumnWidth(1, 100);
    m_fileTree->setColumnWidth(2, 150);
    m_fileTree->setStyleSheet(treeStyle);

    connect(m_fileTree, &QTreeWidget::itemDoubleClicked, this, &RepoFilesView::onItemDoubleClicked);
    layout->addWidget(m_fileTree);
}

void RepoFilesView::loadFiles()
{
    m_fileTree->clear();

    m_api->getRepositoryContents(m_owner, m_repo, m_currentPath, m_branch,
        [this](const QJsonArray &contents) { onFilesLoaded(contents); },
        [this](const QString &error) { onError(error); }
    );
}

void RepoFilesView::onFilesLoaded(const QJsonArray &contents)
{
    m_fileTree->clear();
    for (const QJsonValue &val : contents) {
        QJsonObject item = val.toObject();
        auto *treeItem = new QTreeWidgetItem();

        QString icon = item["type"].toString() == "dir" ? "[D]" : "[F]";
        treeItem->setText(0, icon + " " + item["name"].toString());
        treeItem->setText(1, formatSize(item["size"].toInt()));

        QString url = item["html_url"].toString();
        treeItem->setText(2, url.section('/', -1));

        treeItem->setData(0, Qt::UserRole, QJsonDocument(item).toJson(QJsonDocument::Compact));
        m_fileTree->addTopLevelItem(treeItem);
    }
}

void RepoFilesView::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QByteArray data = item->data(0, Qt::UserRole).toByteArray();
    QJsonObject obj = QJsonDocument::fromJson(data).object();

    if (obj["type"].toString() == "file") {
        showFileContent(obj);
    } else if (obj["type"].toString() == "dir") {
        m_currentPath = obj["path"].toString();
        m_pathLabel->setText("/" + m_currentPath);
        loadFiles();
    }
}

void RepoFilesView::showFileContent(const QJsonObject &fileData)
{
    QString path = fileData["path"].toString();
    QString filename = fileData["name"].toString();

    m_api->getFileContent(m_owner, m_repo, path, m_branch,
        [this, filename](const QJsonObject &data) {
            auto *dialog = new QDialog(this);
            dialog->setWindowTitle(filename);
            dialog->setMinimumSize(800, 600);
            dialog->setAttribute(Qt::WA_DeleteOnClose);

            auto *layout = new QVBoxLayout(dialog);
            auto *content = new QTextEdit();
            content->setReadOnly(true);
            content->setFont(QFont("Consolas", 10));

            QString decoded = data["decoded_content"].toString();
            if (decoded.isEmpty()) {
                QByteArray b64 = data["content"].toString().toUtf8();
                decoded = QString::fromUtf8(QByteArray::fromBase64(b64));
            }
            content->setPlainText(decoded);
            layout->addWidget(content);

            auto *closeBtn = new QPushButton("关闭");
            closeBtn->setStyleSheet(secondaryBtnStyle);
            connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::close);
            layout->addWidget(closeBtn);

            dialog->exec();
        },
        [this](const QString &error) { onError(error); }
    );
}

void RepoFilesView::onError(const QString &error)
{
    QMessageBox::critical(this, "错误", "加载失败：" + error);
}

QString RepoFilesView::formatSize(int size)
{
    if (size <= 0) return "-";
    if (size < 1024) return QString::number(size) + " B";
    if (size < 1024 * 1024) return QString::number(size / 1024.0, 'f', 1) + " KB";
    return QString::number(size / (1024.0 * 1048576.0), 'f', 1) + " MB";
}

// ─── RepoIssuesView ─────────────────────────────────────────────────────────

RepoIssuesView::RepoIssuesView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QTableWidget(parent)
    , m_api(api)
    , m_owner(owner)
    , m_repo(repo)
{
    setupUi();
    loadIssues();
}

void RepoIssuesView::setupUi()
{
    setColumnCount(4);
    setHorizontalHeaderLabels({"标题", "作者", "状态", "更新时间"});
    setStyleSheet(tableStyle);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    setAlternatingRowColors(true);
}

void RepoIssuesView::loadIssues()
{
    m_api->getIssues(m_owner, m_repo, "open",
        [this](const QJsonArray &issues) { onIssuesLoaded(issues); },
        [this](const QString &error) { onError(error); }
    );
}

void RepoIssuesView::onIssuesLoaded(const QJsonArray &issues)
{
    setRowCount(issues.size());
    for (int row = 0; row < issues.size(); ++row) {
        QJsonObject issue = issues[row].toObject();

        auto *titleItem = new QTableWidgetItem(issue["title"].toString());
        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 0, titleItem);

        QString author = issue["user"].toObject()["login"].toString();
        auto *authorItem = new QTableWidgetItem(author);
        authorItem->setFlags(authorItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 1, authorItem);

        bool isOpen = issue["state"].toString() == "open";
        auto *stateItem = new QTableWidgetItem(isOpen ? "Open" : "Closed");
        stateItem->setFlags(stateItem->flags() & ~Qt::ItemIsEditable);
        stateItem->setForeground(isOpen ? QColor("#1a7f37") : QColor("#cf222e"));
        setItem(row, 2, stateItem);

        QString updated = issue["updated_at"].toString().left(10);
        auto *dateItem = new QTableWidgetItem(updated);
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 3, dateItem);
    }
}

void RepoIssuesView::onError(const QString &error)
{
    setRowCount(1);
    auto *errItem = new QTableWidgetItem("加载失败：" + error);
    errItem->setForeground(QColor("#cf222e"));
    setItem(0, 0, errItem);
}

// ─── RepoPRsView ────────────────────────────────────────────────────────────

RepoPRsView::RepoPRsView(GitHubAPI *api, const QString &owner, const QString &repo, QWidget *parent)
    : QTableWidget(parent)
    , m_api(api)
    , m_owner(owner)
    , m_repo(repo)
{
    setupUi();
    loadPRs();
}

void RepoPRsView::setupUi()
{
    setColumnCount(4);
    setHorizontalHeaderLabels({"标题", "作者", "状态", "更新时间"});
    setStyleSheet(tableStyle);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    setAlternatingRowColors(true);
}

void RepoPRsView::loadPRs()
{
    m_api->getPullRequests(m_owner, m_repo, "open",
        [this](const QJsonArray &prs) { onPRsLoaded(prs); },
        [this](const QString &error) { onError(error); }
    );
}

void RepoPRsView::onPRsLoaded(const QJsonArray &prs)
{
    setRowCount(prs.size());
    for (int row = 0; row < prs.size(); ++row) {
        QJsonObject pr = prs[row].toObject();

        auto *titleItem = new QTableWidgetItem(pr["title"].toString());
        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 0, titleItem);

        QString author = pr["user"].toObject()["login"].toString();
        auto *authorItem = new QTableWidgetItem(author);
        authorItem->setFlags(authorItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 1, authorItem);

        QString state;
        QString color;
        if (!pr["merged_at"].toString().isEmpty()) {
            state = "Merged";
            color = "#8250df";
        } else if (pr["draft"].toBool()) {
            state = "Draft";
            color = "#57606a";
        } else {
            state = "Open";
            color = "#1a7f37";
        }

        auto *stateItem = new QTableWidgetItem(state);
        stateItem->setFlags(stateItem->flags() & ~Qt::ItemIsEditable);
        stateItem->setForeground(QColor(color));
        setItem(row, 2, stateItem);

        QString updated = pr["updated_at"].toString().left(10);
        auto *dateItem = new QTableWidgetItem(updated);
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        setItem(row, 3, dateItem);
    }
}

void RepoPRsView::onError(const QString &error)
{
    setRowCount(1);
    auto *errItem = new QTableWidgetItem("加载失败：" + error);
    errItem->setForeground(QColor("#cf222e"));
    setItem(0, 0, errItem);
}

// ─── RepositoryDetailView ───────────────────────────────────────────────────

RepositoryDetailView::RepositoryDetailView(GitHubAPI *api, const QJsonObject &repoData,
                                             std::function<void()> onBack, QWidget *parent)
    : QWidget(parent)
    , m_api(api)
    , m_repoData(repoData)
    , m_onBack(std::move(onBack))
{
    setupUi();
}

void RepositoryDetailView::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    auto *header = new QHBoxLayout();

    auto *backBtn = new QPushButton("< 返回");
    backBtn->setStyleSheet(secondaryBtnStyle);
    connect(backBtn, &QPushButton::clicked, this, &RepositoryDetailView::goBack);
    header->addWidget(backBtn);

    QString owner = m_repoData["owner"].toObject()["login"].toString();
    QString name = m_repoData["name"].toString();
    auto *title = new QLabel(owner + "/" + name);
    title->setFont(QFont("Arial", 24, QFont::Bold));
    header->addWidget(title);
    header->addStretch();

    auto *openBrowserBtn = new QPushButton("在浏览器中打开");
    openBrowserBtn->setStyleSheet(greenBtnStyle);
    connect(openBrowserBtn, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl(m_repoData["html_url"].toString()));
    });
    header->addWidget(openBrowserBtn);
    mainLayout->addLayout(header);

    auto *tabs = new QTabWidget();
    tabs->addTab(new RepoFilesView(m_api, owner, name), "[F] 文件");
    tabs->addTab(new RepoIssuesView(m_api, owner, name), "[I] Issues");
    tabs->addTab(new RepoPRsView(m_api, owner, name), "[P] Pull Requests");
    mainLayout->addWidget(tabs);
}

void RepositoryDetailView::goBack()
{
    if (m_onBack) m_onBack();
}

// ─── RepositoriesView ───────────────────────────────────────────────────────

RepositoriesView::RepositoriesView(GitHubAPI *api, QWidget *parent)
    : QWidget(parent)
    , m_api(api)
{
    setupUi();
    loadRepositories();
}

void RepositoriesView::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    auto *headerLayout = new QHBoxLayout();
    auto *title = new QLabel(getText("public_repos"));
    title->setFont(QFont("Arial", 20, QFont::Bold));
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    auto *refreshBtn = new QPushButton(getText("refresh_button"));
    refreshBtn->setStyleSheet(R"(
        QPushButton {
            padding: 5px 15px;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            background-color: #f3f4f6;
        }
        QPushButton:hover {
            background-color: #e5e7eb;
        }
    )");
    connect(refreshBtn, &QPushButton::clicked, this, &RepositoriesView::loadRepositories);
    headerLayout->addWidget(refreshBtn);
    layout->addLayout(headerLayout);

    m_scroll = new QScrollArea();
    m_scroll->setWidgetResizable(true);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(m_scroll);

    auto *cardsWidget = new QWidget();
    cardsWidget->setStyleSheet("background-color: #ffffff;");
    m_cardsLayout = new QVBoxLayout(cardsWidget);
    m_cardsLayout->setSpacing(15);
    m_cardsLayout->addStretch();
    m_scroll->setWidget(cardsWidget);
}

void RepositoriesView::loadRepositories()
{
    while (m_cardsLayout->count() > 1) {
        QLayoutItem *item = m_cardsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    auto *loading = new QLabel(getText("loading"));
    loading->setAlignment(Qt::AlignCenter);
    m_cardsLayout->insertWidget(0, loading);

    m_api->getRepositories("", "updated",
        [this](const QJsonArray &repos) { onReposLoaded(repos); },
        [this](const QString &error) { onError(error); }
    );
}

void RepositoriesView::onReposLoaded(const QJsonArray &repos)
{
    while (m_cardsLayout->count() > 0) {
        QLayoutItem *item = m_cardsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (const QJsonValue &val : repos) {
        QJsonObject repo = val.toObject();
        auto *card = new RepoCard(repo, [this](const QJsonObject &r) {
            emit repoClicked(r);
        });
        m_cardsLayout->addWidget(card);
    }

    m_cardsLayout->addStretch();
}

void RepositoriesView::onError(const QString &error)
{
    while (m_cardsLayout->count() > 0) {
        QLayoutItem *item = m_cardsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    auto *errorLabel = new QLabel(getText("error_title") + ": " + error);
    errorLabel->setStyleSheet("color: #cf222e;");
    m_cardsLayout->addWidget(errorLabel);
}

// ─── ProfileView ────────────────────────────────────────────────────────────

ProfileView::ProfileView(GitHubAPI *api, const QJsonObject &userData, QWidget *parent)
    : QWidget(parent)
    , m_api(api)
    , m_userData(userData)
{
    setupUi();
}

void ProfileView::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    setStyleSheet("background-color: white;");

    auto *infoLayout = new QHBoxLayout();
    infoLayout->setSpacing(30);

    auto *avatar = new AvatarLabel();
    avatar->setFixedSize(200, 200);
    avatar->setStyleSheet(R"(
        background-color: #f6f8fa;
        border: 1px solid #d0d7de;
        border-radius: 100px;
    )");
    avatar->setAlignment(Qt::AlignCenter);

    QString avatarUrl = m_userData["avatar_url"].toString();
    if (!avatarUrl.isEmpty()) {
        avatar->loadImage(avatarUrl);
    } else {
        avatar->setText("No Avatar");
        avatar->setStyleSheet("color: #57606a;");
    }
    infoLayout->addWidget(avatar);

    auto *infoWidget = new QWidget();
    auto *infoInner = new QVBoxLayout(infoWidget);
    infoInner->setSpacing(10);

    QString displayName = m_userData["name"].toString();
    if (displayName.isEmpty()) displayName = m_userData["login"].toString("Unknown");
    auto *nameLabel = new QLabel(displayName);
    nameLabel->setFont(QFont("Arial", 24, QFont::Bold));
    infoInner->addWidget(nameLabel);

    QString login = m_userData["login"].toString();
    if (!login.isEmpty()) {
        auto *loginLabel = new QLabel("@" + login);
        loginLabel->setStyleSheet("color: #57606a;");
        infoInner->addWidget(loginLabel);
    }

    QString bio = m_userData["bio"].toString();
    if (!bio.isEmpty()) {
        auto *bioLabel = new QLabel(bio);
        bioLabel->setWordWrap(true);
        bioLabel->setStyleSheet("color: #57606a;");
        infoInner->addWidget(bioLabel);
    }

    infoLayout->addWidget(infoWidget);
    layout->addLayout(infoLayout);

    auto *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20);
    statsLayout->addWidget(new QLabel(QString::number(m_userData["followers"].toInt()) + " 关注者"));
    statsLayout->addWidget(new QLabel(QString::number(m_userData["following"].toInt()) + " 正在关注"));
    statsLayout->addWidget(new QLabel(QString::number(m_userData["public_repos"].toInt()) + " 公开仓库"));
    statsLayout->addStretch();
    layout->addLayout(statsLayout);

    auto *extraLayout = new QHBoxLayout();
    extraLayout->setSpacing(20);

    auto addExtra = [&](const QString &icon, const QString &key) {
        QString val = m_userData[key].toString();
        if (!val.isEmpty())
            extraLayout->addWidget(new QLabel(icon + " " + val));
    };

    addExtra("[C]", "company");
    addExtra("[L]", "location");
    addExtra("[E]", "email");
    addExtra("[W]", "blog");
    extraLayout->addStretch();
    layout->addLayout(extraLayout);

    QString createdAt = m_userData["created_at"].toString().left(10);
    if (!createdAt.isEmpty()) {
        layout->addWidget(new QLabel("成员于 " + createdAt + " 加入"));
    }

    layout->addStretch();
}

// ─── MainWindow ─────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_api(nullptr)
{
    setupUi();
}

void MainWindow::setupUi()
{
    setWindowTitle(getText("app_title"));
    setMinimumSize(1200, 800);

    auto *centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupHeader(mainLayout);

    m_contentStack = new QStackedWidget();
    m_contentStack->setStyleSheet("background-color: #ffffff;");
    mainLayout->addWidget(m_contentStack);
}

void MainWindow::setupHeader(QVBoxLayout *layout)
{
    auto *header = new QFrame();
    header->setStyleSheet(QString(R"(
        QFrame {
            background-color: %1;
            padding: 10px;
        }
    )").arg(GitHubColors::HEADER_BG));

    auto *headerLayout = new QHBoxLayout(header);

    auto *logo = new QLabel("GitHub Client");
    logo->setFont(QFont("Arial", 18, QFont::Bold));
    logo->setStyleSheet(QString("color: %1;").arg(GitHubColors::HEADER_TEXT));
    headerLayout->addWidget(logo);

    headerLayout->addStretch();

    m_langCombo = new QComboBox();
    m_langCombo->addItems({"中文", "English"});
    m_langCombo->setStyleSheet("color: white; background-color: transparent; border: none;");
    headerLayout->addWidget(m_langCombo);

    m_userLabel = new QLabel("");
    m_userLabel->setStyleSheet(QString("color: %1;").arg(GitHubColors::HEADER_TEXT));
    headerLayout->addWidget(m_userLabel);

    m_logoutButton = new QPushButton(getText("logout_button"));
    m_logoutButton->setStyleSheet(R"(
        QPushButton {
            color: white;
            background-color: transparent;
            border: 1px solid white;
            border-radius: 6px;
            padding: 5px 15px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");
    connect(m_logoutButton, &QPushButton::clicked, this, &MainWindow::logout);
    headerLayout->addWidget(m_logoutButton);

    layout->addWidget(header);
}

bool MainWindow::login(const QString &token, const QString &cookie, const QString &lang)
{
    setLanguage(lang);

    m_api = new GitHubAPI(token, cookie, this);

    m_api->getUser(
        [this](const QJsonObject &user) {
            m_user = user;
            QString loginName = m_user["login"].toString();
            m_userLabel->setText(getText("welcome").arg(loginName));

            auto *reposView = new RepositoriesView(m_api);
            auto *profileView = new ProfileView(m_api, m_user);

            m_contentStack->addWidget(nullptr); // clear by adding dummy
            while (m_contentStack->count() > 0) {
                QWidget *w = m_contentStack->widget(0);
                m_contentStack->removeWidget(w);
                if (w) w->deleteLater();
            }

            auto *tabs = new QTabWidget();
            tabs->setStyleSheet(R"(
                QTabWidget::pane {
                    border: none;
                    border-top: 1px solid #d0d7de;
                }
                QTabBar::tab {
                    padding: 10px 20px;
                    border: none;
                    border-bottom: 2px solid transparent;
                }
                QTabBar::tab:selected {
                    border-bottom-color: #FD8D77;
                    color: #24292f;
                }
                QTabBar::tab:hover {
                    background-color: #f6f8fa;
                }
            )");
            tabs->addTab(reposView, "[R] " + getText("repositories_tab"));
            tabs->addTab(profileView, "[P] " + getText("profile_tab"));

            connect(reposView, &RepositoriesView::repoClicked, this, &MainWindow::openRepositoryDetail);

            m_contentStack->addWidget(tabs);
        },
        [this](const QString &) {
            m_api->deleteLater();
            m_api = nullptr;
            QMessageBox::critical(this, getText("error_title"), getText("token_invalid"));
        }
    );
    return true;
}

void MainWindow::logout()
{
    auto reply = QMessageBox::question(this, getText("confirm_logout"), "",
                                        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (m_api) {
            m_api->deleteLater();
            m_api = nullptr;
        }
        m_user = QJsonObject();
        m_repoStack.clear();

        while (m_contentStack->count() > 0) {
            QWidget *w = m_contentStack->widget(0);
            m_contentStack->removeWidget(w);
            if (w) w->deleteLater();
        }
        m_contentStack->addWidget(new QWidget());

        showLoginDialog();
    }
}

void MainWindow::openRepositoryDetail(const QJsonObject &repo)
{
    auto *detailView = new RepositoryDetailView(m_api, repo, [this]() { goBack(); });
    m_repoStack.append(m_contentStack->currentWidget());
    m_contentStack->addWidget(detailView);
    m_contentStack->setCurrentIndex(m_contentStack->count() - 1);
}

void MainWindow::goBack()
{
    if (!m_repoStack.isEmpty()) {
        QWidget *current = m_contentStack->currentWidget();
        m_contentStack->removeWidget(current);
        current->deleteLater();

        QWidget *prev = m_repoStack.takeLast();
        m_contentStack->addWidget(prev);
        m_contentStack->setCurrentWidget(prev);
    }
}

void MainWindow::showLoginDialog()
{
    LoginDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString token = dialog.getToken();
        QString cookie = dialog.getCookie();
        QString lang = dialog.getLanguage();

        if (!token.isEmpty()) {
            login(token, cookie, lang);
        } else {
            QMessageBox::warning(this, getText("error_title"), getText("login_required"));
        }
    }
}
