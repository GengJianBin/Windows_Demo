#!/usr/bin/env bash
set -u

echo "========================================"
echo "N-API Adapter 编译脚本"
echo "========================================"
echo

# 获取脚本所在目录并进入
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# 检查 Node.js 是否安装
if ! command -v node >/dev/null 2>&1; then
  echo "[错误] 未找到 Node.js，请先安装 Node.js"
  echo
  echo "访问: https://nodejs.org/"
  exit 1
fi

# 显示 Node.js 版本
echo "[信息] 检测到 Node.js 版本:"
node --version
echo

# 选择 Python 解释器并解析版本，要求 >= 3.3
PYTHON_CMD=""
if command -v python3 >/dev/null 2>&1; then
  PYTHON_CMD="python3"
elif command -v python >/dev/null 2>&1; then
  PYTHON_CMD="python"
fi

if [ -z "$PYTHON_CMD" ]; then
  echo "[错误] 未找到 Python，请先安装 Python 3.3 或更高版本"
  exit 1
fi

PY_VERSION_RAW="$($PYTHON_CMD --version 2>&1)"
PY_VERSION_NUM="$(echo "$PY_VERSION_RAW" | sed -E 's/[^0-9]*([0-9]+\.[0-9]+).*/\1/')"
PY_MAJOR="$(echo "$PY_VERSION_NUM" | cut -d. -f1)"
PY_MINOR="$(echo "$PY_VERSION_NUM" | cut -d. -f2)"

PY_OK=1
if [ "${PY_MAJOR:-0}" -lt 3 ] || { [ "${PY_MAJOR:-0}" -eq 3 ] && [ "${PY_MINOR:-0}" -lt 3 ]; }; then
  PY_OK=0
fi

if [ "$PY_OK" -eq 0 ]; then
  echo "[错误] Python 版本小于 3.3，检测到版本: $PY_VERSION_RAW"
  exit 1
else
  echo "[成功] 检测到 Python 版本: $PY_VERSION_RAW （>= 3.3）"
  echo
fi

# 检查 node-gyp 是否安装
if ! command -v node-gyp >/dev/null 2>&1; then
  echo "[警告] 未找到 node-gyp，正在尝试全局安装..."
  if ! npm install -g node-gyp; then
    echo "[错误] node-gyp 安装失败"
    echo "请手动运行（可能需要使用 sudo）: npm install -g node-gyp"
    exit 1
  fi
  echo "[成功] node-gyp 安装完成"
  echo
fi

# 显示 node-gyp 版本
echo "[信息] node-gyp 版本:"
node-gyp --version || true
echo

# 检查是否安装本地依赖
if [ ! -d "node_modules" ]; then
  echo "[信息] 未找到 node_modules，正在安装依赖..."
  if ! npm install; then
    echo "[错误] 依赖安装失败"
    exit 1
  fi
  echo
fi

# 询问是否清理之前的构建
read -r -p "是否清理之前的构建? (y/n, 默认: n): " CLEAN
if [ "$CLEAN" = "y" ] || [ "$CLEAN" = "Y" ]; then
  echo
  echo "[信息] 正在清理之前的构建..."
  if ! node-gyp clean; then
    echo "[警告] 清理失败，继续构建..."
  fi
  echo
fi

# 配置项目
echo "========================================"
echo "[步骤 1/2] 正在配置项目..."
echo "========================================"
if ! node-gyp configure; then
  echo
  echo "[错误] 配置失败"
  echo
  echo "可能的解决方案:"
  echo "1. 确保已安装构建工具（如 build-essential, python3-dev 等）"
  echo "2. 检查 Python 是否已安装且版本满足要求"
  exit 1
fi
echo "[成功] 配置完成"
echo

# 构建项目
echo "========================================"
echo "[步骤 2/2] 正在构建项目..."
echo "========================================"
if ! node-gyp build; then
  echo
  echo "[错误] 构建失败"
  echo
  echo "请检查:"
  echo "1. 编译错误信息"
  echo "2. 确保所有源文件存在"
  echo "3. 检查 C/C++ 工具链是否正确安装"
  exit 1
fi
echo "[成功] 构建完成"
echo

# 检查输出文件
if [ -f "build/Release/napi_adapter.node" ]; then
  echo "========================================"
  echo "[成功] 编译成功！"
  echo "========================================"
  echo
  echo "输出文件: build/Release/napi_adapter.node"
  echo
  read -r -p "是否运行测试? (y/n, 默认: n): " TEST
  if [ "$TEST" = "y" ] || [ "$TEST" = "Y" ]; then
    echo
    echo "[信息] 正在运行测试..."
    npm test || true
    echo
  fi
else
  echo "[警告] 未找到输出文件 build/Release/napi_adapter.node"
  echo "请检查构建日志"
fi

echo
echo "========================================"
echo "编译脚本执行完成"
echo "========================================"

