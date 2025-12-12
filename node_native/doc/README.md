# N-API 原生插件开发接口适配文件

这是一个完整的 N-API (Node-API) 原生插件开发接口适配文件集合，提供了常用的辅助函数和示例代码。

## 文件结构

- `napi_adapter.h` - 头文件，包含所有辅助函数和类的声明
- `napi_adapter.cpp` - 实现文件，包含示例导出函数和模块初始化
- `binding.gyp` - Node.js 原生模块构建配置文件
- `package.json` - Node.js 包配置文件
- `index.js` - JavaScript 包装文件
- `test.js` - 测试文件
- `build.bat` - Windows 编译脚本（完整版）
- `rebuild.bat` - Windows 快速重新编译脚本
- `clean.bat` - Windows 清理脚本

## 功能特性

### 辅助函数

1. **错误处理**
   - `CreateError()` - 创建错误对象
   - `ThrowError()` - 抛出错误

2. **类型检查**
   - `IsString()`, `IsNumber()`, `IsObject()`, `IsArray()`, `IsFunction()`

3. **类型转换**
   - `GetString()`, `GetStringW()` - 获取字符串
   - `GetInt32()`, `GetUint32()`, `GetInt64()`, `GetDouble()`, `GetBoolean()` - 获取数值

4. **创建 JavaScript 值**
   - `CreateString()`, `CreateNumber()`, `CreateInt32()`, `CreateBoolean()`
   - `CreateArray()`, `CreateObject()`, `CreateNull()`, `CreateUndefined()`

5. **对象和数组操作**
   - `SetProperty()`, `GetProperty()`, `HasProperty()`
   - `SetElement()`, `GetElement()`, `GetArrayLength()`

6. **异步工作支持**
   - `AsyncWork` 类 - 用于执行异步操作

### 示例导出函数

1. `helloWorld()` - 返回问候字符串
2. `addNumbers(a, b)` - 两个数字相加
3. `processString(str)` - 处理字符串
4. `createDataObject()` - 创建包含多个属性的对象
5. `processArray(arr)` - 处理数组（将每个元素乘以2）

## 构建和安装

### 前置要求

- Node.js (>= 10.0.0)
- node-gyp
- C++ 编译器（Windows: Visual Studio, Linux: GCC, macOS: Xcode）

### 安装

```bash
npm install
```

### 构建

**使用 npm 命令：**
```bash
npm run build
```

**使用 Windows 批处理脚本：**
```batch
build.bat
```

`build.bat` 脚本功能：
- 自动检查 Node.js 和 node-gyp 是否安装
- 自动安装缺失的依赖
- 交互式选择是否清理之前的构建
- 显示详细的构建步骤和结果
- 可选择是否运行测试

**快速重新编译：**
```batch
rebuild.bat
```

### 清理

**使用 npm 命令：**
```bash
npm run clean
```

**使用 Windows 批处理脚本：**
```batch
clean.bat
```

### 测试

```bash
npm test
```

## 使用示例

```javascript
const adapter = require('./index.js');

// 调用原生函数
console.log(adapter.helloWorld());
console.log(adapter.addNumbers(10, 20));
console.log(adapter.processString('Hello'));
console.log(adapter.createDataObject());
console.log(adapter.processArray([1, 2, 3]));
```

## 扩展开发

要添加新的导出函数：

1. 在 `napi_adapter.cpp` 中实现函数
2. 在 `Init()` 函数中注册函数
3. 在 `index.js` 中导出函数（可选）

示例：

```cpp
static napi_value MyFunction(napi_env env, napi_callback_info info) {
    // 实现你的逻辑
    return napi_adapter::CreateString(env, "Result");
}

// 在 Init() 中添加：
{ "myFunction", nullptr, MyFunction, nullptr, nullptr, nullptr, napi_default, nullptr },
```

## 注意事项

- 确保在使用 N-API 函数时正确处理错误
- 异步操作应使用 `AsyncWork` 类
- 注意内存管理，避免内存泄漏
- 在 Windows 上可能需要安装 Windows Build Tools

## 许可证

MIT


