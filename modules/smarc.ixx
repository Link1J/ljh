
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_SMARC 1
#define LJH_MODULE_SMARC_EXPORT export
export module ljh.smarc;
extern "C++"
{
#include "ljh/smarc/fwd.hpp"
#include "ljh/smarc/control_block.hpp"
#include "ljh/smarc/list.hpp"
#include "ljh/smarc/node.hpp"
#include "ljh/smarc/ptr.hpp"
#include "ljh/smarc/ref.hpp"
}