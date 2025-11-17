@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo N-API Adapter 编译脚本
echo ========================================
echo.

REM 获取脚本所在目录
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

REM 检查 Node.js 是否安装
where node >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 Node.js，请先安装 Node.js
    echo.
    echo 如果使用 nvs，请运行: nvs add latest
    echo 或者访问: https://nodejs.org/
    pause
    exit /b 1
)

REM 显示 Node.js 版本
echo [信息] 检测到 Node.js 版本:
node --version
echo.

REM 检查 node-gyp 是否安装
where node-gyp >nul 2>&1
if %errorlevel% neq 0 (
    echo [警告] 未找到 node-gyp，正在尝试全局安装...
    call npm install -g node-gyp
    if !errorlevel! neq 0 (
        echo [错误] node-gyp 安装失败
        echo 请手动运行: npm install -g node-gyp
        pause
        exit /b 1
    )
    echo [成功] node-gyp 安装完成
    echo.
)

REM 显示 node-gyp 版本
echo [信息] node-gyp 版本:
call node-gyp --version
echo .

REM 检查是否安装本地依赖
if not exist "node_modules" (
    echo [信息] 未找到 node_modules，正在安装依赖...
    call npm install
    if !errorlevel! neq 0 (
        echo [错误] 依赖安装失败
        pause
        exit /b 1
    )
    echo.
)

REM 询问是否清理之前的构建
set /p CLEAN="是否清理之前的构建? (y/n, 默认: n): "
if /i "%CLEAN%"=="y" (
    echo.
    echo [信息] 正在清理之前的构建...
    call node-gyp clean
    if !errorlevel! neq 0 (
        echo [警告] 清理失败，继续构建...
    )
    echo.
)

REM 配置项目
echo ========================================
echo [步骤 1/2] 正在配置项目...
echo ========================================
call node-gyp configure
if %errorlevel% neq 0 (
    echo.
    echo [错误] 配置失败
    echo.
    echo 可能的解决方案:
    echo 1. 确保已安装 Visual Studio Build Tools 或 Visual Studio
    echo 2. 运行: npm install --global windows-build-tools
    echo 3. 检查 Python 是否已安装
    pause
    exit /b 1
)
echo [成功] 配置完成
echo.

REM 构建项目
echo ========================================
echo [步骤 2/2] 正在构建项目...
echo ========================================
call node-gyp build
if %errorlevel% neq 0 (
    echo.
    echo [错误] 构建失败
    echo.
    echo 请检查:
    echo 1. 编译错误信息
    echo 2. 确保所有源文件存在
    echo 3. 检查 Visual Studio 工具链是否正确安装
    pause
    exit /b 1
)
echo [成功] 构建完成
echo.

REM 检查输出文件
if exist "build\Release\napi_adapter.node" (
    echo ========================================
    echo [成功] 编译成功！
    echo ========================================
    echo.
    echo 输出文件: build\Release\napi_adapter.node
    echo.
    
    REM 询问是否运行测试
    set /p TEST="是否运行测试? (y/n, 默认: n): "
    if /i "!TEST!"=="y" (
        echo.
        echo [信息] 正在运行测试...
        call npm test
        echo.
    )
) else (
    echo [警告] 未找到输出文件 build\Release\napi_adapter.node
    echo 请检查构建日志
)

echo.
echo ========================================
echo 编译脚本执行完成
echo ========================================
pause

