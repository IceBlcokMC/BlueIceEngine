#pragma once
#include "JSClassRegister.h"
#include "TypeInfo.hpp"
#include "puerts_impl/EnumImpl.h"
#include <cstdint>
#include <map>
#include <set>
#include <sstream>
#include <string>


namespace puerts {


struct DeclarationGenerator {
    std::map<std::string, std::vector<std::string>> module_to_classes;

    void GenArguments(const puerts::CFunctionInfo* Type, std::stringstream& ss) {
        for (unsigned int i = 0; i < Type->ArgumentCount(); i++) {
            if (i != 0) ss << ", ";
            auto argInfo = Type->Argument(i);

            ss << "p" << i;

            if (i >= Type->ArgumentCount() - Type->DefaultCount()) {
                ss << "?";
            }

            ss << ": ";

            if (strcmp(argInfo->Name(), "cstring") != 0 && !argInfo->IsUEType() && !argInfo->IsObjectType()
                && argInfo->IsPointer()) {
                ss << "ArrayBuffer";
            } else {
                const puerts::CTypeInfo* TypeInfo = Type->Argument(i);
                if (TypeInfo->IsEnum()) {
                    ss << TypeInfo->EnumTypeName(); // enum
                    continue;
                }

                bool IsReference = argInfo->IsRef();
                bool IsNullable  = !IsReference && argInfo->IsPointer();
                if (IsNullable) {
                    ss << "$Nullable<";
                }
                if (IsReference) {
                    ss << "$Ref<";
                }

                ss << TypeInfo->Name();

                if (IsNullable) {
                    ss << ">";
                }
                if (IsReference) {
                    ss << ">";
                }
            }
        }
    }

    static std::string RemoveNamespace(const std::string& name) {
        size_t lastColonPos = name.rfind("::");
        if (lastColonPos != std::string::npos) {
            return name.substr(lastColonPos + 2);
        }
        return name;
    }

    void GenClassName(const char* name, std::ostringstream& ss) {
        const char* pp = strchr(name, '.');
        while (pp) {
            const char* tmp = strchr(pp + 1, '.');
            if (tmp) {
                pp = tmp;
            } else // no next .
            {
                break;
            }
        }
        if (pp) {
            ss << (pp + 1);
        } else {
            ss << RemoveNamespace(name); // 去除命名空间
        }
    }

