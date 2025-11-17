#include "napi_adapter.h"
#include <vector>
#include <string>

// 示例导出函数：Hello World
static napi_value HelloWorld(napi_env env, napi_callback_info info) {
    napi_value result;
    napi_create_string_utf8(env, "Hello from N-API!", NAPI_AUTO_LENGTH, &result);
    return result;
}

// 示例导出函数：添加两个数字
static napi_value AddNumbers(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    if (argc < 2) {
        napi_adapter::ThrowError(env, "Expected 2 arguments");
        return nullptr;
    }

    if (!napi_adapter::IsNumber(env, args[0]) || !napi_adapter::IsNumber(env, args[1])) {
        napi_adapter::ThrowError(env, "Arguments must be numbers");
        return nullptr;
    }

    double a = napi_adapter::GetDouble(env, args[0]);
    double b = napi_adapter::GetDouble(env, args[1]);
    
    return napi_adapter::CreateNumber(env, a + b);
}

// 示例导出函数：处理字符串
static napi_value ProcessString(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    if (argc < 1) {
        napi_adapter::ThrowError(env, "Expected 1 argument");
        return nullptr;
    }

    if (!napi_adapter::IsString(env, args[0])) {
        napi_adapter::ThrowError(env, "Argument must be a string");
        return nullptr;
    }

    std::string input = napi_adapter::GetString(env, args[0]);
    std::string output = "Processed: " + input;
    
    return napi_adapter::CreateString(env, output);
}

// 示例导出函数：创建对象
static napi_value CreateDataObject(napi_env env, napi_callback_info info) {
    napi_value obj = napi_adapter::CreateObject(env);
    
    napi_adapter::SetProperty(env, obj, "name", napi_adapter::CreateString(env, "N-API Adapter"));
    napi_adapter::SetProperty(env, obj, "version", napi_adapter::CreateString(env, "1.0.0"));
    napi_adapter::SetProperty(env, obj, "count", napi_adapter::CreateInt32(env, 42));
    napi_adapter::SetProperty(env, obj, "active", napi_adapter::CreateBoolean(env, true));
    
    return obj;
}

// 示例导出函数：处理数组
static napi_value ProcessArray(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    if (argc < 1) {
        napi_adapter::ThrowError(env, "Expected 1 argument");
        return nullptr;
    }

    if (!napi_adapter::IsArray(env, args[0])) {
        napi_adapter::ThrowError(env, "Argument must be an array");
        return nullptr;
    }

    uint32_t length = napi_adapter::GetArrayLength(env, args[0]);
    napi_value result = napi_adapter::CreateArray(env, length);
    
    for (uint32_t i = 0; i < length; i++) {
        napi_value element = napi_adapter::GetElement(env, args[0], i);
        double value = napi_adapter::GetDouble(env, element);
        napi_adapter::SetElement(env, result, i, napi_adapter::CreateNumber(env, value * 2));
    }
    
    return result;
}

// 模块初始化函数
static napi_value Init(napi_env env, napi_value exports) {
    // 定义导出的函数
    napi_property_descriptor desc[] = {
        { "helloWorld", nullptr, HelloWorld, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "addNumbers", nullptr, AddNumbers, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "processString", nullptr, ProcessString, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "createDataObject", nullptr, CreateDataObject, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "processArray", nullptr, ProcessArray, nullptr, nullptr, nullptr, napi_default, nullptr },
    };

    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    
    return exports;
}

// N-API 模块注册
NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)




