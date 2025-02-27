"use strict";
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
function onLoad() {
  console.log("onLoad");
}
function onEnable() {
  console.log("onEnable");

  const self = Engine.getSelf(); // endstone::Plugin*
  console.log("self: ", self);
  console.log("self.name: ", self.getName());
}
function onDisable() {
  console.log("onDisable");
}

console.log("Engine: ", typeof Engine);

Engine.registerPlugin({
  name: "test_js_plugin",
  version: "1.0.0",

  onLoad: onLoad,
  onEnable: onEnable,
  onDisable: onDisable,
});
