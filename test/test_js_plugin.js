"use strict"
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
}
function onDisable() {
  console.log("onDisable");
}

__ENGINE_ID__++;
__ENGINE_ID__ = 10;

console.log("__ENGINE_ID__: ", __ENGINE_ID__);
