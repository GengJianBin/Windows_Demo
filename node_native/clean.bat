@echo off
chcp 65001 >nul

echo ========================================
echo N-API Adapter 清理脚本
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

REM 检查 node-gyp 是否可用
where node-gyp >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 node-gyp
    pause
    exit /b 1
)

echo [信息] 正在清理构建文件...
call node-gyp clean

if %errorlevel% equ 0 (
    echo [成功] 清理完成
) else (
    echo [警告] 清理过程中出现错误
)

echo.
pause

