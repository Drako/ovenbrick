/*********************************************************************
 * Copyright © 2015 - 2016, Felix Bytow <felix.bytow@googlemail.com> *
 *                                                                   *
 * See the file COPYING for copying permissions.                     *
 *********************************************************************/

#include "ddic_container.hxx"

namespace ddic
{
  register_proxy::register_proxy(std::shared_ptr<abstract_factory> const & p, container & c)
      : p_(p)
      , c_(c)
  {}

  register_proxy::register_proxy(register_proxy && src)
      : p_(std::move(src.p_))
      , c_(src.c_)
  {}

  register_proxy & register_proxy::as(std::string const & what)
  {
    c_.factories_[what] = p_;
    return *this;
  }
}
