#pragma once

/*********************************************************************
 * Copyright © 2015 - 2016, Felix Bytow <felix.bytow@googlemail.com> *
 *                                                                   *
 * See the file COPYING for copying permissions.                     *
 *********************************************************************/

/**
 * \file ddic_container.hxx
 * \author Felix Bytow
 * \since 0.1.1
 * \brief The DI container.
 * 
 * This file contains the core component of DDIC, the ddic::container.
 */

#ifndef DDIC_CONTAINER_HXX
#define DDIC_CONTAINER_HXX

#include "ddic_factory.hxx"
#include "ddic_register_proxy.hxx"

#include <memory>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace ddic
{
  /**
   * \brief Little helper used for autowiring.
   *
   * This is a little helper used for autowiring.
   * Inside classes you can do:
   * \code
   *  // if your constructor were MyClass(std::shared_ptr<IFoo> && foo, std::shared_ptr<IBar> && bar);
   *  // the following would be the approprite autowire typedef.
   *  typedef ddic::inject<IFoo, IBar> autowire;
   * \endcode
   */
  template <typename... Types>
  struct inject {};

  /**
   * \author Felix Bytow
   * \brief is the actual DI container.
   * \since 0.1.1
   *
   * Objects of this class manage types registered with them
   * and help managing dependencies.
   */
  class container
  {
  public:
    container() {} ///< Default constructor.

    /**
     * \brief Constructor taking parent container.
     *
     * This constructor takes a parent container.
     * Parent containers act like scopes: If something cannot be resolved in the current container,
     * the request is propagated to its parent.
     *
     * \param[in] parent The parent container.
     */
    container(std::weak_ptr<container> && parent)
        : parent_(std::move(parent))
    {}

    // non copyable and non movable
    container(container const &) = delete;
    container(container &&) = delete;
    container & operator = (container const &) = delete;
    container & operator = (container &&) = delete;

    /**
     * \brief Register default constructible types.
     *
     * This function registers the given Type with the container
     * and assigns it to its name.
     *
     * \tparam[in] Type The type to be registered.
     * \tparam[in] Policy Declare how dependencies shall be instantiated.
     *
     * \remark The name might be mangled, so it is a good idea to also give it an alias.
     */
    template <typename Type, creation_policy Policy = creation_policy::always_new>
    auto register_type()
    -> typename std::enable_if<std::is_default_constructible<Type>::value, register_proxy>::type
    {
      auto factory = std::make_shared<default_factory<Type, Policy>>();
      factories_[typeid(Type).name()] = factory;
      return register_proxy(factory, *this);
    }

    /**
     * \brief Register a copyable prototype.
     *
     * This function registers the given prototype object with the container
     * and assigns it to its type name.
     *
     * \tparam[in] Type The type to be registered.
     * \tparam[in] Policy Declare how dependencies shall be instantiated.
     *
     * \param[in] proto A prototype object being copied to instantiate dependencies.
     *
     * \remark The name might be mangled, so it is a good idea to also give it an alias.
     */
    template <typename Type, creation_policy Policy = creation_policy::always_new>
    auto register_type(Type const & proto)
    -> typename std::enable_if<std::is_copy_constructible<Type>::value, register_proxy>::type
    {
      auto factory = std::make_shared<prototype_factory<Type, Policy>>(proto);
      factories_[typeid(Type).name()] = factory;
      return register_proxy(factory, *this);
    }

    /**
     * \brief Register a factory functor.
     *
     * This function registers the given functor as a factory function for
     * its return type. It is assigned to its return type's type name.
     *
     * \tparam[in] Type The type to be registered.
     * \tparam[in] Policy Declare how dependencies shall be instantiated.
     *
     * \param[in] fn A factory function returning an instance of Type.
     *
     * \remark The name might be mangled, so it is a good idea to also give it an alias.
     */
    template <typename Type, creation_policy Policy = creation_policy::always_new>
    register_proxy register_type(std::function<Type* (container &)> const & fn)
    {
      auto factory = std::make_shared<functor_factory<container, Type, Policy>>(fn, *this);
      factories_[typeid(Type).name()] = factory;
      return register_proxy(factory, *this);
    }

    /**
     * \brief Register a Type with injection friendly constructor.
     *
     * This function registers the given Type with the container
     * and assigns it to its name.
     * The constructor needs to take parameters like
     *  (std::shared_ptr<A> const &, std::shared_ptr<B> const &, std::shared_ptr<C> const &)
     *
     * \tparam[in] Type The type to be registered.
     * \tparam[in] Policy Declare how dependencies shall be instantiated.
     *
     * \param[in] inject A ddic::inject<A, B, C, ...>{}, that specifies the types to be injected.
     *
     * \remark The name might be mangled, so it is a good idea to also give it an alias.
     */
    template <typename Type, creation_policy Policy = creation_policy::always_new, typename... Types>
    register_proxy register_type(inject<Types...> const &)
    {
      return register_type<Type, Policy>([](container & c) {
        return new Type(c.resolve<Types>()...);
      });
    }

    /**
     * \brief Register a Type with injection friendly constructor and autowire typedef.
     *
     * This function registers the given Type with the container
     * and assigns it to its name.
     * The Type needs to contain a typedef like
     *  typedef ddic::inject<Foo, Bar, Baz> autowire;
     * The constructor then needs to take parameters like
     *  (std::shared_ptr<Foo> const &, std::shared_ptr<Bar> const &, std::shared_ptr<Baz> const &)
     *
     * \tparam[in] Type The type to be registered.
     * \tparam[in] Policy Declare how dependencies shall be instantiated.
     *
     * \remark The name might be mangled, so it is a good idea to also give it an alias.
     */
    template <typename Type, creation_policy Policy = creation_policy::always_new>
    register_proxy autowire_type()
    {
      return register_type<Type, Policy>(typename Type::autowire());
    }

    /**
     * \brief Resolve a dependency.
     *
     * This function checks if a factory with the provided name is known to the container
     * or any of its parents. If so, the dependency is instantiated according to the
     * creation policy used at registration.
     * If no name is provided, the implementation defined type name of Type is used.
     *
     * \tparam[in] Type The type of the dependency to be resolved.
     *
     * \param[in] name The name of the dependency to be resolved.
     *
     * \return If the dependency was resolved, an instance of the dependency, otherwise an empty std::shared_ptr.
     */
    template <typename Type>
    std::shared_ptr<Type> resolve(std::string const & name = typeid(Type).name())
    {
      // try resolving it here
      auto it = factories_.find(name);
      if (it != factories_.end())
        return std::static_pointer_cast<Type>(it->second->create());

      // maybe the parent can
      auto parent = parent_.lock();
      if (parent)
        return parent->resolve<Type>();

      // nope, cannot resolve the requested thing
      return std::shared_ptr<Type>();
    }

  private:
    std::weak_ptr<container> parent_; ///< Pointer to the parent container.
    std::unordered_map<std::string, std::shared_ptr<abstract_factory>> factories_; ///< All the registered factories.
    friend class register_proxy; ///< The register_proxy is used to created aliases, so it needs to access some internals.
  };
}

#endif // DDIC_CONTAINER_HXX
