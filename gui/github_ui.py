import sys
import os
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QLineEdit, QPushButton, QTabWidget, QScrollArea,
    QStackedWidget, QComboBox, QMessageBox, QFrame, QSplitter,
    QTreeWidget, QTreeWidgetItem, QTextEdit, QTableWidget,
    QTableWidgetItem, QDialog, QDialogButtonBox, QFormLayout,
    QProgressBar, QListWidget, QListWidgetItem, QToolBar, QAction
)
from PyQt5.QtCore import Qt, QThread, pyqtSignal, QSize, QUrl, QTimer
from PyQt5.QtGui import QFont, QColor, QPalette, QIcon, QDesktopServices

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from i18n.translations import get_text, set_language, CURRENT_LANG
from api.github_api import GitHubAPI
from utils.browser_cookies import BrowserCookieExtractor, get_github_cookie_string


class GitHubColors:
    HEADER_BG = '#24292f'
    HEADER_TEXT = '#ffffff'
    SIDEBAR_BG = '#f6f8fa'
    MAIN_BG = '#ffffff'
    BORDER = '#d0d7de'
    BUTTON_PRIMARY = '#2da44e'
    BUTTON_PRIMARY_HOVER = '#2c974b'
    BUTTON_SECONDARY = '#f3f4f6'
    LINK = '#0969da'
    TEXT_PRIMARY = '#24292f'
    TEXT_SECONDARY = '#57606a'
    SUCCESS = '#1a7f37'
    DANGER = '#cf222e'
    WARNING = '#9a6700'


class LoadingThread(QThread):
    finished = pyqtSignal(object)
    error = pyqtSignal(str)

    def __init__(self, func, *args, **kwargs):
        super().__init__()
        self.func = func
        self.args = args
        self.kwargs = kwargs

    def run(self):
        try:
            result = self.func(*self.args, **self.kwargs)
            self.finished.emit(result)
        except Exception as e:
            self.error.emit(str(e))


class CookieLoadThread(QThread):
    """后台加载浏览器 Cookie 的线程"""
    finished = pyqtSignal(str, dict)
    error = pyqtSignal(str)

    def __init__(self, browser: str):
        super().__init__()
        self.browser = browser

    def run(self):
        try:
            extractor = BrowserCookieExtractor()
            
            if self.browser == 'chrome':
                cookies = extractor.get_github_cookie_from_chrome()
            elif self.browser == 'edge':
                cookies = extractor.get_github_cookie_from_edge()
            elif self.browser == 'firefox':
                cookies = extractor.get_github_cookie_from_firefox()
            else:
                cookies = None
            
            if cookies:
                self.finished.emit(self.browser, cookies)
            else:
                self.error.emit(f'未从 {self.browser} 中找到 GitHub Cookie')
        except Exception as e:
            self.error.emit(str(e))


