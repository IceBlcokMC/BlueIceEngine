/**
 * ClassDefine 自动生成器
 * 注意：此工具只能生成基础的 ClassDefine
 *      .constructor()
 *      .function()
 *      .instanceFunction()
 *      以及继承关系。
 *
 * 不支持以下 ClassDefine：
 *      .property()
 *      .instanceProperty()
 *
 * 使用方法：
 *  node tools/defineClass.js ./src/api
 *  node tools/defineClass.js ./src/api/util/UUIDAPI.h
 *
 * 生成后的 ClassDefine 会直接覆盖到对应的 .cc 文件中
 * 提交前请务必检查生成后的代码是否正确。
 */
// @ts-ignore
import process from "process";
// @ts-ignore
import path from "path";
// @ts-ignore
import fs from "fs";
class Matcher {
    /**
     * 提取类名和继承信息
     * @param str .h 文件内容
     */
    static getHeaderClass(str) {
        let className = str.match(/class\s+(\w+)\s*:\s*public\s+(\w+)\s*{/); // 匹配class名(带继承)
        if (!className) {
            className = str.match(/class\s+(\w+)\s*{/); // 匹配class名(不带继承)
            if (!className) {
                return null;
            }
        }
        className.shift(); // 移除第一个元素
        for (let i = 0; i < className.length; i++) {
            const class_ = className[i];
            if (class_ == "ScriptClass") {
                className.splice(i, 1); // 移除ScriptClass
                continue;
            }
        }
        return {
            className: className[0],
            extends: className.length > 1 ? className[1] : null,
        };
    }
    /**
     * 提取所有方法名(只能提取 METHODS 宏定义的方法)
     * @param str .cc 文件内容
     * @returns 方法名
     */
    static getHeaderMethods(str) {
        const Function = /(static\s+)?METHODS\((\w+)\);/g;
        let result = [];
        let tmp;
        while ((tmp = Function.exec(str))) {
            result.push({
                funcName: tmp[2],
                isStatic: tmp[1] && tmp[1] == "static ",
            });
        }
        return result;
    }
    /**
     * 解析头文件中的 ClassDefine 定义
     * @param str .h 文件内容
     * @returns ClassDefine 定义
     */
    static getHeaderClassDefine(str) {
        const regex = /static ClassDefine<([^>]+)> (\w+);/;
        const match = str.match(regex);
        if (match) {
            return {
                defineClass: match[1],
                defineVariable: match[2],
            };
        }
        else {
            return null;
        }
    }
    /**
     * 从代码中提取ClassDefine
     * @param str .cc 文件内容
     * @returns ClassDefine 定义
     */
    static getCppClassDefine(str) {
        let result = str.match(/^ClassDefine[\s\S]+?\n[\s\S]+?\n(?:(?!\.build\(\);$)[\s\S])+?\.build\(\);/gm);
        if (result) {
            return result[0];
        }
        return null;
    }
    /**
     * 获取原有的ClassDefine constructor
     * @param str .cc 文件内容
     * @returns ClassDefine constructor 定义
     */
    static getCppClassDefineConstructor(str) {
        const result = this.getCppClassDefine(str);
        if (!result) {
            return null;
        }
        let constructor = result.match(/\.constructor\(&([^:]+)::([^)]+)\)/);
        if (constructor) {
            return {
                sourceCode: `&${constructor[1]}::${constructor[2]}`,
                className: constructor[1],
                funcName: constructor[2],
            };
        }
        return null;
    }
    static hasClassDefineInCpp(str) {
        return this.getCppClassDefine(str) != null;
    }
}
class Generators {
    /**
     * 递归查找指定文件，返回相对路径
     * @param dir 开始查找的目录
     * @param file 要查找的文件名
     * @returns 找到文件的相对路径，如果未找到返回 null
     */
    static findFile(dir, file) {
        const queue = [dir];
        while (queue.length > 0) {
            const currentDir = queue.shift();
            try {
                const entries = fs.readdirSync(currentDir);
                for (const entry of entries) {
                    const fullPath = path.join(currentDir, entry);
                    const stats = fs.statSync(fullPath);
                    if (stats.isDirectory()) {
                        queue.push(fullPath);
                    }
                    else if (stats.isFile() && entry === file) {
                        return path.relative(dir, fullPath);
                    }
                }
            }
            catch (err) {
                console.error(`Error reading directory ${currentDir}: ${err}`);
            }
        }
        return null;
    }
    static readFile(filePath) {
        return fs.readFileSync(filePath, "utf-8");
    }
    static writeFile(filePath, content) {
        fs.writeFileSync(filePath, content, "utf-8");
    }
    static getHeaderInfo(code) {
        let result = {
            className: "",
            defineClass: "",
            defineVariable: "",
            extends: null,
            methods: [],
        };
        const headerClass = Matcher.getHeaderClass(code);
        if (!headerClass) {
            return null;
        }
        result.className = headerClass.className;
        result.extends = headerClass.extends;
        const headerClassDefine = Matcher.getHeaderClassDefine(code);
        if (!headerClassDefine) {
            return null;
        }
        result.defineClass = headerClassDefine.defineClass;
        result.defineVariable = headerClassDefine.defineVariable;
        const headerMethods = Matcher.getHeaderMethods(code);
        if (!headerMethods) {
            return null;
        }
        result.methods = headerMethods;
        return result;
    }
    static getCppInfo(code) {
        return Matcher.getCppClassDefineConstructor(code);
    }
    static defineClass(headerInfo, cppInfo) {
        const fix = (str) => str.replace(/API/g, "");
        const ClassDefine = (h) => `ClassDefine<${h.defineClass}> ${h.className}::${h.defineVariable} = \n`;
        const defineClass = (h) => `\tdefineClass<${h.defineClass}>("${fix(h.className)}")\n`;
        const constructor_ = (c) => `\t\t.constructor(${c ? c.sourceCode : "nullptr"})\n`;
        const function_ = (h, className) => `\t\t.${h.isStatic ? "function" : "instanceFunction"}("${h.funcName}", &${className}::${h.funcName})\n`;
        const extendsAnnotation = (cur, par) => `\n\t\t// ${cur} extends ${par}\n`;
        const build = (h) => `\t\t.build();`;
        // ClassDefine 生成
        let def = "";
        def += ClassDefine(headerInfo);
        def += defineClass(headerInfo);
        if (headerInfo.defineClass != "void") {
            def += constructor_(cppInfo);
        }
        function genMethods(info, extends_ = false) {
            info.methods.forEach((fn) => {
                if (extends_ && fn.funcName == "toString") {
                    return; // skip
                }
                def += function_(fn, info.className);
            });
        }
        genMethods(headerInfo);
        // extends 生成
        const baseDir = "./src/api";
        function genExtends(currentClass, parentClass) {
            def += extendsAnnotation(currentClass.className, parentClass.className);
            genMethods(parentClass, true);
            if (parentClass.extends) {
                const file = Generators.findFile(baseDir, `${parentClass.extends}.h`);
                const fixedPath = path.join(baseDir, file);
                if (!fs.existsSync(fixedPath)) {
                    throw new Error(`${file} not found`);
                }
                const parentCode = Generators.readFile(fixedPath);
                const parentInfo = Generators.getHeaderInfo(parentCode);
                if (!parentInfo) {
                    throw new Error("parentInfo is null");
                }
                genExtends(parentClass, parentInfo);
            }
        }
        if (headerInfo.extends) {
            const file = Generators.findFile(baseDir, `${headerInfo.extends}.h`);
            const fixedPath = path.join(baseDir, file);
            if (!fs.existsSync(fixedPath)) {
                throw new Error(`${file} not found`);
            }
            const parentCode = Generators.readFile(fixedPath);
            const parentInfo = Generators.getHeaderInfo(parentCode);
            if (!parentInfo) {
                throw new Error("parentInfo is null");
            }
            genExtends(headerInfo, parentInfo);
        }
        def += build(headerInfo);
        return def;
    }
}
function processFile(headerFilePath) {
    const filename = path.basename(headerFilePath);
    if (!filename.endsWith(".h")) {
        return;
    }
    const exclude = ["APIHelper.h", "EnumAPI.h"];
    if (exclude.includes(filename)) {
        return;
    }
    const headerCode = Generators.readFile(headerFilePath);
    const headerInfo = Generators.getHeaderInfo(headerCode);
    if (!headerInfo) {
        return;
    }
    if (headerInfo.defineClass != headerInfo.className &&
        headerInfo.defineClass != "void") {
        throw new Error(`static ClassDefine<${headerInfo.defineClass}> != ${headerInfo.className}`);
    }
    console.log(`process file: ${headerFilePath}`);
    const cppFilePath = path.join(path.dirname(headerFilePath), path.basename(headerFilePath).replace(".h", ".cc"));
    const cppCode = Generators.readFile(cppFilePath);
    const cppInfo = Generators.getCppInfo(cppCode);
    let res = Generators.defineClass(headerInfo, cppInfo);
    let newCode;
    if (Matcher.hasClassDefineInCpp(cppCode)) {
        newCode = cppCode.replace(/^ClassDefine[\s\S]+?\n[\s\S]+?\n(?:(?!\.build\(\);$)[\s\S])+?\.build\(\);/gm, res);
    }
    else {
        res = "namespace jse {\n\n" + res;
        newCode = cppCode.replace(/^namespace jse \{/gm, res);
    }
    Generators.writeFile(cppFilePath, newCode);
}
function processDir(dir) {
    const files = fs.readdirSync(dir);
    files.forEach((file) => {
        const file_path = path.join(dir, file);
        if (fs.statSync(file_path).isDirectory()) {
            processDir(file_path);
            return;
        }
        processFile(file_path);
    });
}
function main(file) {
    if (fs.statSync(file).isDirectory()) {
        processDir(file);
    }
    else {
        processFile(file);
    }
}
main(process.argv[2]);
