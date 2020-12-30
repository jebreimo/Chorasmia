//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>
#include <string>

/** @file
  * @brief Defines the exception thrown by Chorasmia functions.
  */

namespace Chorasmia
{
    /**
     * @brief The exception class used throughout Chorasmia.
     */
    class ChorasmiaException : public std::runtime_error
    {
    public:
        ChorasmiaException() noexcept
            : std::runtime_error("Unspecified error.")
        {}

        /**
         * @brief Passes @a message on to the base class.
         */
        explicit ChorasmiaException(const std::string& message) noexcept
            : std::runtime_error(message)
        {}

        explicit ChorasmiaException(const char* message) noexcept
            : std::runtime_error(message)
        {}
    };
}

#define _CHORASMIA_THROW_3(file, line, msg) \
    throw ::Chorasmia::ChorasmiaException(file ":" #line ": " msg)

#define _CHORASMIA_THROW_2(file, line, msg) \
    _CHORASMIA_THROW_3(file, line, msg)

#define CHORASMIA_THROW(msg) \
    _CHORASMIA_THROW_2(__FILE__, __LINE__, msg)
