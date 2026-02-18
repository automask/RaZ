#pragma once

#ifndef RAZ_HTTPCLIENT_HPP
#define RAZ_HTTPCLIENT_HPP

#include "RaZ/Network/TcpClient.hpp"

namespace Raz {

class HttpClient {
public:
  HttpClient() = default;
  explicit HttpClient(std::string host) { connect(std::move(host)); }

  /// Connects the client to an HTTP server.
  /// \param host Host to establish a connection with.
  void connect(std::string host);
  /// Sends a GET request to the server the client is connected to.
  /// \note This operation is blocking and returns only when all data has been sent or if an error occurred.
  /// \param resource Data to be sent.
  std::string get(std::string_view resource);
  /// Disconnects the client from the server.
  void disconnect() { m_tcpClient.disconnect(); }

private:
  std::string m_host;
  TcpClient m_tcpClient;
};

} // namespace Raz

#endif //RAZ_HTTPCLIENT_HPP
