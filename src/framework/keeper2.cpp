// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "keeper2.hpp"

#include <iostream>
#include <glog/logging.h>

#include "../common/membership.hpp"
#include "../common/exception.hpp"
#include "../common/util.hpp"
#include "server_util.hpp"

using namespace jubatus;
using namespace jubatus::framework;

namespace jubatus { namespace framework {
__thread msgpack::rpc::session_pool *private_session_pool_ = NULL;
__thread keeper2::async_task_loop* keeper2::async_task_loop::private_async_task_loop_;

// NOTE: '__thread' is gcc-extension. We should re-implement with
//       pthread TLS?

}}

keeper2::keeper2(const keeper_argv& a)
  : keeper_common(a),
    jubatus::common::mprpc::rpc_server2() // FIMXE: set server timeout a.timeout
{
}

keeper2::~keeper2(){
}

int keeper2::run()
{
  try {
    { LOG(INFO) << "running in port=" << a_.port; }
    jubatus::util::set_exit_on_term();
    jubatus::util::ignore_sigpipe();

    this->instance.listen( "0.0.0.0", a_.port );
    this->instance.run( a_.threadnum );

    return 0; // never return
  } catch (const jubatus::exception::jubatus_exception& e) {
    std::cout << e.diagnostic_information(true) << std::endl;
    return -1;
  }
}
