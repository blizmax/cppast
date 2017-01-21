// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_CPP_EXPRESSION_HPP_INCLUDED
#define CPPAST_CPP_EXPRESSION_HPP_INCLUDED

#include <memory>

#include <cppast/cpp_type.hpp>

namespace cppast
{
    /// The kind of a [cppast::cpp_expression]().
    enum class cpp_expression_kind
    {
        literal,

        unexposed,
    };

    /// Base class for all C++ expressions.
    class cpp_expression
    {
    public:
        cpp_expression(const cpp_expression&) = delete;
        cpp_expression& operator=(const cpp_expression&) = delete;

        virtual ~cpp_expression() noexcept = default;

        /// \returns The [cppast::cpp_expression_kind]().
        cpp_expression_kind kind() const noexcept
        {
            return do_get_kind();
        }

        /// \returns The type of the expression.
        const cpp_type& type() const noexcept
        {
            return *type_;
        }

    protected:
        /// \effects Creates it given the type.
        /// \requires The type must not be `nullptr`.
        cpp_expression(std::unique_ptr<cpp_type> type) : type_(std::move(type))
        {
            DEBUG_ASSERT(type_ != nullptr, detail::precondition_error_handler{});
        }

    private:
        /// \returns The [cppast::cpp_expression_kind]().
        virtual cpp_expression_kind do_get_kind() const noexcept = 0;

        std::unique_ptr<cpp_type> type_;
    };

    /// An unexposed [cppast::cpp_expression]().
    ///
    /// There is no further information than a string available.
    class cpp_unexposed_expression final : cpp_expression
    {
    public:
        /// \returns A newly created unexposed expression.
        static std::unique_ptr<cpp_unexposed_expression> build(std::unique_ptr<cpp_type> type,
                                                               std::string               str)
        {
            return std::unique_ptr<cpp_unexposed_expression>(
                new cpp_unexposed_expression(std::move(type), std::move(str)));
        }

        /// \returns The expression as a string.
        const std::string& expression() const noexcept
        {
            return str_;
        }

    private:
        cpp_unexposed_expression(std::unique_ptr<cpp_type> type, std::string str)
        : cpp_expression(std::move(type)), str_(std::move(str))
        {
        }

        cpp_expression_kind do_get_kind() const noexcept override
        {
            return cpp_expression_kind::unexposed;
        }

        std::string str_;
    };

    /// A [cppast::cpp_expression]() that is a literal.
    class cpp_literal_expression final : cpp_expression
    {
    public:
        /// \returns A newly created literal expression.
        static std::unique_ptr<cpp_literal_expression> build(std::unique_ptr<cpp_type> type,
                                                             std::string               value)
        {
            return std::unique_ptr<cpp_literal_expression>(
                new cpp_literal_expression(std::move(type), std::move(value)));
        }

        /// \returns The value of the literal, as string.
        const std::string& value() const noexcept
        {
            return value_;
        }

    private:
        cpp_literal_expression(std::unique_ptr<cpp_type> type, std::string value)
        : cpp_expression(std::move(type)), value_(std::move(value))
        {
        }

        cpp_expression_kind do_get_kind() const noexcept override
        {
            return cpp_expression_kind::literal;
        }

        std::string value_;
    };
} // namespace cppast

#endif // CPPAST_CPP_EXPRESSION_HPP_INCLUDED
