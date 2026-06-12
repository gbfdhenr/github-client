import requests
import base64
from typing import Optional, Dict, List, Tuple


class GitHubAPI:
    def __init__(self, token: str, cookie: Optional[str] = None):
        self.token = token
        self.cookie = cookie
        self.base_url = 'https://api.github.com'
        self.session = requests.Session()
        self.session.headers.update({
            'Authorization': f'token {token}',
            'Accept': 'application/vnd.github.v3+json',
        })
        if cookie:
            self.session.headers['Cookie'] = cookie

    def get_user(self) -> Optional[Dict]:
        """获取当前用户信息（来自 GitHub.com）"""
        try:
            response = self.session.get(f'{self.base_url}/user')
            if response.status_code == 200:
                return response.json()
            return None
        except Exception:
            return None

    def get_repositories(self, username: str = None, sort: str = 'updated') -> List[Dict]:
        """获取仓库列表（来自 GitHub.com）"""
        try:
            if username:
                url = f'{self.base_url}/users/{username}/repos'
            else:
                url = f'{self.base_url}/user/repos'
            params = {'per_page': 100, 'sort': sort}
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_repository_detail(self, owner: str, repo: str) -> Optional[Dict]:
        """获取仓库详细信息（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}'
            response = self.session.get(url)
            if response.status_code == 200:
                return response.json()
            return None
        except Exception:
            return None

    def get_issues(self, owner: str, repo: str, state: str = 'open') -> List[Dict]:
        """获取 Issues（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/issues'
            params = {'state': state, 'per_page': 100}
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_pull_requests(self, owner: str, repo: str, state: str = 'open') -> List[Dict]:
        """获取 Pull Requests（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/pulls'
            params = {'state': state, 'per_page': 100}
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_readme(self, owner: str, repo: str, branch: str = 'main') -> Optional[str]:
        """获取 README 内容（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/readme'
            response = self.session.get(url)
            if response.status_code == 200:
                data = response.json()
                content = base64.b64decode(data['content']).decode('utf-8')
                return content
            return None
        except Exception:
            return None

    def get_repository_contents(self, owner: str, repo: str, path: str = '', ref: str = '') -> List[Dict]:
        """获取目录/文件列表（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/contents/{path}'
            params = {}
            if ref:
                params['ref'] = ref
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_file_content(self, owner: str, repo: str, path: str, branch: str = 'main') -> Optional[Dict]:
        """获取单个文件内容（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/contents/{path}'
            params = {'ref': branch}
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                data = response.json()
                if 'content' in data:
                    data['decoded_content'] = base64.b64decode(data['content']).decode('utf-8')
                return data
            return None
        except Exception:
            return None

    def get_branches(self, owner: str, repo: str) -> List[Dict]:
        """获取分支列表（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/branches'
            response = self.session.get(url)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_commits(self, owner: str, repo: str, path: str = '', per_page: int = 30) -> List[Dict]:
        """获取提交历史（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/commits'
            params = {'per_page': per_page}
            if path:
                params['path'] = path
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_issue_detail(self, owner: str, repo: str, issue_number: int) -> Optional[Dict]:
        """获取 Issue 详情（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/issues/{issue_number}'
            response = self.session.get(url)
            if response.status_code == 200:
                return response.json()
            return None
        except Exception:
            return None

    def get_issue_comments(self, owner: str, repo: str, issue_number: int) -> List[Dict]:
        """获取 Issue 评论（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/repos/{owner}/{repo}/issues/{issue_number}/comments'
            response = self.session.get(url)
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_starred_repos(self, username: str = None) -> List[Dict]:
        """获取标星仓库（来自 GitHub.com）"""
        try:
            if username:
                url = f'{self.base_url}/users/{username}/starred'
            else:
                url = f'{self.base_url}/user/starred'
            response = self.session.get(url, params={'per_page': 100})
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_gists(self, username: str = '') -> List[Dict]:
        """获取 Gist 列表（来自 GitHub.com）"""
        try:
            if username:
                url = f'{self.base_url}/users/{username}/gists'
            else:
                url = f'{self.base_url}/gists'
            response = self.session.get(url, params={'per_page': 100})
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_followers(self, username: str) -> List[Dict]:
        """获取关注者列表（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/users/{username}/followers'
            response = self.session.get(url, params={'per_page': 100})
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def get_following(self, username: str) -> List[Dict]:
        """获取正在关注的用户（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/users/{username}/following'
            response = self.session.get(url, params={'per_page': 100})
            if response.status_code == 200:
                return response.json()
            return []
        except Exception:
            return []

    def search_repositories(self, query: str, sort: str = 'stars') -> List[Dict]:
        """搜索仓库（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/search/repositories'
            params = {'q': query, 'sort': sort, 'per_page': 100}
            response = self.session.get(url, params=params)
            if response.status_code == 200:
                data = response.json()
                return data.get('items', [])
            return []
        except Exception:
            return []

    def validate_token(self) -> bool:
        """验证 Token 是否有效（通过 GitHub.com API）"""
        user = self.get_user()
        return user is not None

    def get_rate_limit(self) -> Optional[Dict]:
        """获取 API 调用限制（来自 GitHub.com）"""
        try:
            url = f'{self.base_url}/rate_limit'
            response = self.session.get(url)
            if response.status_code == 200:
                return response.json()
            return None
        except Exception:
            return None
