//
// Created by FyS on 04/03/18.
//

#include <TcpConnection.hh>
#include "ClusterManager.hh"

fys::network::ClusterManager::ClusterManager(uint size) : SessionManager(size) {
    setName("ServerClusterManager");
}

uint fys::network::ClusterManager::addConnectionInCluster(const std::string &clusterKey,
                                                          const fys::network::TcpConnection::ptr &newConnection) {
    uint indexInClusterSession = addConnection(newConnection);

    if (indexInClusterSession >= _clusterKey.size())
        _clusterKey.resize(_clusterKey.size() * 2);
    _clusterKey[indexInClusterSession] = clusterKey;
    return indexInClusterSession;
}
