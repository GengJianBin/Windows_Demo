import { app, BrowserWindow, ipcMain, desktopCapturer, screen } from 'electron';
import * as path from 'path';

// ===== 类型定义 =====

interface SourceRequestOptions {
  types?: string[];
}

interface SerializedSource {
  id: string;
  name: string;
  type: 'screen' | 'window';
  thumbnail: string | null;
}

interface GetSourcesResult {
  success: boolean;
  sources?: SerializedSource[];
  error?: string;
}

interface StartShareOptions {
  smoothMode: boolean;
  shareAudio: boolean;
}

interface StartShareResult {
  success: boolean;
  sourceId?: string;
  options?: StartShareOptions;
  error?: string;
}

interface CancelResult {
  success: boolean;
}

// ===== 窗口管理 =====

let mainWindow: Electron.BrowserWindow | null = null;

function createWindow(): void {
  // 获取主屏幕尺寸，让窗口居中且大小合理
  const primaryDisplay = screen.getPrimaryDisplay();
  const { width: screenW, height: screenH } = primaryDisplay.workAreaSize;

  // 防止窗口超出屏幕
  const winWidth = Math.min(880, screenW - 80);
  const winHeight = Math.min(620, screenH - 80);

  mainWindow = new BrowserWindow({
    width: winWidth,
    height: winHeight,
    resizable: false,
    minimizable: false,
    maximizable: false,
    title: '屏幕共享',
    backgroundColor: '#1e1e2e',
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false,
    },
  });

  // 居中
  mainWindow.center();

  // 开发模式加载 Vite 服务器，生产模式加载本地文件
  const isDev = process.argv.includes('--dev');
  if (isDev) {
    mainWindow.loadURL('http://localhost:5173');
    mainWindow.webContents.openDevTools({ mode: 'detach' });
  } else {
    mainWindow.loadFile(path.join(__dirname, '../..', 'dist', 'index.html'));
  }

  // 窗口关闭时清空引用
  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

// ============ IPC 事件处理 ============

/**
 * 渲染进程请求屏幕/应用源列表
 * 使用 desktopCapturer.getSources 获取缩略图
 */
ipcMain.handle(
  'get-sources',
  async (_event, opts: SourceRequestOptions): Promise<GetSourcesResult> => {
    try {
      const sources = await desktopCapturer.getSources({
        types: (opts.types as any) || ['screen', 'window'],
        thumbnailSize: { width: 320, height: 200 },
        fetchWindowAudio: false,
      });

      // 序列化数据传给渲染进程（thumbnail 是 NativeImage，需要转 base64）
      const result: SerializedSource[] = sources.map((src) => ({
        id: src.id,
        name: src.name,
        type: src.id.startsWith('screen') ? 'screen' : 'window',
        thumbnail: src.thumbnail && !src.thumbnail.isEmpty()
          ? src.thumbnail.toDataURL()
          : null,
      }));

      return { success: true, sources: result };
    } catch (err) {
      const error = err instanceof Error ? err.message : String(err);
      return { success: false, error };
    }
  }
);

/**
 * 用户确认选择某个源，开始共享
 */
ipcMain.handle(
  'start-share',
  async (
    _event,
    sourceId: string,
    options: StartShareOptions
  ): Promise<StartShareResult> => {
    // 实际项目中这里会启动 WebRTC / 推流逻辑
    console.log('[主进程] 开始共享源:', sourceId, '选项:', options);
    return { success: true, sourceId, options };
  }
);

/**
 * 取消共享
 */
ipcMain.handle(
  'cancel-share',
  async (): Promise<CancelResult> => {
    console.log('[主进程] 取消共享');
    if (mainWindow) {
      mainWindow.close();
    }
    return { success: true };
  }
);

// ============ 应用生命周期 ============

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) createWindow();
});
