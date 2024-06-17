#ifndef MAGIC_CPP_TYPE_TREE_H
#define MAGIC_CPP_TYPE_TREE_H

#include "raw_name.h"
#include "type_traits.h"
#include <string>
#include <vector>
#include <cstdint> // for std::uint*_t

namespace magic
{
    template <typename T>
    struct type_info;
}

namespace magic::details
{
    template <typename T, typename U = void>
    struct has_name : std::false_type
    {
    };

    template <typename T>
    struct has_name<T, std::void_t<decltype(type_info<T>::name)>> : std::true_type
    {
    };

    enum class TypeKind
    {
        POINTER,
        REFERENCE,
        ARRAY,
        FUNCTION,
        MEMBER,
        TEMPLATE,
        BASIC,
        NTTP
    };

    struct Type
    {
        virtual ~Type() = default;
        virtual TypeKind Kind() const = 0;
    };

    struct BasicType : public Type
    {
        std::string name;
        bool is_const;
        bool is_volatile;

        virtual ~BasicType(){};
        virtual TypeKind Kind() const override { return TypeKind::BASIC; }
    };

    struct NTTP : public Type
    {
        Type* type;
        std::string name;

        virtual ~NTTP(){};
        virtual TypeKind Kind() const override { return TypeKind::NTTP; }
    };

    struct Pointer : public Type
    {
        Type* pointee;
        bool is_const;
        bool is_volatile;

        virtual ~Pointer() { delete pointee; }
        virtual TypeKind Kind() const override { return TypeKind::POINTER; }
    };

    struct Reference : public Type
    {
        Type* referencee;
        bool is_lvalue_ref;

        virtual ~Reference() { delete referencee; }
        virtual TypeKind Kind() const override { return TypeKind::REFERENCE; }
    };

    struct Array : public Type
    {
        Type* element;
        std::size_t size;

        virtual ~Array() { delete element; }
        virtual TypeKind Kind() const override { return TypeKind::ARRAY; }
    };

    struct Member : public Type
    {
        Type* class_type;
        Type* member_type;
        bool is_const;
        bool is_volatile;

        virtual ~Member()
        {
            delete class_type;
            delete member_type;
        }

        virtual TypeKind Kind() const override { return TypeKind::MEMBER; }
    };

    struct Template : public Type
    {
        std::string name;
        std::vector<Type*> parameters;
        bool is_const;
        bool is_volatile;

        virtual ~Template()
        {
            for (auto parameter : parameters) { delete parameter; }
        }

        virtual TypeKind Kind() const override { return TypeKind::TEMPLATE; }
    };

    struct Function : public Type
    {
        Type* return_type;
        std::vector<Type*> parameters;
        std::uint8_t flag;

        virtual ~Function()
        {
            delete return_type;
            for (auto parameter : parameters) { delete parameter; }
        }

        void setflag(bool is_const, bool is_volatile, bool is_lvalue_ref, bool is_rvalue_ref, bool is_noexcept, bool is_variadic)
        {
            flag = 0;
            flag |= is_const ? 1 : 0;
            flag |= is_volatile ? 2 : 0;
            flag |= is_lvalue_ref ? 4 : 0;
            flag |= is_rvalue_ref ? 8 : 0;
            flag |= is_noexcept ? 16 : 0;
            flag |= is_variadic ? 32 : 0;
        }

        bool isConst() { return flag & 1; }
        bool isVolatile() { return flag & 2; }
        bool isLvalueRef() { return flag & 4; }
        bool isRvalueRef() { return flag & 8; }
        bool isNoexcept() { return flag & 16; }
        bool isVariadic() { return flag & 32; }

        virtual TypeKind Kind() const override { return TypeKind::FUNCTION; }
    };

    struct never_functor
    {
    };

    struct always_functor
    {
        void operator()();
    };

    template <typename T>
    struct is_functor_impl : always_functor, std::conditional_t<std::is_class_v<T>, T, never_functor>
    {
    };

    template <typename T, typename U = void>
    struct is_functor : std::true_type
    {
    };

    template <typename T>
    struct is_functor<T, std::void_t<decltype(&is_functor_impl<T>::operator())>> : std::false_type
    {
    };

    template <typename T>
    constexpr bool is_functor_v = is_functor<T>::value;

    template <typename T>
    constexpr bool is_lambda()
    {
        if constexpr (is_functor_v<T>)
        {
            constexpr auto name = raw_name_of<T>();
            return name.find("lambda") != std::string_view::npos;
        }
        else
        {
            return false;
        }
    }
    template <typename T>
    Type* parse(bool is_full_name);

    template <auto Value>
    Type* parse_nttp(bool is_full_name)
    {
        NTTP* nttp = new NTTP{};
        nttp->type = parse<std::remove_cv_t<decltype(Value)>>(is_full_name);
        using Type = decltype(Value);
        if constexpr (std::is_enum_v<Type>)
        {
            nttp->name = raw_name_of_enum<Value>();
        }
        else if constexpr (std::is_same_v<Type, char>)
        {
            nttp->name = std::string{Value, 1};
        }
        else if constexpr (std::is_integral_v<Type> || std::is_floating_point_v<Type>)
        {
            nttp->name = std::to_string(Value);
        }
        else if constexpr (is_lambda<Type>())
        {
            nttp->name = "<lambda object>";
        }
        else
        {
            nttp->name = raw_name_of<Value>();
        }
        return nttp;
    }

