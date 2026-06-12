#!/usr/bin/env python3
"""
GitHub Client - GitHub 桌面客户端
支持 Windows 和 Linux 系统
"""

import os
import sys

# 添加项目根目录到路径
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from gui.github_ui import main

if __name__ == '__main__':
    main()
