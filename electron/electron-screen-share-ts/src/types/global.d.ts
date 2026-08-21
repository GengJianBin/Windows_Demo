import { SerializedSource, StartShareOptions } from '../../main/preload';

// ===== 共享类型定义 =====

/** 共享源类型 */
export type SourceType = 'screen' | 'window';

/** 共享源对象 */
export interface ISource extends SerializedSource {}

/** 获取源列表请求参数 */
export interface IGetSourcesOptions {
  types?: SourceType[];
}

/** Tab 类型 */
export type TabType = 'screen' | 'window';

/** 共享配置选项 */
export interface IShareOptions extends StartShareOptions {}

/** 确定按钮状态 */
export interface IConfirmState {
  loading: boolean;
  disabled: boolean;
}

// ===== Electron API 类型声明 =====

declare global {
  interface Window {
    electronAPI: {
      getSources: (opts: IGetSourcesOptions) => Promise<{
        success: boolean;
        sources?: ISource[];
        error?: string;
      }>;
      startShare: (
        sourceId: string,
        options: IShareOptions
      ) => Promise<{
        success: boolean;
        sourceId?: string;
        options?: IShareOptions;
        error?: string;
      }>;
      cancelShare: () => Promise<{ success: boolean }>;
      on: (channel: string, callback: (...args: any[]) => void) => void;
    };
  }
}
