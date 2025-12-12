一、错误描述
package.json中的devDependencies字段中指定node-gyp依赖，当指定的node-gyp以来的版本小于10.2.0时，并且在scripts字段中指定了"install": "node-gyp rebuild",则执行npm install 时会报错。

二、错误信息
C:\Workspace\openSource\GitGJB\Windows_Demo\node_native>npm install

> napi-adapter@1.0.0 install
> node-gyp rebuild

gyp info it worked if it ends with ok
gyp info using node-gyp@10.2.0
gyp info using node@25.2.0 | win32 | x64
gyp info find Python using Python version 3.12.4 found at "C:\Users\13684\AppData\Local\Programs\Python\Python312\python.exe"

gyp info find VS using VS2022 (17.14.36414.22) found at:
gyp info find VS "C:\Program Files\Microsoft Visual Studio\2022\Community"
gyp info find VS run with --verbose for detailed information
gyp info spawn C:\Users\13684\AppData\Local\Programs\Python\Python312\python.exe
gyp info spawn args [
gyp info spawn args 'C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\node_modules\\node-gyp\\gyp\\gyp_main.py',
gyp info spawn args 'binding.gyp',
gyp info spawn args '-f',
gyp info spawn args 'msvs',
gyp info spawn args '-I',
gyp info spawn args 'C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\build\\config.gypi',
gyp info spawn args '-I',
gyp info spawn args 'C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\node_modules\\node-gyp\\addon.gypi',
gyp info spawn args '-I',
gyp info spawn args 'C:\\Users\\13684\\AppData\\Local\\node-gyp\\Cache\\25.2.0\\include\\node\\common.gypi',
gyp info spawn args '-Dlibrary=shared_library',
gyp info spawn args '-Dvisibility=default',
gyp info spawn args '-Dnode_root_dir=C:\\Users\\13684\\AppData\\Local\\node-gyp\\Cache\\25.2.0',
gyp info spawn args '-Dnode_gyp_dir=C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\node_modules\\node-gyp',
gyp info spawn args '-Dnode_lib_file=C:\\\\Users\\\\13684\\\\AppData\\\\Local\\\\node-gyp\\\\Cache\\\\25.2.0\\\\<(target_arch)\\\\node.lib',
gyp info spawn args '-Dmodule_root_dir=C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native',
gyp info spawn args '-Dnode_engine=v8',
gyp info spawn args '--depth=.',
gyp info spawn args '--no-parallel',
gyp info spawn args '--generator-output',
gyp info spawn args 'C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\build',
gyp info spawn args '-Goutput_dir=.'
gyp info spawn args ]
gyp info spawn C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe
gyp info spawn args [
gyp info spawn args 'build\\binding.sln',
gyp info spawn args '/clp:Verbosity=minimal',
gyp info spawn args '/nologo',
gyp info spawn args '/p:Configuration=Release;Platform=x64'
gyp info spawn args ]

C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Microsoft\VC\v170\Microsoft.CppBuild.targets(463,5): er
ror MSB8020: 无法找到 ClangCL 的生成工具(平台工具集 =“ClangCL”)。若要使用 ClangCL 生成工具进行生成，请安装 ClangCL 生成工具。或者，可以升级到当前 Visual Studio 工具，方式
是通过选择“项目”菜单或右键单击该解决方案，然后选择“重定解决方案目标”。 [C:\Workspace\openSource\GitGJB\Windows_Demo\node_native\build\napi_adapter.vcxpr
oj]
gyp ERR! build error
gyp ERR! stack Error: `C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe` failed with exit code: 1
gyp ERR! stack at ChildProcess.<anonymous> (C:\Workspace\openSource\GitGJB\Windows_Demo\node_native\node_modules\node-gyp\lib\build.js:216:23)
gyp ERR! stack at ChildProcess.emit (node:events:508:28)
gyp ERR! stack at ChildProcess._handle.onexit (node:internal/child_process:293:12)
gyp ERR! System Windows_NT 10.0.26200
gyp ERR! command "C:\\Users\\13684\\AppData\\Local\\nvs\\node\\25.2.0\\x64\\node.exe" "C:\\Workspace\\openSource\\GitGJB\\Windows_Demo\\node_native\\node_modules\\node-gyp\\bin\\node-gyp.js" "rebuild"
gyp ERR! cwd C:\Workspace\openSource\GitGJB\Windows_Demo\node_native
gyp ERR! node -v v25.2.0
gyp ERR! node-gyp -v v10.2.0
gyp ERR! not ok
npm error code 1
npm error path C:\Workspace\openSource\GitGJB\Windows_Demo\node_native
npm error command failed
npm error command C:\Windows\system32\cmd.exe /d /s /c node-gyp rebuild
npm error A complete log of this run can be found in: C:\Users\13684\AppData\Local\npm-cache\_logs\2025-11-17T07_02_25_765Z-debug-0.log

三、原因
node-gyp的bug，devDependencies字段指定node-gyp的版本大于10.2.0

四、全局node-gyp和npm内置的node-gyp
- 全局的node-gyp通过 npm install -g node-gyp命令安装
  - 路径：C:\Users\13684\AppData\Local\nvs\node\25.2.0\x64\node_modules\node-gyp
- npm内置的node-gyp在C:\Users\13684\AppData\Local\nvs\node\25.2.0\x64\node_modules\npm\node_modules\node-gyp路径中
  - 绑定在 npm 内部，随 npm 版本升级而升级
  - 只供 npm 脚本调用（如 "install": "node-gyp rebuild"）
  - 不依赖全局环境，也不会受全局 node-gyp 版本影响
  - 主要目的是 保证 npm install 可重复、可预测
