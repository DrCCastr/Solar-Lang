/***
 * @file types.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "lexer/pack.hpp"
#include <unordered_map>

using namespace std;

//////////
// Code //
//////////

namespace Solar {

    enum class TypeEnum {
        Auto,
        Unknow,

        Null,
        Bool,
        Int,
        Float,
        Double,
        Char,
        Function,

        UserType,
        Struct,
        Class,
        Namespace,
    };

    const unordered_map<string, TypeEnum> primaryTypes = {
        {"null", TypeEnum::Null},
        {"bool", TypeEnum::Bool},
        {"int", TypeEnum::Int},
        {"float", TypeEnum::Float},
        {"double", TypeEnum::Double},
        {"char", TypeEnum::Char},
        {"func", TypeEnum::Function},
    };

    struct Type {
        TypeEnum kind;
        string extra; // User-Type name
        string family; // "Int & Float = Number" and etc
        bool isPointer;
        vector<string> parents;
        bool primaryType;
        vector<Type> generics;
        vector<vector<Type>> unsizedGenerics;

        bool compare(Type other) const {
            if (other.kind != this->kind) return false;
            if (other.extra != this->extra) return false;

            return true;
        }

        bool canConvert(Type other) const {
            if (other.kind == this->kind) return true;
            if ((other.family == this->family) && (this->primaryType == true && other.primaryType == true)) return true;

            if (([this, other](){
                for (const auto& parent : this->parents) {
                    for (const auto& otherParent : other.parents) {
                        if (parent == otherParent) return true;
                    }
                }

                return false;
            })()) return true;

            if ((this->extra == other.extra) && (this->primaryType == false && other.primaryType == false)) return true;

            return false;
        }

        string toString() const {
            string typeName;

            switch (this->kind) {
                case TypeEnum::Auto: typeName = "Auto"; break;
                case TypeEnum::Bool: typeName = "Bool"; break;
                case TypeEnum::Char: typeName = "Char"; break;
                case TypeEnum::Class: typeName = "Class"; break;
                case TypeEnum::Float: typeName = "Float"; break;
                case TypeEnum::Double: typeName = "Double"; break;
                case TypeEnum::Int: typeName = "Int"; break;
                case TypeEnum::Function: typeName = "Function"; break;
                case TypeEnum::Namespace: typeName = "Namespace"; break;
                case TypeEnum::Null: typeName = "Null"; break;
                case TypeEnum::Struct: typeName = "Struct"; break;
                case TypeEnum::Unknow: typeName = "Unknow"; break;
                case TypeEnum::UserType: typeName = "UserType"; break;
            }

            return typeName + (this->extra != "" ? "(" + this->extra + ")" : "");
        }

        Type(TypeEnum kind = TypeEnum::Unknow, bool pointer = false, string extra = "", vector<Type> generics = {}, vector<vector<Type>> unsizedGenerics = {})
        : kind(kind), extra(extra), isPointer(pointer), primaryType(true), generics(generics), unsizedGenerics(unsizedGenerics) {
            switch (kind) {
                case TypeEnum::Null: this->family = "Void"; break;
                case TypeEnum::Bool: this->family = "Bool"; break;
                case TypeEnum::Int: this->family = "Number"; break;
                case TypeEnum::Double: this->family = "Double"; break;
                case TypeEnum::Float: this->family = "Number"; break;
                case TypeEnum::Char: this->family = "Alfa"; break;
                case TypeEnum::Function: this->family = "Function"; break;

                case TypeEnum::Auto: // Same unknow
                case TypeEnum::Unknow: this->family = "None"; break;

                default: {
                    this->family = "Custom";
                    this->primaryType = false;
                    break;
                }
            }
        };
    };

    inline bool operator==(const Type& a, const Type& b) {
        return a.compare(b);
    }

}
