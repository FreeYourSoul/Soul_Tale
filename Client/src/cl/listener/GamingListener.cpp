//
// Created by FyS on 18/03/18.
//

#include <spdlog/spdlog.h>
#include <google/protobuf/util/time_util.h>
#include <FySPlayerInteraction.pb.h>
#include <FySMessage.pb.h>
#include "GamingListener.hh"

using namespace google::protobuf::util;

fys::cl::buslistener::GamingListener::GamingListener(std::shared_ptr<fys::cl::Game> &ws) {
}

void fys::cl::buslistener::GamingListener::operator()(fys::mq::QueueContainer<pb::FySMessage> msg) {

}

