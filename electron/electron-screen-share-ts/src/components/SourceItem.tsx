import React from 'react';
import type { ISource } from '../types/global';

// ===== Props 类型定义 =====
export interface ISourceItemProps {
  source: ISource;
  isSelected: boolean;
  onSelect: (id: string) => void;
}

// ===== 图标映射 =====
const TYPE_ICON: Record<string, string> = {
  screen: '🖥',
  window: '🪟',
};

/**
 * SourceItem - 单个共享源卡片
 */
const SourceItem: React.FC<ISourceItemProps> = ({
  source,
  isSelected,
  onSelect,
}) => {
  const { id, name, type, thumbnail } = source;
  const icon = TYPE_ICON[type] || '📄';

  return (
    <div
      className={`source-card ${isSelected ? 'selected' : ''}`}
      onClick={() => onSelect(id)}
      title={name}
    >
      {/* 缩略图 */}
      {thumbnail ? (
        <img className="thumb" src={thumbnail} alt={name} />
      ) : (
        <div className="thumb-placeholder">{icon}</div>
      )}

      {/* 选中角标 */}
      <div className="check-badge">✓</div>

      {/* 底部名称条 */}
      <div className="label-bar">
        <span className="icon">{icon}</span>
        <span className="name">{name}</span>
      </div>
    </div>
  );
};

export default SourceItem;