class BrowserCookieDialog(QDialog):
    """浏览器 Cookie 选择对话框"""
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('从浏览器导入 Cookie')
        self.setMinimumSize(450, 300)
        self.selected_cookies = {}
        self.setup_ui()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(15)
        layout.setContentsMargins(30, 30, 30, 30)

        title = QLabel('从浏览器导入 GitHub Cookie')
        title.setFont(QFont('Arial', 16, QFont.Bold))
        title.setAlignment(Qt.AlignCenter)
        layout.addWidget(title)

        desc = QLabel('选择一个浏览器以自动提取 GitHub Cookie\n无需手动输入 Token 或 Cookie')
        desc.setAlignment(Qt.AlignCenter)
        desc.setStyleSheet('color: #57606a; margin-bottom: 10px;')
        layout.addWidget(desc)

        button_layout = QVBoxLayout()
        button_layout.setSpacing(10)

        browsers = [
            ('chrome', 'Google Chrome', '🌐'),
            ('edge', 'Microsoft Edge', '🌀'),
            ('firefox', 'Mozilla Firefox', '🦊')
        ]

        for browser_id, browser_name, icon in browsers:
            btn = QPushButton(f'{icon} {browser_name}')
            btn.setStyleSheet('''
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
            ''')
            btn.clicked.connect(lambda checked, bid=browser_id: self.on_browser_selected(bid))
            button_layout.addWidget(btn)

        layout.addLayout(button_layout)

        cancel_btn = QPushButton('取消')
        cancel_btn.setStyleSheet('''
            QPushButton {
                padding: 10px;
                border: 1px solid #d0d7de;
                border-radius: 6px;
                background-color: white;
            }
            QPushButton:hover {
                background-color: #f6f8fa;
            }
        ''')
        cancel_btn.clicked.connect(self.reject)
        layout.addWidget(cancel_btn)

        self.status_label = QLabel('')
        self.status_label.setAlignment(Qt.AlignCenter)
        self.status_label.setStyleSheet('color: #cf222e;')
        self.status_label.setWordWrap(True)
        layout.addWidget(self.status_label)

    def on_browser_selected(self, browser: str):
        self.status_label.setText(f'正在从 {browser} 提取 Cookie...')
        self.status_label.setStyleSheet('color: #0969da;')
        
        self.thread = CookieLoadThread(browser)
        self.thread.finished.connect(self.on_cookie_loaded)
        self.thread.error.connect(self.on_error)
        self.thread.start()

    def on_cookie_loaded(self, browser: str, cookies: dict):
        self.status_label.setText(f'✅ 成功从 {browser} 提取 Cookie!')
        self.status_label.setStyleSheet('color: #1a7f37;')
        
        self.selected_cookies = cookies
        QTimer.singleShot(800, self.accept)

    def on_error(self, error: str):
        self.status_label.setText(error)
        self.status_label.setStyleSheet('color: #cf222e;')


class LoginDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle(get_text('app_title'))
        self.setMinimumSize(450, 350)
        self.setup_ui()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        layout.setSpacing(15)
        layout.setContentsMargins(30, 30, 30, 30)

        title = QLabel('GitHub Client')
        title.setFont(QFont('Arial', 18, QFont.Bold))
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet('color: #24292f; margin-bottom: 10px;')
        layout.addWidget(title)

        token_label = QLabel(get_text('token_label'))
        token_label.setStyleSheet('color: #24292f; font-weight: bold;')
        layout.addWidget(token_label)

        self.token_input = QLineEdit()
        self.token_input.setPlaceholderText('ghp_xxxxxxxxxxxx')
        self.token_input.setStyleSheet('''
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
        ''')
        layout.addWidget(self.token_input)

        cookie_layout = QHBoxLayout()
        self.cookie_label = QLabel(get_text('cookie_label'))
        self.cookie_label.setStyleSheet('color: #24292f;')
        cookie_layout.addWidget(self.cookie_label)
        cookie_layout.addStretch()

        self.cookie_input = QLineEdit()
        self.cookie_input.setPlaceholderText('可选，从浏览器导入')
        self.cookie_input.setStyleSheet('''
            QLineEdit {
                padding: 8px;
                border: 1px solid #d0d7de;
                border-radius: 6px;
                font-size: 14px;
            }
        ''')
        cookie_layout.addWidget(self.cookie_input)

        self.import_cookie_btn = QPushButton('🌐 从浏览器导入')
        self.import_cookie_btn.setStyleSheet('''
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
        ''')
        self.import_cookie_btn.clicked.connect(self.import_from_browser)
        cookie_layout.addWidget(self.import_cookie_btn)

        layout.addLayout(cookie_layout)

        lang_layout = QHBoxLayout()
        lang_label = QLabel(get_text('language_label'))
        self.lang_combo = QComboBox()
        self.lang_combo.addItems(['中文', 'English'])
        self.lang_combo.setCurrentIndex(0)
        lang_layout.addWidget(lang_label)
        lang_layout.addWidget(self.lang_combo)
        layout.addLayout(lang_layout)

        tip_label = QLabel('💡 提示：可通过 Token 页面生成 https://github.com/settings/tokens')
        tip_label.setStyleSheet('color: #57606a; font-size: 12px;')
        tip_label.setWordWrap(True)
        layout.addWidget(tip_label)

        self.login_button = QPushButton(get_text('login_button'))
        self.login_button.setStyleSheet('''
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
        ''')
        self.login_button.clicked.connect(self.accept)
        layout.addWidget(self.login_button)

        self.status_label = QLabel('')
        self.status_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.status_label)

    def import_from_browser(self):
        dialog = BrowserCookieDialog(self)
        if dialog.exec_() == QDialog.Accepted and dialog.selected_cookies:
            extractor = BrowserCookieExtractor()
            cookie_string = extractor.format_cookie_string(dialog.selected_cookies)
            self.cookie_input.setText(cookie_string)
            self.status_label.setText('✅ Cookie 已成功导入')
            self.status_label.setStyleSheet('color: #1a7f37;')

    def get_token(self):
        return self.token_input.text().strip()

    def get_cookie(self):
        return self.cookie_input.text().strip()

    def get_language(self):
        return 'zh' if self.lang_combo.currentIndex() == 0 else 'en'