    void GenClass(const puerts::JSClassDefinition* ClassDefinition) {
        std::ostringstream Output;

        puerts::NamedFunctionInfo* ConstructorInfo = ClassDefinition->ConstructorInfos;
        if (!ConstructorInfo || !ConstructorInfo->Name || !ConstructorInfo->Type) {
            Output << "    /** @hideconstructor */" << "\n"; // 如果没有构造函数，则隐藏构造函数
        }

        Output << "    class ";
        GenClassName(ClassDefinition->ScriptName, Output);
        if (ClassDefinition->SuperTypeId) {
            Output << " extends ";
            GenClassName(puerts::FindClassByID(ClassDefinition->SuperTypeId)->ScriptName, Output);
        }
        Output << " {\n";

        std::set<std::string> AddedFunctions;

        while (ConstructorInfo && ConstructorInfo->Name && ConstructorInfo->Type) {
            std::stringstream Tmp;
            Tmp << "        constructor(";
            GenArguments(ConstructorInfo->Type, Tmp);
            Tmp << ");\n";
            if (AddedFunctions.find(Tmp.str()) == AddedFunctions.end()) {
                AddedFunctions.emplace(Tmp.str());
                Output << Tmp.str();
            }
            ++ConstructorInfo;
        }

        puerts::NamedPropertyInfo* PropertyInfo = ClassDefinition->PropertyInfos;
        while (PropertyInfo && PropertyInfo->Name && PropertyInfo->Type) {
            Output << "        " << PropertyInfo->Name << ": " << PropertyInfo->Type->Name() << ";\n";
            ++PropertyInfo;
        }

        puerts::NamedPropertyInfo* VariableInfo = ClassDefinition->VariableInfos;
        while (VariableInfo && VariableInfo->Name && VariableInfo->Type) {
            uint64_t Pos = VariableInfo - ClassDefinition->VariableInfos;
            Output << "        static " << (ClassDefinition->Variables[Pos].Setter ? "" : "readonly ")
                   << VariableInfo->Name << ": " << VariableInfo->Type->Name() << ";\n";
            ++VariableInfo;
        }

        puerts::NamedFunctionInfo* FunctionInfo = ClassDefinition->FunctionInfos;
        while (FunctionInfo && FunctionInfo->Name && FunctionInfo->Type) {
            std::stringstream Tmp;
            Tmp << "        static " << FunctionInfo->Name;
            if (FunctionInfo->Type->Return()) {
                Tmp << "(";
                GenArguments(FunctionInfo->Type, Tmp);
                const puerts::CTypeInfo* ReturnType = FunctionInfo->Type->Return();
                Tmp << "): " << ReturnType->Name() << ";\n";
            } else {
                Tmp << FunctionInfo->Type->CustomSignature() << ";\n";
            }
            if (AddedFunctions.find(Tmp.str()) == AddedFunctions.end()) {
                AddedFunctions.emplace(Tmp.str());
                Output << Tmp.str();
            }
            ++FunctionInfo;
        }

        puerts::NamedFunctionInfo* MethodInfo = ClassDefinition->MethodInfos;
        while (MethodInfo && MethodInfo->Name && MethodInfo->Type) {
            std::stringstream Tmp;
            Tmp << "        " << MethodInfo->Name;
            if (MethodInfo->Type->Return()) {
                Tmp << "(";
                GenArguments(MethodInfo->Type, Tmp);
                const puerts::CTypeInfo* ReturnType = MethodInfo->Type->Return();
                Tmp << "): " << ReturnType->Name() << ";\n";
            } else {
                Tmp << MethodInfo->Type->CustomSignature() << ";\n";
            }
            if (AddedFunctions.find(Tmp.str()) == AddedFunctions.end()) {
                AddedFunctions.emplace(Tmp.str());
                Output << Tmp.str();
            }
            ++MethodInfo;
        }

        Output << "    }\n\n";

        // std::string moudle_name(
        //     ClassDefinition->ScriptName,
        //     strchr(ClassDefinition->ScriptName, '.') - ClassDefinition->ScriptName
        // );
        const char* dot_position = strchr(ClassDefinition->ScriptName, '.');
        size_t      length =
            dot_position ? (dot_position - ClassDefinition->ScriptName) : strlen(ClassDefinition->ScriptName);

        std::string moudle_name(ClassDefinition->ScriptName, length);

        if (module_to_classes.find(moudle_name) == module_to_classes.end()) {
            module_to_classes[moudle_name] = std::vector<std::string>();
        }

        module_to_classes[moudle_name].push_back(Output.str());

        // std::cout << moudle_name << "_" << Output.str();
    }

    static std::string GetFirstNamepace(const std::string& input) {
        size_t pos = input.find("::");
        if (pos != std::string::npos) {
            return input.substr(0, pos);
        }
        return input;
    }

    static std::string GenerateEnumeration() {
        auto& enums = enum_impl::getAllEnums();

        std::ostringstream oss;
        std::string        rootNamespace;

        for (const auto& enum_full_name : enums) {
            if (rootNamespace.empty()) {
                rootNamespace = GetFirstNamepace(enum_full_name.first);
                oss << "declare namespace " << rootNamespace << " {\n";
            }

            oss << "    enum " << RemoveNamespace(enum_full_name.first) << " {\n";

            for (const auto& enum_value : enum_full_name.second) {
                oss << "        " << enum_value.first << " = " << enum_value.second << ",\n";
            }

            oss << "    }\n\n";
        }
        oss << "}\n";

        return oss.str();
    }

    std::string GetOutput() {
        std::ostringstream NativeTypeMap; // 类型映射表 C++ FullName -> Js declare type
        std::ostringstream DeclareTypes;  // 声明类型
        std::string        RootNamespace; // 根命名空间

        NativeTypeMap << "declare type NativeTypeMap = {\n";
        DeclareTypes << "declare namespace ";

        for (const auto& pair : module_to_classes) {
            if (RootNamespace.empty()) {
                RootNamespace = GetFirstNamepace(pair.first);

                DeclareTypes << RootNamespace << " {\n"; // 全局命名空间
                // DeclareTypes << "    import {$Ref, $Nullable, cstring} from \"puerts\"\n\n";
            }
            NativeTypeMap << "    \"" << pair.first << "\": ";                             // Key
            NativeTypeMap << RootNamespace << "." << RemoveNamespace(pair.first) << ",\n"; // Value

            for (const auto& i : pair.second) {
                DeclareTypes << i;
            }
        }
        NativeTypeMap << "}\n";
        DeclareTypes << "}\n";

        NativeTypeMap << "\n" << "declare type NativeClasses = keyof NativeTypeMap;" << "\n";

        std::ostringstream Output;
        Output << GenerateEnumeration() << "\n";
        Output << NativeTypeMap.str() << "\n" << DeclareTypes.str();
        return Output.str();
    }
};


} // namespace puerts
