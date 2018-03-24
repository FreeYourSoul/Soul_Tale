//
// Created by FyS on 30/05/17.
//

#include <spdlog/spdlog.h>
#include <FySMessage.pb.h>
#include <TokenGenerator.hh>
#include <TcpConnection.hh>
#include "SessionManager.hh"

fys::network::SessionManager::SessionManager(uint size) : _connections(size), _connectionsToken(size)
{}

uint fys::network::SessionManager::addConnection(const std::shared_ptr<TcpConnection> &newConnection) {
    uint i = 0;

    for (; i < _connections.size(); ++i) {
        if (!_connections.at(i)) {
            connectionHandle(newConnection, i);
            return i;
        }
    }
    _connections.resize(_connections.size() + 100, nullptr);
    _connectionsToken.resize(_connections.size() + 100);
    connectionHandle(newConnection, i);
    return i;
}

void fys::network::SessionManager::connectionHandle(const fys::network::TcpConnection::ptr &newConnection, uint i) {
    Token newToken = fys::utils::TokenGenerator::getInstance()->generateByte();

    this->_connections.at(i) = newConnection;
    this->_connectionsToken.at(i) = newToken;
    newConnection->setSessionIndex(i);
    newConnection->setCustomShutdownHandler([this, i, token = std::move(newToken)]() { this->disconnectUser(i, token); });
}

void fys::network::SessionManager::disconnectUser(uint idxSession, const fys::network::Token &token) {
    if (idxSession < _connectionsToken.size()) {
        if (std::equal(_connectionsToken.at(idxSession).begin(), _connectionsToken.at(idxSession).end(), token.begin())) {
            spdlog::get("c")->debug("Disconnect user {} from Session manager ({}) and token {}", idxSession, _name, std::string(token.begin(), token.end()));
            _connections.at(idxSession) = nullptr;
            _connectionsToken.at(idxSession) = {};
            return;
        }
    }
    spdlog::get("c")->error("Couldn't find the specified user's token to disconnect on sessionManager {}", _name);
}

const std::string fys::network::SessionManager::getIp(uint indexInSession) const noexcept {
    if (indexInSession < _connections.size())
        return _connections.at(indexInSession)->getIpAddress();
    return "";
}

const std::string fys::network::SessionManager::getConnectionToken(uint indexInSession) const noexcept {
    if (indexInSession < _connectionsToken.size())
        return std::string(_connectionsToken.at(indexInSession).begin(), _connectionsToken.at(indexInSession).end());
    return "";
}

void fys::network::SessionManager::sendResponse(uint indexInSession, fys::pb::FySResponseMessage &&message) const noexcept {
    if (indexInSession < _connections.size())
        _connections.at(indexInSession)->send(std::move(message));
}

std::pair<std::string, ushort>  fys::network::SessionManager::getConnectionData(uint indexInSession) const noexcept {
    return std::make_pair(_connections.at(indexInSession)->getIpAddress(), _connections.at(indexInSession)->getPort());
}

void fys::network::SessionManager::setName(std::string &&name) {
    _name = std::move(name);
}