class RepoCard(QFrame):
    def __init__(self, repo_data, on_click=None):
        super().__init__()
        self.repo_data = repo_data
        self.on_click = on_click
        self.setup_ui()

    def setup_ui(self):
        self.setStyleSheet('''
            QFrame {
                background-color: white;
                border: 1px solid #d0d7de;
                border-radius: 6px;
            }
            QFrame:hover {
                border-color: #0969da;
            }
        ''')

        layout = QVBoxLayout(self)
        layout.setSpacing(8)
        layout.setContentsMargins(16, 16, 16, 16)

        name_label = QLabel(self.repo_data.get('full_name', 'Unknown'))
        name_label.setFont(QFont('Arial', 16, QFont.Bold))
        name_label.setStyleSheet(f'color: {GitHubColors.LINK};')
        name_label.setCursor(Qt.PointingHandCursor)
        name_label.mousePressEvent = lambda e: self.on_repo_click()
        layout.addWidget(name_label)

        desc = self.repo_data.get('description') or get_text('no_description')
        desc_label = QLabel(desc)
        desc_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY};')
        desc_label.setWordWrap(True)
        layout.addWidget(desc_label)

        stats_layout = QHBoxLayout()
        stats_layout.setSpacing(15)

        stars = self.repo_data.get('stargazers_count', 0)
        stars_label = QLabel(f'⭐ {stars}')
        stars_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY};')
        stats_layout.addWidget(stars_label)

        forks = self.repo_data.get('forks_count', 0)
        forks_label = QLabel(f'🍴 {forks}')
        forks_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY};')
        stats_layout.addWidget(forks_label)

        issues = self.repo_data.get('open_issues_count', 0)
        issues_label = QLabel(f'📋 {issues}')
        issues_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY};')
        stats_layout.addWidget(issues_label)

        stats_layout.addStretch()
        layout.addLayout(stats_layout)

        updated = self.repo_data.get('updated_at', '')
        if updated:
            updated = updated[:10]
            time_label = QLabel(f'更新于：{updated}')
            time_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY}; font-size: 12px;')
            layout.addWidget(time_label)

    def on_repo_click(self):
        if self.on_click:
            self.on_click(self.repo_data)


class RepositoryDetailView(QWidget):
    def __init__(self, api: GitHubAPI, repo_data: dict):
        super().__init__()
        self.api = api
        self.repo_data = repo_data
        self.owner = repo_data.get('owner', {}).get('login', '')
        self.repo_name = repo_data.get('name', '')
        self.setup_ui()

    def setup_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(20, 20, 20, 20)
        main_layout.setSpacing(15)

        header = QHBoxLayout()
        title = QLabel(f'{self.owner}/{self.repo_name}')
        title.setFont(QFont('Arial', 24, QFont.Bold))
        header.addWidget(title)
        header.addStretch()

        open_browser_btn = QPushButton('在浏览器中打开')
        open_browser_btn.setStyleSheet('''
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
        ''')
        open_browser_btn.clicked.connect(lambda: QDesktopServices.openUrl(
            QUrl(self.repo_data.get('html_url', ''))
        ))
        header.addWidget(open_browser_btn)
        main_layout.addLayout(header)

        tabs = QTabWidget()
        tabs.addTab(RepoFilesView(self.api, self.owner, self.repo_name), '📁 文件')
        tabs.addTab(RepoIssuesView(self.api, self.owner, self.repo_name), '📋 Issues')
        tabs.addTab(RepoPRsView(self.api, self.owner, self.repo_name), '🔀 Pull Requests')
        main_layout.addWidget(tabs)


