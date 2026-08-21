import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import './styles.css';

// 如果是 global.ts,  *.d.ts 文件不能被 import
//import './types/global'

// 如果是 global.d.ts（类型声明文件）
/// <reference path="./types/global.d.ts" />

// 确保 TypeScript 识别 global.d.ts
//import './types/global';

const rootElement = document.getElementById('root');

if (!rootElement) {
  throw new Error('找不到 #root 元素');
}

ReactDOM.createRoot(rootElement).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);
