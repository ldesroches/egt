//
// ssl/stream_base.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EGT_ASIO_SSL_STREAM_BASE_HPP
#define EGT_ASIO_SSL_STREAM_BASE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <egt/asio/detail/config.hpp>

#include <egt/asio/detail/push_options.hpp>

namespace egt {
namespace asio {
namespace ssl {

/// The stream_base class is used as a base for the egt::asio::ssl::stream
/// class template so that we have a common place to define various enums.
class stream_base
{
public:
  /// Different handshake types.
  enum handshake_type
  {
    /// Perform handshaking as a client.
    client,

    /// Perform handshaking as a server.
    server
  };

protected:
  /// Protected destructor to prevent deletion through this type.
  ~stream_base()
  {
  }
};

} // namespace ssl
} // namespace asio
} // namespace egt

#include <egt/asio/detail/pop_options.hpp>

#endif // EGT_ASIO_SSL_STREAM_BASE_HPP