class RepoFilesView(QWidget):
    def __init__(self, api: GitHubAPI, owner: str, repo: str):
        super().__init__()
        self.api = api
        self.owner = owner
        self.repo = repo
        self.current_path = ''
        self.branch = 'main'
        self.setup_ui()
        self.load_files()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(10, 10, 10, 10)

        header = QHBoxLayout()
        self.path_label = QLabel('/')
        self.path_label.setFont(QFont('Arial', 12, QFont.Bold))
        header.addWidget(self.path_label)
        header.addStretch()
        layout.addLayout(header)

        self.file_tree = QTreeWidget()
        self.file_tree.setColumnCount(1)
        self.file_tree.setHeaderLabels(['文件名'])
        self.file_tree.setColumnWidth(0, 400)
        self.file_tree.setStyleSheet('''
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
        ''')
        self.file_tree.itemDoubleClicked.connect(self.on_item_double_click)
        layout.addWidget(self.file_tree)

    def load_files(self):
        self.thread = LoadingThread(
            self.api.get_repository_contents,
            self.owner, self.repo, self.current_path, self.branch
        )
        self.thread.finished.connect(self.on_files_loaded)
        self.thread.error.connect(self.on_error)
        self.thread.start()

    def on_files_loaded(self, contents):
        self.file_tree.clear()
        for item in contents:
            tree_item = QTreeWidgetItem()
            icon = '📁' if item['type'] == 'dir' else '📄'
            tree_item.setText(0, f'{icon} {item["name"]}')
            tree_item.setData(0, Qt.UserRole, item)
            self.file_tree.addTopLevelItem(tree_item)

    def on_item_double_click(self, item, column):
        data = item.data(0, Qt.UserRole)
        if data and data.get('type') == 'file':
            self.show_file_content(data)
        elif data and data.get('type') == 'dir':
            self.current_path = data.get('path', '')
            self.path_label.setText(f'/{self.current_path}')
            self.load_files()

    def show_file_content(self, file_data):
        path = file_data.get('path', '')
        thread = LoadingThread(
            self.api.get_file_content,
            self.owner, self.repo, path, self.branch
        )
        thread.finished.connect(lambda data: self.show_file_dialog(file_data.get('name', ''), data))
        thread.start()

    def show_file_dialog(self, filename, data):
        dialog = QDialog(self)
        dialog.setWindowTitle(filename)
        dialog.setMinimumSize(800, 600)
        
        layout = QVBoxLayout(dialog)
        content = QTextEdit()
        content.setReadOnly(True)
        content.setFont(QFont('Consolas', 10))
        
        decoded = data.get('decoded_content', data.get('content', ''))
        content.setPlainText(decoded)
        layout.addWidget(content)
        dialog.exec_()

    def on_error(self, error):
        QMessageBox.critical(self, '错误', f'加载失败：{error}')


