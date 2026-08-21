import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

// Electron 环境下使用 file:// 协议，需要设置 base
export default defineConfig({
  plugins: [react()],
  base: './',
  build: {
    outDir: 'dist',
    emptyOutDir: true,
    // Electron 主进程通过 file:// 加载，不需要 code splitting
    rollupOptions: {
      output: {
        format: 'iife',
      },
    },
  },
  server: {
    // Vite 开发服务器端口
    port: 5173,
  },
});
