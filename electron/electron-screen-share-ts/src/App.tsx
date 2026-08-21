import React, { useState, useEffect, useMemo, useCallback } from 'react';
import TabSwitch from './components/TabSwitch';
import SourceList from './components/SourceList';
import ControlPanel from './components/ControlPanel';
import type { ISource, TabType, IShareOptions } from './types/global';

// ===== 类型定义 =====

interface ISourcesResponse {
  success: boolean;
  sources?: ISource[];
  error?: string;
}

interface IStartShareResponse {
  success: boolean;
  sourceId?: string;
  options?: IShareOptions;
  error?: string;
}

// ===== 常量 =====

const INITIAL_TAB: TabType = 'screen';
const INITIAL_SMOOTH_MODE = true;
const INITIAL_SHARE_AUDIO = false;

/**
 * App - 屏幕共享选择页面 · 主组件
 *
 * 功能：
 *  - 获取并展示可共享的屏幕和应用列表
 *  - 支持点击选中某个源
 *  - 底部勾选框：流畅模式 / 共享声音
 *  - 确定 → 通知主进程开始共享
 *  - 取消 → 关闭窗口
 */
const App: React.FC = () => {
  // ===== 数据状态 =====
  const [sources, setSources] = useState<ISource[]>([]);
  const [loading, setLoading] = useState<boolean>(true);
  const [error, setError] = useState<string | null>(null);

  // ===== UI 状态 =====
  const [activeTab, setActiveTab] = useState<TabType>(INITIAL_TAB);
  const [selectedId, setSelectedId] = useState<string | null>(null);

  // ===== 选项状态 =====
  const [smoothMode, setSmoothMode] = useState<boolean>(INITIAL_SMOOTH_MODE);
  const [shareAudio, setShareAudio] = useState<boolean>(INITIAL_SHARE_AUDIO);
  const [confirming, setConfirming] = useState<boolean>(false);

  // ===== 初始化：获取源列表 =====
  useEffect(() => {
    let cancelled = false;

    async function fetchSources(): Promise<void> {
      try {
        setLoading(true);
        setError(null);

        const result: ISourcesResponse = await window.electronAPI.getSources({
          types: ['screen', 'window'],
        });

        if (cancelled) return;

        if (result.success && result.sources) {
          setSources(result.sources);
        } else {
          setError(result.error || '获取源失败');
        }
      } catch (err) {
        if (cancelled) return;
        const message = err instanceof Error ? err.message : String(err);
        setError(message);
      } finally {
        if (!cancelled) setLoading(false);
      }
    }

    fetchSources();

    return () => {
      cancelled = true;
    };
  }, []);

  // ===== 派生数据（用 useMemo，不存 state） =====
  const filteredSources = useMemo<ISource[]>(
    () => sources.filter((s) => s.type === activeTab),
    [sources, activeTab]
  );

  const counts = useMemo(
    () => ({
      screen: sources.filter((s) => s.type === 'screen').length,
      window: sources.filter((s) => s.type === 'window').length,
    }),
    [sources]
  );

  const selectedSource = useMemo<ISource | null>(
    () => sources.find((s) => s.id === selectedId) || null,
    [sources, selectedId]
  );

  // ===== 事件处理（用 useCallback 稳定引用） =====
  const handleSelect = useCallback((id: string): void => {
    setSelectedId(id);
  }, []);

  const handleTabChange = useCallback((tab: TabType): void => {
    setActiveTab(tab);
    // 切换 tab 时清空选中（避免跨类型选中）
    setSelectedId(null);
  }, []);

  const handleToggleSmooth = useCallback((): void => {
    setSmoothMode((v) => !v);
  }, []);

  const handleToggleAudio = useCallback((): void => {
    setShareAudio((v) => !v);
  }, []);

  const handleConfirm = useCallback(async (): Promise<void> => {
    if (!selectedSource || confirming) return;

    try {
      setConfirming(true);

      const result: IStartShareResponse = await window.electronAPI.startShare(
        selectedSource.id,
        { smoothMode, shareAudio }
      );

      if (result.success) {
        console.log('[渲染进程] 共享已启动:', selectedSource.id);
      }
    } catch (err) {
      const message = err instanceof Error ? err.message : String(err);
      console.error('启动共享失败:', message);
      alert('启动共享失败: ' + message);
    } finally {
      setConfirming(false);
    }
  }, [selectedSource, confirming, smoothMode, shareAudio]);

  const handleCancel = useCallback(async (): Promise<void> => {
    try {
      await window.electronAPI.cancelShare();
    } catch (err) {
      const message = err instanceof Error ? err.message : String(err);
      console.error('取消失败:', message);
    }
  }, []);

  // ===== 错误态 =====
  if (error) {
    return (
      <div className="app-container">
        <div className="empty-state">
          <div className="empty-icon">⚠️</div>
          <div className="empty-text">获取共享源失败：{error}</div>
          <button className="btn btn-confirm" onClick={handleCancel}>
            关闭
          </button>
        </div>
      </div>
    );
  }

  // ===== 正常渲染 =====
  return (
    <div className="app-container">
      {/* 头部 + Tab */}
      <div className="app-header">
        <h1>选择要共享的内容</h1>
        <TabSwitch
          activeTab={activeTab}
          onTabChange={handleTabChange}
          counts={counts}
        />
      </div>

      {/* 源列表区域 */}
      <div className="source-area">
        <SourceList
          sources={filteredSources}
          selectedId={selectedId}
          onSelect={handleSelect}
          loading={loading}
        />
      </div>

      {/* 底部控制面板 */}
      <ControlPanel
        selectedSource={selectedSource}
        smoothMode={smoothMode}
        shareAudio={shareAudio}
        onToggleSmooth={handleToggleSmooth}
        onToggleAudio={handleToggleAudio}
        onConfirm={handleConfirm}
        onCancel={handleCancel}
        confirming={confirming}
      />
    </div>
  );
};

export default App;