class RepoIssuesView(QTableWidget):
    def __init__(self, api: GitHubAPI, owner: str, repo: str):
        super().__init__()
        self.api = api
        self.owner = owner
        self.repo = repo
        self.setup_ui()
        self.load_issues()

    def setup_ui(self):
        self.setColumnCount(4)
        self.setHorizontalHeaderLabels(['标题', '作者', '状态', '更新时间'])
        self.setStyleSheet('''
            QTableWidget {
                border: 1px solid #d0d7de;
                border-radius: 6px;
                gridline-color: #d0d7de;
                gridline-width: 1px;
                gridline-color: #f6f8fa;
            }
            QHeaderView::section {
                background-color: #f6f8fa;
                padding: 8px;
                border: none;
                border-bottom: 1px solid #d0d7de;
                font-weight: bold;
            }
        ''')
        self.horizontalHeader().setStretchLastSection(True)

    def load_issues(self):
        self.thread = LoadingThread(
            self.api.get_issues,
            self.owner, self.repo, 'open'
        )
        self.thread.finished.connect(self.on_issues_loaded)
        self.thread.error.connect(self.on_error)
        self.thread.start()

    def on_issues_loaded(self, issues):
        self.setRowCount(len(issues))
        for row, issue in enumerate(issues):
            self.setItem(row, 0, QTableWidgetItem(issue.get('title', '')))
            self.setItem(row, 1, QTableWidgetItem(issue.get('user', {}).get('login', '')))
            
            state = '🟢 Open' if issue.get('state') == 'open' else '🔴 Closed'
            self.setItem(row, 2, QTableWidgetItem(state))
            
            updated = issue.get('updated_at', '')[:10] if issue.get('updated_at') else ''
            self.setItem(row, 3, QTableWidgetItem(updated))

    def on_error(self, error):
        QMessageBox.critical(self, '错误', f'加载失败：{error}')


class RepoPRsView(QTableWidget):
    def __init__(self, api: GitHubAPI, owner: str, repo: str):
        super().__init__()
        self.api = api
        self.owner = owner
        self.repo = repo
        self.setup_ui()
        self.load_prs()

    def setup_ui(self):
        self.setColumnCount(4)
        self.setHorizontalHeaderLabels(['标题', '作者', '状态', '更新时间'])
        self.setStyleSheet('''
            QTableWidget {
                border: 1px solid #d0d7de;
                border-radius: 6px;
                gridline-color: #d0d7de;
            }
            QHeaderView::section {
                background-color: #f6f8fa;
                padding: 8px;
                border: none;
                border-bottom: 1px solid #d0d7de;
                font-weight: bold;
            }
        ''')
        self.horizontalHeader().setStretchLastSection(True)

    def load_prs(self):
        self.thread = LoadingThread(
            self.api.get_pull_requests,
            self.owner, self.repo, 'open'
        )
        self.thread.finished.connect(self.on_prs_loaded)
        self.thread.error.connect(self.on_error)
        self.thread.start()

    def on_prs_loaded(self, prs):
        self.setRowCount(len(prs))
        for row, pr in enumerate(prs):
            self.setItem(row, 0, QTableWidgetItem(pr.get('title', '')))
            self.setItem(row, 1, QTableWidgetItem(pr.get('user', {}).get('login', '')))
            
            state = '🟢 Open' if pr.get('merged_at') is None else '🟣 Merged'
            self.setItem(row, 2, QTableWidgetItem(state))
            
            updated = pr.get('updated_at', '')[:10] if pr.get('updated_at') else ''
            self.setItem(row, 3, QTableWidgetItem(updated))

    def on_error(self, error):
        QMessageBox.critical(self, '错误', f'加载失败：{error}')


