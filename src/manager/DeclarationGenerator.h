#pragma once
#include "JSClassRegister.h"
#include "TypeInfo.hpp"
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
                bool IsReference = argInfo->IsRef();
                bool IsNullable  = !IsReference && argInfo->IsPointer();
                if (IsNullable) {
                    ss << "$Nullable<";
                }
                if (IsReference) {
                    ss << "$Ref<";
                }

                const puerts::CTypeInfo* TypeInfo = Type->Argument(i);
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

    // void GenClassName(const char* name, std::stringstream& ss) {
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
            ss << name;
        }
    }

    void GenClass(const puerts::JSClassDefinition* ClassDefinition) {
        // std::stringstream Output;
        std::ostringstream Output;

        Output << "    class ";
        GenClassName(ClassDefinition->ScriptName, Output);
        if (ClassDefinition->SuperTypeId) {
            Output << " extends ";
            GenClassName(puerts::FindClassByID(ClassDefinition->SuperTypeId)->ScriptName, Output);
        }
        Output << " {\n";

        std::set<std::string> AddedFunctions;

        puerts::NamedFunctionInfo* ConstructorInfo = ClassDefinition->ConstructorInfos;
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
            int Pos = VariableInfo - ClassDefinition->VariableInfos;
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
                Tmp << ") :" << ReturnType->Name() << ";\n";
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
                Tmp << ") :" << ReturnType->Name() << ";\n";
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

    std::string GetOutput() {
        std::stringstream Output;
        for (const auto& pair : module_to_classes) {
            Output << "declare module \"" << pair.first << "\" {\n";
            Output << "    import {$Ref, $Nullable, cstring} from \"puerts\"\n\n";
            for (int i = 0; i < pair.second.size(); i++) {
                Output << pair.second[i];
            }
            Output << "}\n";
        }
        return Output.str();
    }
};


} // namespace puerts
