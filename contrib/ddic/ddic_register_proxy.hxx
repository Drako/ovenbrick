#pragma once

/*********************************************************************
 * Copyright © 2015 - 2016, Felix Bytow <felix.bytow@googlemail.com> *
 *                                                                   *
 * See the file COPYING for copying permissions.                     *
 *********************************************************************/

/**
 * \file ddic_register_proxy.hxx
 * \author Felix Bytow
 * \since 0.1.1
 * \brief Proxy class to create aliases for registered types.
 * 
 * This file contains the ddic::register_proxy class.
 * It is used for registering aliases to registered types inside a ddic::container.
 */

#ifndef DDIC_REGISTER_PROXY_HXX
#define DDIC_REGISTER_PROXY_HXX

namespace ddic
{
  struct abstract_factory;
  class container;

  /**
   * \author Felix Bytow
   * \brief is used to register aliases when registering a new type.
   * \since 0.1.1
   *
   * When registering a type with a container, the container returns a register_proxy.
   * The proxy can then be used to create aliases for the registered type.
   */
  class register_proxy
  {
  public:
    /**
     * \brief Takes a factory and a container.
     * \param[in] p Pointer to the factory for the registered type.
     * \param[out] c Container used to register aliases.
     */
    register_proxy(std::shared_ptr<abstract_factory> const & p, container & c);
    register_proxy(register_proxy && src); ///< Move constructor

    /**
     * \brief Registers an alias for the registered type.
     * \param[in] what The name of the alias.
     * \return The register_proxy itself.
     */
    register_proxy & as(std::string const & what);

    /**
     * \brief Registers an alias for the registered type.
     * \tparam Type The other type the registered one provides.
     * \return The register_proxy itself.
     */
    template <typename Type>
    register_proxy & as()
    {
      return as(typeid(Type).name());
    }

  private:
    std::shared_ptr<abstract_factory> p_; ///< A pointer to the factory for the registered type.
    container & c_; ///< The container used to register the type.
  };
}

#endif // DDIC_REGISTER_PROXY_HXX
