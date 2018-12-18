//
// Created by FyS on 26/05/17.
//

#include <spdlog/spdlog.h>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <tclap/ValueArg.h>
#include <tclap/CmdLine.h>
#include "Context.hh"

fys::cl::Context::Context(const fys::cl::Context &other) :
        _port(other._port), _gtwPort(other._gtwPort), _gtwIp(other._gtwIp), _asioThread(other._asioThread), _busIniFilePath(other._busIniFilePath),
        _queuesSize(other._queuesSize), _verbose(other._verbose)
{}

fys::cl::Context::Context(fys::cl::Context &&other) noexcept :
        _port(other._port), _gtwPort(other._gtwPort), _gtwIp(std::move(other._gtwIp)), _asioThread(other._asioThread),
        _busIniFilePath(std::move(other._busIniFilePath)), _queuesSize(other._queuesSize), _verbose(other._verbose)
{}

fys::cl::Context::Context(const int ac, const char *const *av) {
    try {
        TCLAP::CmdLine cli("Game of Fys World Client Game", ' ', "1.0");
        TCLAP::ValueArg<std::string> configPath("c", "config", "Path of config file", false, "/home/FyS/ClionProjects/FreeYourSoul_Client/Client/resource/config.ini", "string");
        TCLAP::ValueArg<std::string> map("m", "map", "Main map managed by the server", false, "/home/FyS/Project/FreeYourSouls_Client/Client/resource/tmx_maps/map.tmx", "string");
        TCLAP::ValueArg<ushort> changePort("p", "port", "Listening Port", false, 0, "integer");
        TCLAP::ValueArg<ushort> changeGtwPort("s", "gport", "Listening Port for Gateway", false, 0, "integer");
        TCLAP::ValueArg<ushort> changeGtwIp("g", "gip", "IP of Gateway", false, 0, "string");
        TCLAP::ValueArg<std::size_t> changeThread("t", "thread", "Thread Numbers for listening", false, 0, "integer");
        TCLAP::ValueArg<bool> verbose("v", "verbose", "Print logs on standard output", false, true, "boolean");

        cli.add(configPath);
        cli.add(changePort);
        cli.add(changeGtwPort);
        cli.add(changeGtwIp);
        cli.add(changeThread);
        cli.add(verbose);
        cli.add(map);
        cli.parse(ac, av);
        this->initializeFromIni(configPath.getValue());
        if (changePort.getValue() > 0)
            setPort(changePort.getValue());
        if (changeGtwPort.getValue() > 0)
            setGtwPort(changeGtwPort.getValue());
        if (changeThread.getValue() > 0)
            setAsioThread(changeThread.getValue());
        setVerbose(verbose.getValue());
    }
    catch (std::exception &e) {
        std::cerr << "Context not initialized " << e.what() << std::endl;
    }
}

void fys::cl::Context::initializeFromIni(const std::string &iniPath) {
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(iniPath, pt);

    setGtwPort(pt.get<ushort>(CL_INI_GTW_PORT));
    setGtwIp(std::move(pt.get<std::string>(CL_INI_GTW_IP)));
    setQueuesSize(pt.get<std::size_t>(CL_QUEUES_SIZE));
    setPositionId(pt.get<std::string>(CL_MAP_POSITIONID));
    setTmxFileMapName(pt.get<std::string>(CL_MAP_TMX));
}

ushort fys::cl::Context::getPort() const {
    return _port;
}

void fys::cl::Context::setPort(const ushort port) {
    Context::_port = port;
}

size_t fys::cl::Context::getAsioThread() const {
    return _asioThread;
}

void fys::cl::Context::setAsioThread(const size_t asioThread) {
    Context::_asioThread = asioThread;
}

std::size_t fys::cl::Context::getQueuesSize() const {
    return _queuesSize;
}

void fys::cl::Context::setQueuesSize(std::size_t _queuesSize) {
    Context::_queuesSize = _queuesSize;
}

void fys::cl::Context::logContext() {
    spdlog::get("c")->info("Current Context: [ port: {} , positionId: {}, asioThread: {}, mainMapFilePath: {} , gatewayIp: {} , gatewayPort: {} ]",
                           _port, _positionId, _asioThread, _tmxFileMapName, _gtwIp, _gtwPort);
}

bool fys::cl::Context::isVerbose() const {
    return _verbose;
}

void fys::cl::Context::setVerbose(bool _verbose) {
    Context::_verbose = _verbose;
}

ushort fys::cl::Context::getGtwPort() const {
    return _gtwPort;
}

void fys::cl::Context::setGtwPort(ushort gtwPort) {
    Context::_gtwPort = gtwPort;
}

void fys::cl::Context::setGtwIp(std::string &&gtwIp) noexcept {
    Context::_gtwIp = std::move(gtwIp);
}

const std::string &fys::cl::Context::getGtwIp() const {
    return _gtwIp;
}

const std::string &fys::cl::Context::getPositionId() const {
    return _positionId;
}

void fys::cl::Context::setPositionId(const std::string &positionId) {
    Context::_positionId = positionId;
}

const std::string &fys::cl::Context::getTmxFileMapName() const {
    return _tmxFileMapName;
}

void fys::cl::Context::setTmxFileMapName(std::string &&tmxFileMapName) {
    _tmxFileMapName = std::move(tmxFileMapName);
}
