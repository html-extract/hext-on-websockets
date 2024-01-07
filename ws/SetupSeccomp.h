#pragma once

#include <stdexcept>


namespace ws {


class SetupSeccompError : public std::runtime_error
{ using std::runtime_error::runtime_error; };


void SetupSeccomp();


} // namespace ws

