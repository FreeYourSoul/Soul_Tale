//
// Created by FyS on 31/08/17.
//

#include <spdlog/spdlog.h>
#include <FySAuthenticationLoginMessage.pb.h>
#include <FySAuthenticationResponse.pb.h>
#include <FySMessage.pb.h>
#include <WorldServer.hh>
#include "listener/Authenticator.hh"

fys::ws::buslistener::Authenticator::Authenticator(WorldServer::ptr& gtw) : _ws(gtw)
{}

void fys::ws::buslistener::Authenticator::operator()(mq::QueueContainer<pb::FySMessage> msg) {
    pb::LoginMessage authMessage;

    msg.getContained().content().UnpackTo(&authMessage);
    if (pb::LoginMessage_Type_IsValid(authMessage.typemessage())) {
        switch (authMessage.typemessage()) {

            case pb::LoginMessage_Type_NotifyNewPlayer :
                notifyPlayerIncoming(msg.getIndexSession(), std::move(authMessage));
                break;

            case pb::LoginMessage_Type_NotifyNewServer :
                notifyServer(std::move(authMessage));
                break;

            case pb::LoginMessage_Type_LoginPlayerOnGame:
                authPlayer(msg.getIndexSession(), std::move(authMessage));
                break;

            default:
                break;
        }
    }
}

void fys::ws::buslistener::Authenticator::notifyServer(fys::pb::LoginMessage &&loginMessage) {
    pb::NotifyServerIncoming notif;

    loginMessage.content().UnpackTo(&notif);
    _ws->connectAndAddWorldServerInCluster(notif.positionid(), notif.token(), notif.ip(), notif.port());
}

void fys::ws::buslistener::Authenticator::notifyPlayerIncoming(uint indexSession, fys::pb::LoginMessage &&loginMsg) {
    pb::NotifyPlayerIncoming notif;
    loginMsg.content().UnpackTo(&notif);
    network::Token token(notif.token().begin(), notif.token().end());

    _ws->getGamerConnections().addIncomingPlayer(notif.ip(), token);
}

void fys::ws::buslistener::Authenticator::authPlayer(uint indexSession, fys::pb::LoginMessage &&loginMessage) {
    pb::LogingPlayerOnGame loginPlayerOnGame;
    const std::string &actualToken = _ws->getGamerConnections().getConnectionToken(indexSession);
    const std::string &token = loginPlayerOnGame.tokengameserver();

    loginMessage.content().UnpackTo(&loginPlayerOnGame);
    if (std::equal(token.begin(), token.end(), actualToken.begin())) {
        _ws->getGamerConnections().connectPlayerWithToken(indexSession, {token.begin(), token.end()});
        spdlog::get("c")->info("A new player ({} at index {}) connected on server", loginMessage.user(), indexSession);
    }
    else {
        spdlog::get("c")->error("Mismatch has been found for player({}) {}, token is {} and should be {}",
                                indexSession, loginMessage.user(), token, actualToken);
    }
}

