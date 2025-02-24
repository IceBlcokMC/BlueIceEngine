console.log("__ENGINE_ID__: ", __ENGINE_ID__);
console.log("__declaration__: ", __declaration__);
console.log("loadNativeClass: ", loadNativeClass);

// 测试 dts 生成
function GenerateDTS() {
  const fs = require("fs");
  const dts = __declaration__();
  const path = "D:\\Codes\\Js_Engine\\types\\AutoGen.d.ts";
  fs.writeFileSync(path, dts);
}
GenerateDTS();

// 测试注册
const JSE = loadNativeClass("JSEAPI");
console.log("JSE", JSE);
console.log("JSE.registerPlugin:", JSE.registerPlugin);
console.log("JSE.isWindows:", JSE.isWindows, JSE.isWindows());
console.log("JSE.isLinux:", JSE.isLinux, JSE.isLinux());

function ref(x) {
  return [x];
}

const TestBuilder = loadNativeClass("TestBuilder");
const obj = new TestBuilder();
obj.name = "test";
obj.version = "1.0.0";
obj.description = "test";
obj.onLoad = function () {
  console.log("onLoad");
};
obj.onEnable = function () {
  console.log("onEnable");
};
obj.onDisable = function () {
  console.log("onDisable");
};

JSE.registerPlugin(obj); // TODO: Fix this，此方法不知道为什么会引发异常：Error: access a null object
// [JsEngine] Uncaught v8_exception: Error: access a null object
// [JsEngine] Error: access a null object
// [JsEngine]     at Object.<anonymous> (D:\Codes\Js_Engine\test\test_js_plugin.js:40:5)
// [JsEngine]     at Module._compile (node:internal/modules/cjs/loader:1565:14)
// [JsEngine]     at Module._extensions..js (node:internal/modules/cjs/loader:1708:10)
// [JsEngine]     at Module.load (node:internal/modules/cjs/loader:1318:32)
// [JsEngine]     at Module._load (node:internal/modules/cjs/loader:1128:12)
// [JsEngine]     at TracingChannel.traceSync (node:diagnostics_channel:322:14)
// [JsEngine]     at wrapModuleLoad (node:internal/modules/cjs/loader:219:24)
// [JsEngine]     at Module.require (node:internal/modules/cjs/loader:1340:12)
// [JsEngine]     at require (node:internal/modules/helpers:138:16)
// [JsEngine]     at D:\Codes\bedrock_server\bedrock_server.exe:21:21