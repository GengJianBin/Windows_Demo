/**
 * Electron 最小类型声明
 * 仅声明本项目用到的 API，避免依赖完整的 @types/electron
 */

declare namespace Electron {
  interface Size {
    width: number;
    height: number;
  }

  interface Rectangle {
    x: number;
    y: number;
    width: number;
    height: number;
  }

  interface Display {
    id: number;
    bounds: Rectangle;
    workArea: Rectangle;
    workAreaSize: Size;
    size: Size;
    scaleFactor: number;
  }

  interface NativeImage {
    isEmpty(): boolean;
    toDataURL(): string;
    toPNG(): Buffer;
    toJPEG(quality: number): Buffer;
    getSize(): Size;
  }

  interface DesktopCapturerSource {
    id: string;
    name: string;
    thumbnail: NativeImage;
    display_id?: string;
    appIcon?: NativeImage;
  }

  interface DesktopCapturer {
    getSources(options: {
      types: string[];
      thumbnailSize: Size;
      fetchWindowAudio?: boolean;
    }): Promise<DesktopCapturerSource[]>;
  }

  interface IpcMain {
    handle(channel: string, listener: (event: any, ...args: any[]) => any): void;
    on(channel: string, listener: (event: any, ...args: any[]) => void): void;
  }

  interface WebPreferences {
    preload?: string;
    contextIsolation?: boolean;
    nodeIntegration?: boolean;
  }

  interface BrowserWindowConstructorOptions {
    width?: number;
    height?: number;
    resizable?: boolean;
    minimizable?: boolean;
    maximizable?: boolean;
    title?: string;
    backgroundColor?: string;
    webPreferences?: WebPreferences;
  }

  interface BrowserWindow {
    loadFile(path: string): void;
    loadURL(url: string): void;
    center(): void;
    close(): void;
    on(event: string, listener: (...args: any[]) => void): void;
    webContents: {
      openDevTools(options?: { mode?: string }): void;
    };
  }

  interface Screen {
    getPrimaryDisplay(): Display;
  }

  interface App {
    whenReady(): Promise<void>;
    quit(): void;
    on(event: string, listener: (...args: any[]) => void): void;
  }

  interface ContextBridge {
    exposeInMainWorld(apiKey: string, api: any): void;
  }

  interface IpcRenderer {
    invoke(channel: string, ...args: any[]): Promise<any>;
    on(channel: string, listener: (event: any, ...args: any[]) => void): void;
  }
}

declare module 'electron' {
  export const app: Electron.App;
  export const BrowserWindow: {
    new (options?: Electron.BrowserWindowConstructorOptions): Electron.BrowserWindow;
    getAllWindows(): Electron.BrowserWindow[];
  };
  export const ipcMain: Electron.IpcMain;
  export const desktopCapturer: Electron.DesktopCapturer;
  export const screen: Electron.Screen;
  export const contextBridge: Electron.ContextBridge;
  export const ipcRenderer: Electron.IpcRenderer;
}
