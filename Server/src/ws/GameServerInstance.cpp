//
// Created by FyS on 28/08/17.
//

#include "GameServerInstance.hh"

fys::ws::GameServerInstance::GameServerInstance(fys::ws::GameServerInstance &&other) noexcept :
_ip(std::move(other._ip)), _port(other._port)
{}

fys::ws::GameServerInstance &fys::ws::GameServerInstance::operator=(fys::ws::GameServerInstance other) {
    std::swap(_ip, other._ip);
    other._port = other._port;
    return *this;
}

const std::string &fys::ws::GameServerInstance::getIp() const {
    return _ip;
}

void fys::ws::GameServerInstance::setIp(const std::string &ip) {
    GameServerInstance::_ip = ip;
}

ushort fys::ws::GameServerInstance::getPort() const {
    return _port;
}

void fys::ws::GameServerInstance::setPort(const ushort port) {
    GameServerInstance::_port = port;
}
