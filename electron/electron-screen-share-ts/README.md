# Electron 屏幕共享选择页面（React + TypeScript）

## 项目结构

```
electron-screen-share-ts/
├── package.json              # 项目配置 + 依赖
├── tsconfig.json             # 渲染进程 TS 配置
├── tsconfig.node.json        # 主进程 TS 配置
├── vite.config.ts            # Vite 构建配置
├── index.html                # Vite 入口 HTML
├── main/                     # Electron 主进程（TypeScript）
│   ├── main.ts               # 主进程逻辑 + IPC 处理
│   └── preload.ts            # 安全桥接层（contextBridge）
└── src/                      # 渲染进程（React + TypeScript）
    ├── main.tsx              # React 入口
    ├── App.tsx               # 主组件（状态管理 + 数据流）
    ├── styles.css            # 全局深色主题样式
    ├── types/
    │   └── global.d.ts       # 全局类型声明（window.electronAPI）
    └── components/
        ├── TabSwitch.tsx      # 顶部 Tab：共享屏幕 / 共享应用
        ├── SourceItem.tsx     # 单个源卡片（缩略图 + 选中态）
        ├── SourceList.tsx     # 网格列表（含加载态 + 空态）
        └── ControlPanel.tsx   # 底部面板（勾选框 + 确定/取消）
```

## 技术栈

- **Electron 28+** — 桌面应用框架
- **React 18** — UI 渲染
- **TypeScript 5** — 全量类型安全
- **Vite 5** — 极速构建工具
- **纯 CSS** — 无 UI 框架依赖（深色主题）

## 快速开始

```bash
# 安装依赖
npm install

# 开发模式（Vite HMR + Electron 热重载）
npm run dev

# 构建并启动
npm run start
```

## 开发模式说明

- Vite 开发服务器运行在 `http://localhost:5173`
- Electron 主进程监听文件变化自动重启
- 渲染进程支持 HMR（热模块替换）
- 主进程代码修改后自动重新编译

## 构建产物

```
dist/         → 渲染进程（React 打包结果）
dist-main/    → 主进程（TypeScript 编译结果）
```

## 功能一览

| 功能 | 说明 |
|------|------|
| 🖥 共享屏幕 Tab | 列出所有显示器/屏幕 |
| 🪟 共享应用 Tab | 列出所有可共享的窗口 |
| 缩略图预览 | 每个源显示实时缩略图 |
| 选中高亮 | 选中卡片紫色边框 + 角标 |
| 🚀 流畅模式 | 勾选框，默认开启 |
| 🔊 共享声音 | 勾选框，可选 |
| 确定按钮 | 未选中时禁用，点击后通知主进程 |
| 取消按钮 | 关闭选择窗口 |
| 加载/空/错误态 | 三种边界状态完整处理 |

## TypeScript 类型设计

| 类型 | 位置 | 用途 |
|------|------|------|
| `ISource` | `types/global.d.ts` | 共享源对象（id/name/type/thumbnail） |
| `TabType` | `types/global.d.ts` | Tab 类型，仅限 `'screen' \| 'window'` |
| `IShareOptions` | `types/global.d.ts` | 共享配置（流畅模式 + 共享声音） |
| `ElectronAPI` | `main/preload.ts` | 暴露给渲染进程的 API 契约 |
| `IComponentProps` | 各组件文件 | 每个组件的 Props 接口 |

## IPC 通信契约

### 渲染进程 → 主进程

| 通道 | 参数 | 返回值 |
|------|------|--------|
| `get-sources` | `{ types?: string[] }` | `{ success, sources?, error? }` |
| `start-share` | `sourceId: string, options: IShareOptions` | `{ success, sourceId?, options? }` |
| `cancel-share` | 无 | `{ success: true }` |
