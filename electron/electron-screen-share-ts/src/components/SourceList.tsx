import React from 'react';
import SourceItem from './SourceItem';
import type { ISource } from '../types/global';

// ===== Props 类型定义 =====
export interface ISourceListProps {
  sources: ISource[];
  selectedId: string | null;
  onSelect: (id: string) => void;
  loading: boolean;
}

/**
 * SourceList - 源列表（网格布局），包含加载态和空态
 */
const SourceList: React.FC<ISourceListProps> = ({
  sources,
  selectedId,
  onSelect,
  loading,
}) => {
  // 加载态
  if (loading) {
    return (
      <div className="loading-state">
        <div className="spinner"></div>
        <div className="loading-text">正在获取可共享的内容…</div>
      </div>
    );
  }

  // 空态
  if (!sources || sources.length === 0) {
    return (
      <div className="empty-state">
        <div className="empty-icon">🔍</div>
        <div className="empty-text">没有找到可共享的内容</div>
      </div>
    );
  }

  // 正常列表
  return (
    <div className="source-grid">
      {sources.map((src) => (
        <SourceItem
          key={src.id}
          source={src}
          isSelected={selectedId === src.id}
          onSelect={onSelect}
        />
      ))}
    </div>
  );
};

export default SourceList;
