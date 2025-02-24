#pragma once
#include "V8Engine.h"
#include <atomic>
#include <endstone/scheduler/task.h>
#include <filesystem>
#include <memory>
#include <node.h>
#include <optional>
#include <unordered_map>
#include <vector>


namespace jse {


class NodeManager final {
private:
    bool                                        mIsInitialized{false}; // 是否初始化
    std::vector<std::string>                    mArgs;                 // 参数
    std::vector<std::string>                    mExecArgs;             // 执行参数
    std::unique_ptr<node::MultiIsolatePlatform> mPlatform;             // v8 平台
    std::unordered_map<EngineID, V8EnginePtr>   mEngines;              // 引擎列表

    std::atomic<bool> mUvLoopThreadRunning{true}; // uv loop 线程是否在运行

public:
    NodeManager()                              = default;
    NodeManager(const NodeManager&)            = delete;
    NodeManager& operator=(const NodeManager&) = delete;
    NodeManager(NodeManager&&)                 = delete;
    NodeManager& operator=(NodeManager&&)      = delete;

    static NodeManager& getInstance();

    void initNodeJs();

    void shutdownNodeJs();

    void initUvLoopThread();
    void shutdownUvLoopThread();

public:
    bool hasEngine(EngineID id) const;

    V8Engine* newScriptEngine();

    V8Engine* getEngine(EngineID id);

    bool destroyEngine(EngineID id);

    bool NpmInstall(std::string npmExecuteDir);

public:
    static bool loadFile(V8Engine* wrapper, std::filesystem::path const& file, bool esm = false);

    static std::optional<std::string> readFileContent(const std::filesystem::path& file);

    static std::optional<std::string> findMainScript(const std::filesystem::path& packagePath);
    static bool                       packageHasDependency(const std::filesystem::path& packagePath);
    static bool                       packageIsEsm(const std::filesystem::path& packagePath);
};


} // namespace jse
