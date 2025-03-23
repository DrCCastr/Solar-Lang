/***
 * @file types.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include "solar_pack.hpp"
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
        Char,
        String,
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
        {"char", TypeEnum::Char},
        {"string", TypeEnum::String},
    };

    struct Type {
        TypeEnum kind;
        string extra; // User-Type name
        string family; // "Int & Float = Number" and etc
        vector<string> parents;
        bool primaryType;

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
                case TypeEnum::Int: typeName = "Int"; break;
                case TypeEnum::Namespace: typeName = "Namespace"; break;
                case TypeEnum::Null: typeName = "Null"; break;
                case TypeEnum::String: typeName = "String"; break;
                case TypeEnum::Struct: typeName = "Struct"; break;
                case TypeEnum::Unknow: typeName = "Unknow"; break;
                case TypeEnum::UserType: typeName = "UserType"; break;
            }

            return typeName + (this->extra != "" ? "(" + this->extra + ")" : "");
        }

        Type(TypeEnum kind = TypeEnum::Unknow, string extra = "") : kind(kind), extra(extra), primaryType(true) {
            switch (kind) {
                case TypeEnum::Null: this->family = "Void"; break;
                case TypeEnum::Bool: this->family = "Bool"; break;
                case TypeEnum::Int: this->family = "Number"; break;
                case TypeEnum::Float: this->family = "Number"; break;
                case TypeEnum::Char: this->family = "Alfa"; break;
                case TypeEnum::String: this->family = "Alfa"; break;

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

}
