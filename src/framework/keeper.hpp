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

#pragma once

#include <string>
#include <vector>

#include <glog/logging.h>

#include <pficommon/lang/function.h>
#include <pficommon/lang/bind.h>
#include <pficommon/network/mprpc.h>

#include "keeper_common.hpp"
#include "../common/mprpc/rpc_client.hpp"

#include "server_util.hpp"
#include <iostream>

namespace jubatus {
namespace framework {

class keeper : public keeper_common, pfi::network::mprpc::rpc_server {
 public:
  keeper(const keeper_argv& a);
  virtual ~keeper();
  int run();

  template <typename R>
  void register_random(const std::string& method_name) {
    pfi::lang::function<R(std::string)> f = pfi::lang::bind(&keeper::template random_proxy0<R>, this, method_name, pfi::lang::_1);
    add(method_name, f);
  }
  template <typename R, typename A0> //, typename A1, typename A2>
  void register_random(const std::string& method_name) {
    pfi::lang::function<R(std::string,A0)> f = pfi::lang::bind(&keeper::template random_proxy1<R,A0>, this, method_name, pfi::lang::_1, pfi::lang::_2);
    add(method_name, f);
  }
  template <typename R, typename A0, typename A1>//, typename A2>
  void register_random(const std::string& method_name) {
    pfi::lang::function<R(std::string,A0,A1)> f = pfi::lang::bind(&keeper::template random_proxy2<R,A0,A1>, this, method_name, pfi::lang::_1, pfi::lang::_2, pfi::lang::_3);
    add(method_name,f); 
  }
  template <typename R, typename A0, typename A1, typename A2>
  void register_random(const std::string& method_name) {
    pfi::lang::function<R(std::string,A0,A1,A2)> f = pfi::lang::bind(&keeper::template random_proxy3<R,A0,A1,A2>, this, method_name, pfi::lang::_1, pfi::lang::_2, pfi::lang::_3, pfi::lang::_4);
    add(method_name,f); 
  }
  
  template <typename R>
  void register_broadcast(std::string method_name,
                          pfi::lang::function<R(R,R)> agg){//pfi::lang::function<R(R,R)>& agg) {
    pfi::lang::function<R(std::string)> f =
      pfi::lang::bind(&keeper::template broadcast_proxy0<R>, this, method_name, pfi::lang::_1, 
                      agg);
    add(method_name, f);
  }
  template <typename R, typename A0>
  void register_broadcast(std::string method_name,
                          pfi::lang::function<R(R,R)> agg){//pfi::lang::function<R(R,R)>& agg) {
    pfi::lang::function<R(std::string, A0)> f =
      pfi::lang::bind(&keeper::template broadcast_proxy1<R, A0>, this, method_name, pfi::lang::_1, pfi::lang::_2,
                      agg);
    add(method_name, f);
  }


  template <int N, typename R>
  void register_cht(std::string method_name, pfi::lang::function<R(R,R)> agg) {
    pfi::lang::function<R(std::string, std::string)> f =
      pfi::lang::bind(&keeper::template cht_proxy0<N,R>, this, method_name, pfi::lang::_1, pfi::lang::_2, agg);
    add(method_name, f);
  }
  template <int N, typename R, typename A0>
  void register_cht(std::string method_name, pfi::lang::function<R(R,R)> agg) {
    pfi::lang::function<R(std::string, std::string, A0)> f =
      pfi::lang::bind(&keeper::template cht_proxy1<N,R,A0>, this, method_name, pfi::lang::_1, pfi::lang::_2, pfi::lang::_3, agg);
    add(method_name, f);
  }
  template <int N, typename R, typename A0, typename A1>
  void register_cht(std::string method_name, pfi::lang::function<R(R,R)> agg) {
    pfi::lang::function<R(std::string, std::string, A0, A1)> f =
      pfi::lang::bind(&keeper::template cht_proxy2<N,R, A0, A1>, this, method_name, pfi::lang::_1, pfi::lang::_2, pfi::lang::_3, pfi::lang::_4, agg);
    add(method_name, f);
  }
  
