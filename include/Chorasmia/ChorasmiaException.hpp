//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>

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
        using std::runtime_error::runtime_error;
    };
}

#define CHORASMIA_IMPL_THROW_3(file, line, msg) \
    throw ::Chorasmia::ChorasmiaException(file ":" #line ": " msg)

#define CHORASMIA_IMPL_THROW_2(file, line, msg) \
    CHORASMIA_IMPL_THROW_3(file, line, msg)

#define CHORASMIA_THROW(msg) \
    CHORASMIA_IMPL_THROW_2(__FILE__, __LINE__, msg)
