//
// Created by FyS on 04/03/18.
//

#ifndef FREESOULS_WORLD_CLUSTERMANAGER_HH
#define FREESOULS_WORLD_CLUSTERMANAGER_HH

#include "SessionManager.hh"

namespace fys::network {

    class ClusterManager : public SessionManager {
    public:
        enum { CONNECTION_NUMBER = 10 };

    public:
        ClusterManager(uint size);

        uint addConnectionInCluster(const std::string& clusterKey, const std::shared_ptr<TcpConnection> &newConnection);

    private:
        std::vector<std::string> _clusterKey;
    };
}


#endif //FREESOULS_WORLD_CLUSTERMANAGER_HH
