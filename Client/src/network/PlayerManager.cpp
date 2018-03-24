//
// Created by FyS on 05/03/18.
//

#include <spdlog/spdlog.h>
#include <utility>
#include <TcpConnection.hh>
#include "PlayerManager.hh"

fys::network::PlayerManager::PlayerManager(uint size) : SessionManager(size), _incomingPlayer(size) {
    setName("Player Manager");
}

void fys::network::PlayerManager::addIncomingPlayer(const std::string &ipIncoPlayer, const Token& tokenIncoPlayer) {
    _incomingPlayer[ipIncoPlayer] = tokenIncoPlayer;
}

bool fys::network::PlayerManager::isAuthenticated(uint indexInSession, const std::string &token) const {
    return indexInSession < _connectionsToken.size() &&
           std::equal(token.begin(), token.end(), _connectionsToken.at(indexInSession).begin());
}

uint fys::network::PlayerManager::addPlayerConnection(const fys::network::TcpConnection::ptr &newConnection) {
    auto findIt = _incomingPlayer.find(newConnection->getIpAddress());

    if (findIt == _incomingPlayer.end()) {
        spdlog::get("c")->warn("The player isn't registered on the accepted ip list, ip trying to connect is {}", newConnection->getIpAddress());
        return std::numeric_limits<uint>::max();
    }
    return addConnection(newConnection);
}

bool fys::network::PlayerManager::connectPlayerWithToken(uint indexInSession, const Token &token) {
    const std::string ip = getIp(indexInSession);
    auto findIt = _incomingPlayer.find(ip);

    if (findIt != _incomingPlayer.end()) {
        if (std::equal(token.begin(), token.end(), _incomingPlayer.at(ip).begin())) {
            _incomingPlayer.erase(findIt);
            return true;
        }
        disconnectUser(indexInSession, _incomingPlayer.at(ip));
        spdlog::get("c")->warn("The given token is wrong for index {} ip {}", indexInSession, ip);
    }
    spdlog::get("c")->error("The given ip {} is not registered as accepted ip, "
                                    "this should have already been checked!", ip);
    return false;
}


