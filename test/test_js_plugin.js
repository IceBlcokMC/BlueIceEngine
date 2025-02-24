console.log("__ENGINE_ID__: ", __ENGINE_ID__);
console.log("__declaration__: ", __declaration__);
console.log("loadNativeClass: ", loadNativeClass);

// 测试 dts 生成
function GenerateDTS() {
  const fs = require("fs");
  const dts = __declaration__();
  const path = "D:\\Codes\\Js_Engine\\types\\AutoGen.d.ts";
  console.log(`GenerateDTS: \n${dts}`);
  fs.writeFileSync(path, dts);
}
GenerateDTS();

// 测试 Native 类加载
const assert = (condition, message) => {
  if (!condition) {
    throw new Error(message || "Assertion failed");
  }
};

const NativeClasses = ["JSEAPI"];
NativeClasses.forEach((cls) => {
  const tryLoad = loadNativeClass(cls);
  assert(tryLoad, `Failed to load native class ${cls}`);
});
