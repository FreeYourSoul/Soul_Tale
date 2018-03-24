//
// Created by FyS on 30/05/17.
//

#ifndef FREESOULS_CONNECTIONMANAGER_HH
#define FREESOULS_CONNECTIONMANAGER_HH

#include <zconf.h>
#include <vector>
#include <memory>

// forward declarations
namespace fys {
    namespace pb {
        class FySResponseMessage;
    }
    namespace network {
        class TcpConnection;
    }
}

namespace fys::network {

    using Token = std::vector<char>;

    class SessionManager {

    public:
        using uptr = std::unique_ptr<SessionManager>;
        using ptr = std::shared_ptr<SessionManager>;
        using wptr = std::weak_ptr<SessionManager>;

        explicit SessionManager(uint size);

        const std::string getConnectionToken(uint indexInSession) const noexcept;
        const std::string getIp(uint indexInSession) const noexcept;
        void setName(std::string &&name);
        std::pair<std::string, ushort> getConnectionData(uint indexInSession) const noexcept;
        uint addConnection(const std::shared_ptr<TcpConnection> &newConnection);
        void disconnectUser(uint idxSession, const fys::network::Token &token);

        void sendResponse(uint i, pb::FySResponseMessage &&message) const noexcept;

    private:
        inline void connectionHandle(const std::shared_ptr<TcpConnection> &newConnection, uint i);

    protected:
        std::string _name;
        std::vector<std::shared_ptr<TcpConnection>> _connections;
        std::vector<Token> _connectionsToken;
    };

}


#endif //FREESOULS_CONNECTIONMANAGER_HH
