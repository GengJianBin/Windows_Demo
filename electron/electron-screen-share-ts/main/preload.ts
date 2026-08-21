declare function require(id: string): any;
const electron = require('electron') as {
  contextBridge: Electron.ContextBridge;
  ipcRenderer: Electron.IpcRenderer;
};
const { contextBridge, ipcRenderer } = electron;

// ===== 类型定义 =====

export interface SourceRequestOptions {
  types?: string[];
}

export interface SerializedSource {
  id: string;
  name: string;
  type: 'screen' | 'window';
  thumbnail: string | null;
}

export interface GetSourcesResult {
  success: boolean;
  sources?: SerializedSource[];
  error?: string;
}

export interface StartShareOptions {
  smoothMode: boolean;
  shareAudio: boolean;
}

export interface StartShareResult {
  success: boolean;
  sourceId?: string;
  options?: StartShareOptions;
  error?: string;
}

export interface ElectronAPI {
  getSources: (opts: SourceRequestOptions) => Promise<GetSourcesResult>;
  startShare: (sourceId: string, options: StartShareOptions) => Promise<StartShareResult>;
  cancelShare: () => Promise<{ success: boolean }>;
  on: (channel: string, callback: (...args: any[]) => void) => void;
}

// ===== 暴露 API 给渲染进程 =====

const electronAPI: ElectronAPI = {
  getSources: (opts: SourceRequestOptions): Promise<GetSourcesResult> =>
    ipcRenderer.invoke('get-sources', opts),

  startShare: (
    sourceId: string,
    options: StartShareOptions
  ): Promise<StartShareResult> =>
    ipcRenderer.invoke('start-share', sourceId, options),

  cancelShare: (): Promise<{ success: boolean }> =>
    ipcRenderer.invoke('cancel-share'),

  on: (channel: string, callback: (...args: any[]) => void): void => {
    ipcRenderer.on(channel, (_event, ...args) => callback(...args));
  },
};

contextBridge.exposeInMainWorld('electronAPI', electronAPI);