 private:
  template <typename R>
  R random_proxy0(const std::string& method_name, const std::string& name){
    //    {DLOG(INFO)<< __func__ << " " << method_name << " " << name;}
    std::vector<std::pair<std::string, int> > list;
    get_members_(name, list);

    const std::pair<std::string, int>& c = list[rng_(list.size())];
    try{
      pfi::network::mprpc::rpc_client cli(c.first, c.second, a_.timeout);
      return cli.call<R(std::string)>(method_name)(name);
    }catch(const std::exception& e){
      LOG(ERROR) << e.what() << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <typename R, typename A>
  R random_proxy1(const std::string& method_name, const std::string& name, const A& arg){
    //    {DLOG(INFO)<< __func__ << " " << method_name << " " << name;}
    std::vector<std::pair<std::string, int> > list;
    get_members_(name, list);

    const std::pair<std::string, int>& c = list[rng_(list.size())];
    try{
      pfi::network::mprpc::rpc_client cli(c.first, c.second, a_.timeout);
      return cli.call<R(std::string,A)>(method_name)(name, arg);
    }catch(const std::exception& e){
      LOG(ERROR) << e.what() << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <typename R, typename A0, typename A1>
  R random_proxy2(const std::string& method_name, const std::string& name, const A0& a0, const A1& a1){
    std::vector<std::pair<std::string, int> > list;
    get_members_(name, list);

    const std::pair<std::string, int>& c = list[rng_(list.size())];
    try{
      pfi::network::mprpc::rpc_client cli(c.first, c.second, a_.timeout);
      return cli.call<R(std::string,A0,A1)>(method_name)(name, a0, a1);
    }catch(const std::exception& e){
      LOG(ERROR) << e.what() << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <typename R, typename A0, typename A1, typename A2>
  R random_proxy3(const std::string& method_name, const std::string& name, const A0& a0, const A1& a1, const A2& a2){
    std::vector<std::pair<std::string, int> > list;

    get_members_(name, list);

    const std::pair<std::string, int>& c = list[rng_(list.size())];
    try{
      pfi::network::mprpc::rpc_client cli(c.first, c.second, a_.timeout);
      return cli.call<R(std::string,A0,A1,A2)>(method_name)(name, a0, a1, a2);
    }catch(const std::exception& e){
      LOG(ERROR) << e.what() << " from " << c.first << ":" << c.second;
      throw;
    }
  }

  template <typename R>
  R broadcast_proxy0(const std::string& method_name, const std::string& name,
                    pfi::lang::function<R(R,R)>& agg) {
    //    {DLOG(INFO)<< __func__ << " " << method_name << " " << name;}
    std::vector<std::pair<std::string, int> > list;
    get_members_(name, list);

    try{
      jubatus::common::mprpc::rpc_mclient c(list, a_.timeout);
      return *(c.call(method_name, name, agg));
    }catch(const std::exception& e){
      LOG(ERROR) << e.what(); // << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <typename R, typename A>
  R broadcast_proxy1(const std::string& method_name, const std::string& name, const A& arg,
                    pfi::lang::function<R(R,R)>& agg) {
    //    {DLOG(INFO)<< __func__ << " " << method_name << " " << name;}
    std::vector<std::pair<std::string, int> > list;
    get_members_(name, list);

    try{
      jubatus::common::mprpc::rpc_mclient c(list, a_.timeout);
      return *(c.call(method_name, name, arg, agg));
    }catch(const std::exception& e){
      LOG(ERROR) << e.what(); // << " from " << c.first << ":" << c.second;
      throw;
    }
  }


  template <int N, typename R>
  R cht_proxy0(const std::string& method_name, const std::string& name, const std::string& id,
              pfi::lang::function<R(R,R)>& agg) {
    std::vector<std::pair<std::string, int> > list;
    get_members_from_cht_(name, id, list, N);

    try{
      jubatus::common::mprpc::rpc_mclient c(list, a_.timeout);
      return *(c.call(method_name, name, id, agg));
    }catch(const std::exception& e){
      LOG(ERROR) << N << " " << e.what(); // << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <int N, typename R, typename A0>
  R cht_proxy1(const std::string& method_name, const std::string& name, const std::string& id, const A0& arg,
              pfi::lang::function<R(R,R)>& agg) {
    std::vector<std::pair<std::string, int> > list;
    get_members_from_cht_(name, id, list, N);

    try{
      jubatus::common::mprpc::rpc_mclient c(list, a_.timeout);
      return *(c.call(method_name, name, id, arg, agg));
    }catch(const std::exception& e){
      LOG(ERROR) << e.what(); // << " from " << c.first << ":" << c.second;
      throw;
    }
  }
  template <int N, typename R, typename A0, typename A1>
  R cht_proxy2(const std::string& method_name, const std::string& name, const std::string& id, const A0& a0, const A1& a1,
              pfi::lang::function<R(R,R)>& agg) {
    std::vector<std::pair<std::string, int> > list;
    get_members_from_cht_(name, id, list, N);

    try{
      jubatus::common::mprpc::rpc_mclient c(list, a_.timeout);
      return *(c.call(method_name, name, id, a0, a1, agg));
    }catch(const std::exception& e){
      LOG(ERROR) << e.what(); // << " from " << c.first << ":" << c.second;
      throw;
    }
  }

};

}
} //namespace jubatus