class RepositoriesView(QWidget):
    def __init__(self, api: GitHubAPI):
        super().__init__()
        self.api = api
        self.setup_ui()
        self.load_repositories()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(20, 20, 20, 20)

        header_layout = QHBoxLayout()
        title = QLabel(get_text('public_repos'))
        title.setFont(QFont('Arial', 20, QFont.Bold))
        header_layout.addWidget(title)
        header_layout.addStretch()

        self.refresh_button = QPushButton(get_text('refresh_button'))
        self.refresh_button.setStyleSheet('''
            QPushButton {
                padding: 5px 15px;
                border: 1px solid #d0d7de;
                border-radius: 6px;
                background-color: #f3f4f6;
            }
            QPushButton:hover {
                background-color: #e5e7eb;
            }
        ''')
        self.refresh_button.clicked.connect(self.load_repositories)
        header_layout.addWidget(self.refresh_button)
        layout.addLayout(header_layout)

        self.scroll = QScrollArea()
        self.scroll.setWidgetResizable(True)
        self.scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        layout.addWidget(self.scroll)

        self.cards_widget = QWidget()
        self.cards_widget.setStyleSheet('background-color: #ffffff;')
        self.cards_layout = QVBoxLayout(self.cards_widget)
        self.cards_layout.setSpacing(15)
        self.cards_layout.addStretch()
        self.scroll.setWidget(self.cards_widget)

    def load_repositories(self):
        while self.cards_layout.count() > 1:
            item = self.cards_layout.takeAt(0)
            if item.widget():
                item.widget().deleteLater()

        loading = QLabel(get_text('loading'))
        loading.setAlignment(Qt.AlignCenter)
        self.cards_layout.insertWidget(0, loading)

        self.thread = LoadingThread(self.api.get_repositories)
        self.thread.finished.connect(self.on_repos_loaded)
        self.thread.error.connect(self.on_error)
        self.thread.start()

    def on_repos_loaded(self, repos):
        while self.cards_layout.count() > 0:
            item = self.cards_layout.takeAt(0)
            if item.widget():
                item.widget().deleteLater()

        for repo in repos:
            card = RepoCard(repo, on_click=self.on_repo_click)
            self.cards_layout.addWidget(card)

        self.cards_layout.addStretch()

    def on_error(self, error):
        while self.cards_layout.count() > 0:
            item = self.cards_layout.takeAt(0)
            if item.widget():
                item.widget().deleteLater()

        error_label = QLabel(f'{get_text("error_title")}: {error}')
        error_label.setStyleSheet(f'color: {GitHubColors.DANGER};')
        self.cards_layout.addWidget(error_label)

    def on_repo_click(self, repo):
        parent = self.parent()
        while parent:
            if isinstance(parent, QMainWindow):
                parent.open_repository_detail(repo)
                break
            parent = parent.parent()


