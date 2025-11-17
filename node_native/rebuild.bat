@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo N-API Adapter 重新编译脚本
echo ========================================
echo.

REM 获取脚本所在目录
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

REM 检查并激活 nvs 环境（如果使用 nvs）
where nvs >nul 2>&1
if %errorlevel% equ 0 (
    call nvs auto >nul 2>&1
    if %errorlevel% neq 0 (
        call nvs use default >nul 2>&1
    )
)

REM 检查 Node.js 是否安装
where node >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 Node.js，请先安装 Node.js
    pause
    exit /b 1
)

REM 清理之前的构建
echo [信息] 正在清理之前的构建...
call node-gyp clean
echo.

REM 重新配置和构建
echo [信息] 正在重新配置和构建...
call node-gyp rebuild
if %errorlevel% neq 0 (
    echo.
    echo [错误] 重新构建失败
    pause
    exit /b 1
)

echo.
echo [成功] 重新构建完成！
echo 输出文件: build\Release\napi_adapter.node
echo.
pause