    inline std::size_t lambda_id = 0;

    template <typename T>
    std::string resolve_lambda()
    {
        static std::size_t id = lambda_id++;
        return "<lambda{" + std::to_string(id) + "}>";
    }

    template <typename Tuple, std::size_t... Is>
    void parse_function_params(std::index_sequence<Is...>, std::vector<Type*>& parameters, bool is_full_name)
    {
        ((parameters[Is] = (parse<std::tuple_element_t<Is, Tuple>>(is_full_name))), ...);
    }

    template <typename Tuple, std::size_t... Is>
    void parse_template_params(std::index_sequence<Is...>, std::vector<Type*>& parameters, bool is_full_name)
    {
        std::size_t index = 0;
        auto f = [&](auto param)
        {
            using Param = decltype(param);
            if constexpr (!Param::has_value)
            {
                parameters[index] = parse<typename Param::type>(is_full_name);
            }
            else
            {
                parameters[index] = parse_nttp<Param::value>(is_full_name);
            }
            index += 1;
        };
        (f(std::tuple_element_t<Is, Tuple>{}), ...);
    }

    template <typename T>
    Type* parse(bool is_full_name)
    {
        constexpr auto is_const = std::is_const_v<T>;
        constexpr auto is_volatile = std::is_volatile_v<T>;
        using Primary = std::remove_cv_t<T>;

        if constexpr (has_name<T>::value)
        {
            if (!is_full_name)
            {
                BasicType* basic_type = new BasicType{};
                basic_type->name = type_info<T>::name;
                basic_type->is_const = false;
                basic_type->is_volatile = false;
                return basic_type;
            }
        }
        else if constexpr (has_name<Primary>::value)
        {
            if (!is_full_name)
            {
                BasicType* basic_type = new BasicType{};
                basic_type->name = type_info<Primary>::name;
                basic_type->is_const = is_const;
                basic_type->is_volatile = is_volatile;
                return basic_type;
            }
        }

        if constexpr (std::is_pointer_v<Primary>)
        {
            Type* pointee = parse<std::remove_pointer_t<Primary>>(is_full_name);
            Pointer* pointer = new Pointer{};
            pointer->pointee = pointee;
            pointer->is_const = is_const;
            pointer->is_volatile = is_volatile;
            return pointer;
        }
        else if constexpr (std::is_reference_v<Primary>)
        {
            Type* referencee = parse<std::remove_reference_t<Primary>>(is_full_name);
            Reference* reference = new Reference{};
            reference->referencee = referencee;
            reference->is_lvalue_ref = std::is_lvalue_reference_v<Primary>;
            return reference;
        }
        else if constexpr (std::is_array_v<Primary>)
        {
            Type* element = parse<std::remove_extent_t<T>>(is_full_name);
            Array* array = new Array{};
            array->element = element;
            array->size = std::extent_v<Primary>;
            return array;
        }
        else if constexpr (std::is_member_pointer_v<Primary>)
        {
            Type* class_type = parse<class_type_of_pointer<Primary>>(is_full_name);
            Type* member_type = parse<member_type_of_pointer<Primary>>(is_full_name);
            Member* member = new Member{};
            member->class_type = class_type;
            member->member_type = member_type;
            member->is_const = is_const;
            member->is_volatile = is_volatile;
            return member;
        }
        else if constexpr (std::is_function_v<Primary>)
        {
            Type* return_type = parse<return_type_of<Primary>>(is_full_name);

            using Args = args_type_of<Primary>;
            constexpr std::size_t arity = fn_arity_v<Primary>;
            std::vector<Type*> parameters(arity);
            parse_function_params<Args>(std::make_index_sequence<arity>{}, parameters, is_full_name);

            Function* function = new Function{};
            function->return_type = return_type;
            function->parameters = std::move(parameters);
            function->setflag(is_const_fn_v<Primary>,
                              is_volatile_fn_v<Primary>,
                              is_lvalue_ref_fn_v<Primary>,
                              is_rvalue_ref_fn_v<Primary>,
                              is_noexcept_fn_v<Primary>,
                              is_c_variadic_fn_v<Primary>);
            return function;
        }
        else if constexpr (is_template_class_v<Primary>)
        {
            using Args = args_type_of<Primary>;
            constexpr auto size = template_arity_v<Primary>;
            std::vector<Type*> parameters(size);
            parse_template_params<Args>(std::make_index_sequence<size>{}, parameters, is_full_name);
            Template* template_type = new Template{};
            template_type->name = raw_name_of_template<Primary>();
            template_type->parameters = std::move(parameters);
            template_type->is_const = is_const;
            template_type->is_volatile = is_volatile;
            return template_type;
        }
        else
        {
            BasicType* basic_type = new BasicType{};
            if constexpr (is_lambda<Primary>())
            {
                basic_type->name = resolve_lambda<Primary>();
            }
            else
            {
                basic_type->name = raw_name_of<Primary>();
            }
            basic_type->is_const = is_const;
            basic_type->is_volatile = is_volatile;
            return basic_type;
        }
    }
} // namespace magic::details

namespace magic
{
    using details::parse;
    using details::Type;
    using details::TypeKind;
} // namespace magic
#endif