class ProfileView(QWidget):
    def __init__(self, api: GitHubAPI, user_data):
        super().__init__()
        self.api = api
        self.user_data = user_data
        self.setup_ui()

    def setup_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(30, 30, 30, 30)
        self.setStyleSheet('background-color: white;')

        info_layout = QHBoxLayout()
        info_layout.setSpacing(30)

        avatar = QLabel()
        avatar.setFixedSize(200, 200)
        avatar.setStyleSheet('''
            background-color: #f6f8fa;
            border: 1px solid #d0d7de;
            border-radius: 100px;
        ''')
        
        avatar_url = self.user_data.get('avatar_url', '')
        if avatar_url:
            try:
                from PyQt5.QtGui import QPixmap, QNetworkAccessManager, QNetworkRequest
                pixmap = QPixmap()
                pixmap.loadFromData(QNetworkAccessManager().get(QNetworkRequest(QUrl(avatar_url))).readAll())
                avatar.setPixmap(pixmap.scaled(200, 200, Qt.KeepAspectRatio, Qt.SmoothTransformation))
            except Exception:
                pass
        
        info_layout.addWidget(avatar)

        info_widget = QWidget()
        info_layout_inner = QVBoxLayout(info_widget)
        info_layout_inner.setSpacing(10)

        name = self.user_data.get('name') or self.user_data.get('login', 'Unknown')
        name_label = QLabel(name)
        name_label.setFont(QFont('Arial', 24, QFont.Bold))
        info_layout_inner.addWidget(name_label)

        bio = self.user_data.get('bio')
        if bio:
            bio_label = QLabel(bio)
            bio_label.setWordWrap(True)
            bio_label.setStyleSheet(f'color: {GitHubColors.TEXT_SECONDARY};')
            info_layout_inner.addWidget(bio_label)

        info_layout.addWidget(info_widget)
        layout.addLayout(info_layout)

        stats_layout = QHBoxLayout()
        stats_layout.setSpacing(20)

        followers = self.user_data.get('followers', 0)
        following = self.user_data.get('following', 0)
        repos = self.user_data.get('public_repos', 0)

        stats_layout.addWidget(QLabel(f'👥 {followers} 关注者'))
        stats_layout.addWidget(QLabel(f'👤 {following} 正在关注'))
        stats_layout.addWidget(QLabel(f'📦 {repos} 公开仓库'))
        stats_layout.addStretch()
        layout.addLayout(stats_layout)

        layout.addStretch()


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.api = None
        self.user = None
        self.repo_stack = []
        self.setup_ui()

    def setup_ui(self):
        global CURRENT_LANG
        self.setWindowTitle(get_text('app_title'))
        self.setMinimumSize(1200, 800)

        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        self.setup_header(main_layout)

        self.content_stack = QStackedWidget()
        self.content_stack.setStyleSheet('background-color: #ffffff;')
        main_layout.addWidget(self.content_stack)

        self.login_widget = QWidget()
        self.content_stack.addWidget(self.login_widget)

    def setup_header(self, layout):
        header = QFrame()
        header.setStyleSheet(f'''
            QFrame {{
                background-color: {GitHubColors.HEADER_BG};
                padding: 10px;
            }}
        ''')
        header_layout = QHBoxLayout(header)

        logo = QLabel('GitHub Client')
        logo.setFont(QFont('Arial', 18, QFont.Bold))
        logo.setStyleSheet(f'color: {GitHubColors.HEADER_TEXT};')
        header_layout.addWidget(logo)

        header_layout.addStretch()

        self.lang_combo = QComboBox()
        self.lang_combo.addItems(['中文', 'English'])
        self.lang_combo.setStyleSheet('color: white; background-color: transparent; border: none;')
        header_layout.addWidget(self.lang_combo)

        self.user_label = QLabel('')
        self.user_label.setStyleSheet(f'color: {GitHubColors.HEADER_TEXT};')
        header_layout.addWidget(self.user_label)

        self.logout_button = QPushButton(get_text('logout_button'))
        self.logout_button.setStyleSheet('''
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
        ''')
        self.logout_button.clicked.connect(self.logout)
        header_layout.addWidget(self.logout_button)

        layout.addWidget(header)

    def login(self, token, cookie, lang):
        global CURRENT_LANG
        CURRENT_LANG = lang
        set_language(lang)

        self.api = GitHubAPI(token, cookie)

        if not self.api.validate_token():
            QMessageBox.critical(self, get_text('error_title'), get_text('token_invalid'))
            return False

        self.user = self.api.get_user()
        if self.user:
            self.user_label.setText(f'{get_text("welcome").format(self.user.get("login", ""))}')

            repos_view = RepositoriesView(self.api)
            profile_view = ProfileView(self.api, self.user)

            self.content_stack.clear()
            tabs = QTabWidget()
            tabs.setStyleSheet('''
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
            ''')
            tabs.addTab(repos_view, f'📁 {get_text("repositories_tab")}')
            tabs.addTab(profile_view, f'👤 {get_text("profile_tab")}')
            self.content_stack.addWidget(tabs)

            return True
        return False

    def logout(self):
        reply = QMessageBox.question(
            self,
            get_text('confirm_logout'),
            '',
            QMessageBox.Yes | QMessageBox.No
        )
        if reply == QMessageBox.Yes:
            self.api = None
            self.user = None
            self.repo_stack = []
            self.content_stack.clear()
            self.content_stack.addWidget(QWidget())
            self.show_login_dialog()

    def open_repository_detail(self, repo):
        detail_view = RepositoryDetailView(self.api, repo)
        self.repo_stack.append(self.content_stack.currentWidget())
        self.content_stack.addWidget(detail_view)
        self.content_stack.setCurrentIndex(self.content_stack.count() - 1)

    def show_login_dialog(self):
        dialog = LoginDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            token = dialog.get_token()
            cookie = dialog.get_cookie()
            lang = dialog.get_language()
            if token:
                self.login(token, cookie, lang)
            else:
                QMessageBox.warning(self, get_text('error_title'), get_text('login_required'))


def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')

    app.setStyleSheet('''
        QMainWindow {
            background-color: #ffffff;
        }
        QLabel {
            color: #24292f;
        }
    ''')

    window = MainWindow()
    window.show_login_dialog()
    window.show()

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
