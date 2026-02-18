#include "RaZ/Network/HttpClient.hpp"
#include "RaZ/Utils/Logger.hpp"

#include <algorithm>
#include <charconv>
#include <ranges>

namespace Raz {

namespace {

struct HttpResponse {
  unsigned short statusCode {};
  std::size_t contentLength {};
  bool isChunked = false;
  std::string location;
};

HttpResponse parseResponse(std::string_view response) {
  HttpResponse httpResponse {};

  // The last 4 characters (\r\n\r\n) are ignored to avoid getting two final empty lines
  for (const auto line : std::views::split(response.substr(0, response.size() - 4), std::string_view("\r\n"))) {
    const std::string_view header(line.data(), line.size());

    if (header.starts_with("HTTP"))
      std::from_chars(header.data() + 9, header.data() + 12, httpResponse.statusCode);
    else if (header.starts_with("Content-Length"))
      std::from_chars(header.data() + 15, header.data() + header.size(), httpResponse.contentLength);
    else if (header.starts_with("Transfer-Encoding") && header.find("chunked") != std::string::npos)
      httpResponse.isChunked = true;
    else if (header.starts_with("Location"))
      httpResponse.location = header.substr(10);
  }

  return httpResponse;
}

} // namespace

void HttpClient::connect(std::string host) {
  m_host = std::move(host);
  m_tcpClient.connect(m_host, 80);
}

std::string HttpClient::get(std::string_view resource) {
  Logger::debug("[HttpClient] Sending GET '{}'...", resource);

  const std::string request = std::format("GET {} HTTP/1.1\r\nHost: {}\r\n\r\n", resource, m_host);
  m_tcpClient.send(request);

  const std::string headers = m_tcpClient.receiveUntil("\r\n\r\n", true);
  const auto [statusCode, contentLength, isChunked, location] = parseResponse(headers);

  if (statusCode == 301 || statusCode == 302 || statusCode == 303 || statusCode == 307 || statusCode == 308)
    return get(location); // TODO: redirections can happen indefinitely in some cases

  if (contentLength > 0)
    return m_tcpClient.receiveExactly(contentLength);

  if (isChunked) // TODO: properly handle chunked data
    return m_tcpClient.receiveUntil("\r\n\r\n");

  Logger::error("[HttpClient] Failed to get resource '{}': unsupported response method", resource);
  return {};
}

}
