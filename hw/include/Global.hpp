#pragma once
/*
* These are global definitions that lets just agree on.
* Fr, tho dont change these plz
*/

#include <string>
#include <xsi.h>

static const std::string TOP_RESET_NAME       = "rst_n";
static const std::string TOP_CLK_NAME         = "clk";
static const std::string SIMENGINE_LIB_NAME   = "libxv_simulator_kernel.so";
const s_xsi_vlog_logicval HIGH                = {0X00000001, 0X00000000};
const s_xsi_vlog_logicval LOW                 = {0X00000000, 0X00000000};
static const std::string WDB_FILE_NAME        = "test.wdb";

using SignalIntPair     = std::pair<std::string, int>;
using FloatValidIntfT   = std::tuple<std::pair<std::string, int>, std::pair<std::string, int>>;
using PortNumber        = int;
