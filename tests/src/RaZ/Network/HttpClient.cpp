#include "RaZ/Network/HttpClient.hpp"
#include "RaZ/Network/TcpServer.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("HttpClient connection", "[network]") {
  Raz::HttpClient client;

  CHECK_NOTHROW(client.disconnect()); // Disconnecting a client when it's not connected isn't an error

  CHECK_THROWS(client.connect("doesntexist"));
  CHECK_THROWS(client.connect("localhost")); // No server available

  Raz::TcpServer server(80);

  CHECK_NOTHROW(client.connect("localhost")); // Local TCP server listening on port 80 (HTTP)
  CHECK_NOTHROW(client.disconnect());
}
