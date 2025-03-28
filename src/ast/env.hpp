/***
 * @file env.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "pack.hpp"

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    struct AstEnv {
        Type returnType;
        bool autoRetType;
        bool isFunc;
        unordered_map<string, Type> userTypes;
        unordered_map<string, Type> variables;
        unordered_map<string, Type> arguments;
        unordered_map<string, Type> functions;
        AstEnv* parent;

        AstEnv(AstEnv* parent = nullptr, bool isFunc = true) : autoRetType(true), isFunc(isFunc), parent(parent) {}

        bool hasVariable(const string& varName) {
            if (this->variables.find(varName) != this->variables.end()) {
                return true;
            }
            if (this->parent != nullptr) { 
                return this->parent->hasVariable(varName);
            }
            return false;
        }

        bool hasParameter(const string& paramName) {
            if (this->arguments.find(paramName) != this->arguments.end()) {
                return true;
            }
            if (this->parent != nullptr) {
                return this->parent->hasParameter(paramName);
            }
            return false;
        }

        bool hasFunction(const string& funcName) {
            if (this->functions.find(funcName) != this->functions.end()) {
                return true;
            }
            if (this->parent != nullptr) {
                return this->parent->hasFunction(funcName);
            }
            return false;
        }

        bool hasUserType(const string& typeName) {
            if (this->userTypes.find(typeName) != this->userTypes.end()) {
                return true;
            }
            if (this->parent != nullptr) {
                return this->parent->hasUserType(typeName);
            }
            return false;
        }

        bool hasValue(const string& valueName) {
            return this->hasVariable(valueName) || this->hasParameter(valueName) || this->hasFunction(valueName);
        }

        void addVariable(const string& varName, const Type& varType) {
            this->variables[varName] = varType;
        }

        void addParameter(const string& paramName, const Type& paramType) {
            this->arguments[paramName] = paramType;
        }

        void addUserType(const string& typeName, const Type& type) {
            this->userTypes[typeName] = type;
        }

        void addFunction(const string& funcName, const Type& retType, const vector<Type>& argTypes) {
            this->functions[funcName] = Type(TypeEnum::Function, false, "", {retType}, {argTypes});
        }

        Type getVariableType(const string& varName) {
            if (this->variables.find(varName) != this->variables.end()) {
                return this->variables[varName];
            }
            if (this->parent != nullptr) {
                return this->parent->getVariableType(varName);
            }
            return Type();
        }

        Type getParameterType(const string& paramName) {
            if (this->arguments.find(paramName) != this->arguments.end()) {
                return this->arguments[paramName];
            }
            if (this->parent != nullptr) {
                return this->parent->getParameterType(paramName);
            }
            return Type();
        }

        Type getUserType(const string& typeName) {
            if (this->userTypes.find(typeName) != this->userTypes.end()) {
                return this->userTypes[typeName];
            }
            if (this->parent != nullptr) {
                return this->parent->getUserType(typeName);
            }
            return Type();
        }

        Type getFuncType(const string& funcName) {
            if (this->functions.find(funcName) != this->functions.end()) {
                return this->functions[funcName];
            }
            if (this->parent != nullptr) {
                return this->parent->getFuncType(funcName);
            }
            return Type();
        }

        Type getValueType(const string& valueName) {
            if (this->hasVariable(valueName)) {
                return this->getVariableType(valueName);
            }
            if (this->hasParameter(valueName)) {
                return this->getParameterType(valueName);
            }
            if (this->hasFunction(valueName)) {
                return this->getFuncType(valueName);
            }
            return Type();
        }
    };

}