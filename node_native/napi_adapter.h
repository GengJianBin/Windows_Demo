#ifndef NAPI_ADAPTER_H
#define NAPI_ADAPTER_H

#include <node_api.h>
#include <string>
#include <vector>
#include <memory>

namespace napi_adapter {

// 错误处理辅助函数
inline napi_value CreateError(napi_env env, const char* message) {
    napi_value error;
    napi_create_string_utf8(env, message, NAPI_AUTO_LENGTH, &error);
    napi_value error_obj;
    napi_create_error(env, nullptr, error, &error_obj);
    return error_obj;
}

inline void ThrowError(napi_env env, const char* message) {
    napi_value error = CreateError(env, message);
    napi_throw(env, error);
}

// 类型检查辅助函数
inline bool IsString(napi_env env, napi_value value) {
    napi_valuetype type;
    napi_typeof(env, value, &type);
    return type == napi_string;
}

inline bool IsNumber(napi_env env, napi_value value) {
    napi_valuetype type;
    napi_typeof(env, value, &type);
    return type == napi_number;
}

inline bool IsObject(napi_env env, napi_value value) {
    napi_valuetype type;
    napi_typeof(env, value, &type);
    return type == napi_object;
}

inline bool IsArray(napi_env env, napi_value value) {
    bool result;
    napi_is_array(env, value, &result);
    return result;
}

inline bool IsFunction(napi_env env, napi_value value) {
    napi_valuetype type;
    napi_typeof(env, value, &type);
    return type == napi_function;
}

// 类型转换辅助函数
inline std::string GetString(napi_env env, napi_value value) {
    size_t length;
    napi_get_value_string_utf8(env, value, nullptr, 0, &length);
    std::string result(length, '\0');
    napi_get_value_string_utf8(env, value, &result[0], length + 1, &length);
    return result;
}

inline std::wstring GetStringW(napi_env env, napi_value value) {
    size_t length;
    napi_get_value_string_utf16(env, value, nullptr, 0, &length);
    std::wstring result(length, L'\0');
    napi_get_value_string_utf16(env, value, reinterpret_cast<char16_t*>(&result[0]), length + 1, &length);
    return result;
}

inline int32_t GetInt32(napi_env env, napi_value value) {
    int32_t result;
    napi_get_value_int32(env, value, &result);
    return result;
}

inline uint32_t GetUint32(napi_env env, napi_value value) {
    uint32_t result;
    napi_get_value_uint32(env, value, &result);
    return result;
}

inline int64_t GetInt64(napi_env env, napi_value value) {
    int64_t result;
    napi_get_value_int64(env, value, &result);
    return result;
}

inline double GetDouble(napi_env env, napi_value value) {
    double result;
    napi_get_value_double(env, value, &result);
    return result;
}

inline bool GetBoolean(napi_env env, napi_value value) {
    bool result;
    napi_get_value_bool(env, value, &result);
    return result;
}

// 创建 JavaScript 值
inline napi_value CreateString(napi_env env, const char* str) {
    napi_value result;
    napi_create_string_utf8(env, str, NAPI_AUTO_LENGTH, &result);
    return result;
}

inline napi_value CreateString(napi_env env, const std::string& str) {
    return CreateString(env, str.c_str());
}

inline napi_value CreateStringW(napi_env env, const std::wstring& str) {
    napi_value result;
    napi_create_string_utf16(env, reinterpret_cast<const char16_t*>(str.c_str()), str.length(), &result);
    return result;
}

inline napi_value CreateNumber(napi_env env, double value) {
    napi_value result;
    napi_create_double(env, value, &result);
    return result;
}

inline napi_value CreateInt32(napi_env env, int32_t value) {
    napi_value result;
    napi_create_int32(env, value, &result);
    return result;
}

inline napi_value CreateUint32(napi_env env, uint32_t value) {
    napi_value result;
    napi_create_uint32(env, value, &result);
    return result;
}

inline napi_value CreateInt64(napi_env env, int64_t value) {
    napi_value result;
    napi_create_int64(env, value, &result);
    return result;
}

inline napi_value CreateBoolean(napi_env env, bool value) {
    napi_value result;
    napi_get_boolean(env, value, &result);
    return result;
}

inline napi_value CreateArray(napi_env env, size_t length) {
    napi_value result;
    napi_create_array_with_length(env, length, &result);
    return result;
}

inline napi_value CreateObject(napi_env env) {
    napi_value result;
    napi_create_object(env, &result);
    return result;
}

inline napi_value CreateNull(napi_env env) {
    napi_value result;
    napi_get_null(env, &result);
    return result;
}

inline napi_value CreateUndefined(napi_env env) {
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

// 对象属性操作
inline void SetProperty(napi_env env, napi_value object, const char* name, napi_value value) {
    napi_value key;
    napi_create_string_utf8(env, name, NAPI_AUTO_LENGTH, &key);
    napi_set_property(env, object, key, value);
}

inline napi_value GetProperty(napi_env env, napi_value object, const char* name) {
    napi_value key;
    napi_create_string_utf8(env, name, NAPI_AUTO_LENGTH, &key);
    napi_value result;
    napi_get_property(env, object, key, &result);
    return result;
}

inline bool HasProperty(napi_env env, napi_value object, const char* name) {
    napi_value key;
    napi_create_string_utf8(env, name, NAPI_AUTO_LENGTH, &key);
    bool result;
    napi_has_property(env, object, key, &result);
    return result;
}

// 数组操作
inline void SetElement(napi_env env, napi_value array, uint32_t index, napi_value value) {
    napi_set_element(env, array, index, value);
}

inline napi_value GetElement(napi_env env, napi_value array, uint32_t index) {
    napi_value result;
    napi_get_element(env, array, index, &result);
    return result;
}

inline uint32_t GetArrayLength(napi_env env, napi_value array) {
    uint32_t length;
    napi_get_array_length(env, array, &length);
    return length;
}

// 回调函数调用
inline napi_value CallFunction(napi_env env, napi_value func, napi_value this_arg, 
                               size_t argc, const napi_value* argv) {
    napi_value result;
    napi_call_function(env, this_arg, func, argc, argv, &result);
    return result;
}

// 异步工作辅助类
class AsyncWork {
public:
    AsyncWork(napi_env env, napi_value callback, const char* resource_name = nullptr)
        : env_(env), callback_(nullptr), async_work_(nullptr) {
        napi_create_reference(env, callback, 1, &callback_);
        napi_value resource_name_val = nullptr;
        if (resource_name) {
            napi_create_string_utf8(env, resource_name, NAPI_AUTO_LENGTH, &resource_name_val);
        }
        napi_create_async_work(env, nullptr, resource_name_val, Execute, Complete, this, &async_work_);
    }

    virtual ~AsyncWork() {
        if (callback_) {
            napi_delete_reference(env_, callback_);
        }
        if (async_work_) {
            napi_delete_async_work(env_, async_work_);
        }
    }

    void Queue() {
        napi_queue_async_work(env_, async_work_);
    }

protected:
    virtual void ExecuteWork() = 0;
    virtual void CompleteWork(napi_value error, napi_value result) = 0;

private:
    static void Execute(napi_env env, void* data) {
        AsyncWork* work = static_cast<AsyncWork*>(data);
        work->ExecuteWork();
    }

    static void Complete(napi_env env, napi_status status, void* data) {
        AsyncWork* work = static_cast<AsyncWork*>(data);
        napi_value error = nullptr;
        napi_value result = nullptr;
        
        if (status != napi_ok) {
            napi_value error_msg;
            napi_create_string_utf8(env, "Async work failed", NAPI_AUTO_LENGTH, &error_msg);
            napi_create_error(env, nullptr, error_msg, &error);
        }
        
        work->CompleteWork(error, result);
        
        napi_value callback;
        napi_get_reference_value(env, work->callback_, &callback);
        napi_value global;
        napi_get_global(env, &global);
        
        if (error) {
            napi_value argv[1] = { error };
            napi_call_function(env, global, callback, 1, argv, nullptr);
        } else {
            napi_value argv[2] = { nullptr, result };
            napi_call_function(env, global, callback, 2, argv, nullptr);
        }
    }

    napi_env env_;
    napi_ref callback_;
    napi_async_work async_work_;
};

} // namespace napi_adapter

#endif // NAPI_ADAPTER_H




