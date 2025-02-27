"use strict";
console.log("Engine: ", typeof Engine);
console.log("__ENGINE_ID__: ", __ENGINE_ID__);
console.log("loadNativeClass: ", loadNativeClass);

//----------------------------------------------------------------
// Dts 生成
//----------------------------------------------------------------
function Test_GetDeclaration() {
  const fs = require("fs");
  const dts = Engine.getDeclaration();
  const path = "D:\\Codes\\Js_Engine\\types\\AutoGen.d.ts";
  fs.writeFileSync(path, dts);
}

//----------------------------------------------------------------
// 单测
//----------------------------------------------------------------
function assertType(var_, type) {
  if (Object.prototype.toString.call(var_) !== `[object ${type}]`) {
    throw new Error(
      `Expected ${type}, got ${Object.prototype.toString.call(var_)}`
    );
  }
}

function Test_PluginBind() {
  const self = Engine.getSelf(); // endstone::Plugin*
  assertType(self, "Object");
  assertType(self.getName(), "String");
}

//----------------------------------------------------------------
// 注册
//----------------------------------------------------------------
Engine.registerPlugin({
  name: "test_js_plugin",
  version: "1.0.0",

  onLoad: () => {
    console.log("onLoad called");

    Test_GetDeclaration();
  },
  onEnable: () => {
    console.log("onEnable called");

    Test_PluginBind();
  },
  onDisable: () => {
    console.log("onDisable called");
  },
});
