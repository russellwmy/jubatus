// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "keeper.hpp"

#include <iostream>
#include <glog/logging.h>

#include "../common/membership.hpp"
#include "../common/exception.hpp"
#include "../common/util.hpp"
#include "server_util.hpp"

using namespace jubatus;
using namespace jubatus::framework;

keeper::keeper(const keeper_argv& a)
  : keeper_common(a),
    pfi::network::mprpc::rpc_server(a.timeout) 
{
}

keeper::~keeper(){
}

int keeper::run()
{
  try {
    jubatus::util::set_exit_on_term();
    jubatus::util::ignore_sigpipe();
    if (this->serv(a_.port, a_.threadnum)) {
      return 0;
    } else {
      LOG(ERROR) << "failed starting server: any process using port " << a_.port << "?";
      return -1;
    }
  } catch (const jubatus::exception::jubatus_exception& e) {
    std::cout << e.diagnostic_information(true) << std::endl;
    return -1;
  }
}

