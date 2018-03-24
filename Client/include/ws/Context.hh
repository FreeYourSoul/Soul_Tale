//
// Created by FyS on 26/05/17.
//

#ifndef FREESOULS_CONTEXT_HH
#define FREESOULS_CONTEXT_HH

#include <string>
#include <ostream>
#include "GameServerInstance.hh"

// -- Config File Keys
static constexpr char WS_INI_PORT[] = "server.port";
static constexpr char WS_INI_GTW_PORT[] = "gateway.port";
static constexpr char WS_INI_GTW_IP[] = "gateway.ip";
static constexpr char WS_INI_ASIO_THREADS[] = "server.asioThread";
static constexpr char WS_QUEUES_SIZE[] = "bus.queuesSize";
static constexpr char WS_MAP_POSITIONID[] = "map.positionId";
static constexpr char WS_MAP_TMX[] = "map.tmxPath";

namespace fys::ws {

    enum { BUS_QUEUES_SIZE = 1024 };

    class Context {

    public:
        ~Context() = default;
        Context(const int ac, const char *const *av);
        Context(const Context&);
        Context(Context&&) noexcept;

        void logContext();

        ushort getPort() const;
        std::size_t getQueuesSize() const;
        size_t getAsioThread() const;
        ushort getGtwPort() const;
        bool isVerbose() const;
        const std::string &getGtwIp() const;
        const std::string &getTmxFileMapName() const;

        void setPort(ushort port);
        void setAsioThread(size_t asioThread);
        void setVerbose(bool verbose);
        void setQueuesSize(std::size_t queuesSize);
        void setGtwPort(ushort gtwPort);
        void setGtwIp(std::string&& gtwIp) noexcept;
        void setTmxFileMapName(std::string &&tmxFileMapName);

        const std::string &getPositionId() const;

        void setPositionId(const std::string &_positionId);

    private:
        void initializeFromIni(const std::string &iniPath);

    private:
        ushort _port;
        ushort _gtwPort;
        std::string _gtwIp;
        std::size_t _asioThread;
        std::string _busIniFilePath;
        std::size_t _queuesSize;
        std::string _positionId;
        bool _verbose;

        std::string _tmxFileMapName;
        std::vector<ws::GameServerInstance> _gameServers;
    };

}


#endif //FREESOULS_CONTEXT_HH
