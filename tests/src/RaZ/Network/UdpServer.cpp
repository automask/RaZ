#include "RaZ/Network/UdpServer.hpp"

#include <catch2/catch_test_macros.hpp>

#include <thread>

TEST_CASE("UdpServer basic", "[network]") {
  Raz::UdpServer server;
  CHECK_FALSE(server.isRunning());

  CHECK_NOTHROW(server.stop()); // Stopping a non-running server isn't an error
  CHECK_FALSE(server.isRunning());

  CHECK_NOTHROW(server.start(1234));
  CHECK(server.isRunning());
  CHECK_NOTHROW(server.start(1234)); // Starting an already running server restarts it properly
  CHECK(server.isRunning());

  CHECK_NOTHROW(server.stop());
  CHECK_FALSE(server.isRunning());
  CHECK_NOTHROW(server.stop()); // Stopping an already stopped server does nothing
  CHECK_FALSE(server.isRunning());
}
