//
// Created by FyS on 05/03/18.
//

#ifndef FREESOULS_WORLD_PLAYERMANAGER_HH
#define FREESOULS_WORLD_PLAYERMANAGER_HH

#include <unordered_map>
#include "SessionManager.hh"

namespace fys::network {

    // forward declaration
    class TcpConnection;

    class PlayerManager : public SessionManager {

    public:
        enum { CONNECTION_NUMBER = 1000 };

    public:
        PlayerManager(uint size);

        /**
         * \brief Add an entry in the accepted ip/token for new player connection
         * \param ipIncoPlayer
         * \param tokenIncoPlayer
         */
        void addIncomingPlayer(const std::string& ipIncoPlayer, const Token& tokenIncoPlayer);
        /**
         * \brief Add a player connection (call the SessionManager #fys::network::SessionManager::addConnection()) if
         * the given ip is in the accepted incoming player ip (stored in #_incomingPlayer)
         * \param newConnection
         * \return
         */
        uint addPlayerConnection(const std::shared_ptr<TcpConnection> &newConnection);
        /**
         * \brief Check if the token corresponding to the given ip is correct and connect the user
         * \note Connect the user thanks to the given token. This token has to match the one given during the incoming
         * player registering phase (by call of #addIncomingPlayer(const std::string&, const Token&) ). If it
         * doesn't match, the player's session is disconnected because of failed authentication.
         * \param indexInSession used to find the ip of the user
         * \param token to check
         * \return true if the given token match the accepted one, return false otherwise
         */
        bool connectPlayerWithToken(uint indexInSession, const Token &token);

        /**
         * \brief Check authentication for the given indexInSession
         * \param indexInSession
         * \return
         */
        bool isAuthenticated(uint indexInSession, const std::string &token) const;

    private:
        std::unordered_map<std::string, Token> _incomingPlayer;

    };

}

#endif //FREESOULS_WORLD_PLAYERMANAGER_HH
