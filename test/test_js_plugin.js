console.log("__ENGINE_ID__: ", globalThis.__ENGINE_ID__);

const JSE = loadNativeClass("jse::JSEAPI");
console.log(`JSEAPI: ${JSE}`);

const isWindows = JSE.isWindows;
console.log(`JSEAPI.isWindows: ${isWindows}`);
console.log(`JSEAPI.isWindows Call: ${isWindows()}`);
