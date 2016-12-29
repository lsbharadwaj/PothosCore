///
/// \file Archive/Macros.hpp
///
/// Serialization registration and declaration macros.
///
/// \copyright
/// Copyright (c) 2016 Josh Blum
/// SPDX-License-Identifier: BSL-1.0
///

#pragma once
#include <Pothos/Config.hpp>
#include <Pothos/Archive/ArchiveEntry.hpp>
#include <Pothos/Archive/Invoke.hpp>
#include <type_traits>

/*!
 * Register archival functions for a given class.
 * A unique name is required with the class because
 * the typeinfo name is not consistent across platforms.
 */
#define POTHOS_CLASS_EXPORT_GUID(T, id) \
    namespace Pothos { namespace Archive { \
        template <> struct ArchiveEntryContainer<T> { \
            static const ArchiveEntry &entry; \
        }; \
        const ArchiveEntry &ArchiveEntryContainer<T>::entry = \
            ArchiveEntryT<T>(id) ; \
    }}

//! Register archival functions using the stringified type as the ID
#define POTHOS_CLASS_EXPORT(T) POTHOS_CLASS_EXPORT_GUID(T, #T)

/*!
 * Declare a serialize() function that can dispatch to
 * an individually declared save and load function.
 * Call in the outside scope, no namespaces or functions.
 */
#define POTHOS_SERIALIZATION_SPLIT_FREE(T) \
    namespace Pothos { namespace serialization { \
        template <typename Archive> \
        void serialize(Archive &ar, T &t, const unsigned int ver) { \
            Pothos::serialization::invokeSplit(ar, t, ver); \
        } \
    }}

/*!
 * Declare a serialize() function that can dispatch to
 * individually declared save and load member functions.
 * Call in the public part of a struct or class declaration.
 */
#define POTHOS_SERIALIZATION_SPLIT_MEMBER() \
    template <typename Archive> \
    typename std::enable_if<std::is_same<typename Archive::isSave, std::true_type>::value>::type \
    serialize(Archive &ar, const unsigned int ver) { \
        this->save(ar, ver); \
    } \
    template <typename Archive> \
    typename std::enable_if<std::is_same<typename Archive::isSave, std::false_type>::value>::type \
    serialize(Archive &ar, const unsigned int ver) { \
        this->load(ar, ver); \
    }
