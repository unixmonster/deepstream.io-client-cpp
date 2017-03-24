/*
 * Copyright 2017 deepstreamHub GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdint>

#include <algorithm>
#include <chrono>
#include <stdexcept>

#include <ostream>

#include "connection.hpp"
#include "message.hpp"
#include "state.hpp"
#include "use.hpp"
#include "websockets.hpp"
#include <deepstream/core/buffer.hpp>
#include <deepstream/core/client.hpp>
#include <deepstream/core/error_handler.hpp>

#include <cassert>
#include <include/deepstream.hpp>

namespace deepstream {

Client::Client(const std::string& uri, std::unique_ptr<ErrorHandler> p_eh, WSFactory* wsFactory)
    : p_impl_(Connection::make(uri, std::move(p_eh), nullptr).release())
    , event([this](const Message& message) -> bool {
        assert(p_impl_);
        return p_impl_->send_(message) == websockets::State::OPEN;
    })
    , presence([this](const Message& message) -> bool {
        assert(p_impl_);
        return p_impl_->send_(message) == websockets::State::OPEN;
    })
{
}

Client::Client(const std::string& uri, WSFactory* wsFactory)
    : Client(uri, std::unique_ptr<ErrorHandler>(new ErrorHandler()), wsFactory)
{
}

Client::~Client() { delete p_impl_; }

bool Client::login() { return p_impl_->login("{}"); }

bool Client::login(const std::string& auth, Buffer* p_user_data)
{
    return p_impl_->login(auth, p_user_data);
}

void Client::close() { return p_impl_->close(); }

State Client::getConnectionState()
{
    return p_impl_->getConnectionState();
}

void Client::process_messages()
{
    return p_impl_->process_messages(&event, &presence);
}
}
