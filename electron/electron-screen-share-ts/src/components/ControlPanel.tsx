import React from 'react';
import type { ISource } from '../types/global';

// ===== Props 类型定义 =====
export interface IControlPanelProps {
  selectedSource: ISource | null;
  smoothMode: boolean;
  shareAudio: boolean;
  onToggleSmooth: () => void;
  onToggleAudio: () => void;
  onConfirm: () => void;
  onCancel: () => void;
  confirming: boolean;
}

/**
 * ControlPanel - 底部控制面板
 * 包含：流畅模式 + 共享声音 勾选框 + 确定/取消按钮
 */
const ControlPanel: React.FC<IControlPanelProps> = ({
  selectedSource,
  smoothMode,
  shareAudio,
  onToggleSmooth,
  onToggleAudio,
  onConfirm,
  onCancel,
  confirming,
}) => {
  const canConfirm = !!selectedSource && !confirming;

  return (
    <div className="control-panel">
      {/* 左侧：勾选框组 */}
      <div className="checkbox-group">
        <label className="checkbox-item">
          <input
            type="checkbox"
            checked={smoothMode}
            onChange={onToggleSmooth}
          />
          <span>🚀 流畅模式</span>
        </label>
        <label className="checkbox-item">
          <input
            type="checkbox"
            checked={shareAudio}
            onChange={onToggleAudio}
          />
          <span>🔊 共享声音</span>
        </label>
      </div>

      {/* 中间：当前选中摘要 */}
      {selectedSource && (
        <div className="selection-summary">
          已选：{selectedSource.name}
        </div>
      )}

      {/* 右侧：按钮组 */}
      <div className="button-group">
        <button className="btn btn-cancel" onClick={onCancel}>
          取消
        </button>
        <button
          className="btn btn-confirm"
          onClick={onConfirm}
          disabled={!canConfirm}
        >
          {confirming ? '启动中…' : '确定共享'}
        </button>
      </div>
    </div>
  );
};

export default ControlPanel;
