import React from 'react';
import type { TabType } from '../types/global';

// ===== Props 类型定义 =====
export interface ITabSwitchProps {
  activeTab: TabType;
  onTabChange: (tab: TabType) => void;
  counts: {
    screen: number;
    window: number;
  };
}

/**
 * TabSwitch - 顶部 Tab 切换：屏幕 / 应用
 */
const TabSwitch: React.FC<ITabSwitchProps> = ({
  activeTab,
  onTabChange,
  counts,
}) => {
  return (
    <div className="tab-bar">
      <button
        className={`tab-btn ${activeTab === 'screen' ? 'active' : ''}`}
        onClick={() => onTabChange('screen')}
      >
        🖥 共享屏幕
        <span className="tab-count">{counts.screen}</span>
      </button>
      <button
        className={`tab-btn ${activeTab === 'window' ? 'active' : ''}`}
        onClick={() => onTabChange('window')}
      >
        🪟 共享应用
        <span className="tab-count">{counts.window}</span>
      </button>
    </div>
  );
};

export default TabSwitch;
