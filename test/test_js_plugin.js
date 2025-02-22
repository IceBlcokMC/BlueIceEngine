console.log("__ENGINE_ID__: ", globalThis.__ENGINE_ID__);

const JSE = loadNativeClass("JSEAPI");
console.log(`JSEAPI: ${JSE}`);

const isWindows = JSE.isWindows;
console.log(`JSEAPI.isWindows: ${isWindows}`);
console.log(`JSEAPI.isWindows Call: ${isWindows()}`);

console.log("__declaration__: ", __declaration__);
console.log("__declaration__ Call: \n", __declaration__());
