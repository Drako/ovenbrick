#pragma once

/*********************************************************************
 * Copyright © 2015 - 2016, Felix Bytow <felix.bytow@googlemail.com> *
 *                                                                   *
 * See the file COPYING for copying permissions.                     *
 *********************************************************************/

/**
 * \file ddic_factory.hxx
 * \author Felix Bytow
 * \since 0.1.1
 * \brief Factory classes for the DI container.
 * 
 * This file contains all the factory classes used by ddic::container.
 * All factories implement the ddic::abstract_factory interface which provides a create method.
 * That method shall return a pointer to an object of a registered type.
 * The implementation depends on the factory and the given ddic::creation_policy.
 */

#ifndef DDIC_FACTORY_HXX
#define DDIC_FACTORY_HXX

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>

#include "ddic_creation_policy.hxx"

namespace ddic
{
  /**
   * \author Felix Bytow
   * \brief defines the common interface of all ddic factories.
   * \since 0.1.1
   *
   * This is the common interface of all factories used inside ddic.
   */
  struct abstract_factory
  {
    virtual ~abstract_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * This method is overloaded by specific factories.
     * It returns an instance according to the registered creation_policy.
     *
     * \return An instance of the registered type.
     **/
    virtual std::shared_ptr<void> create() = 0;
  };

  /**
   * \author Felix Bytow
   * \brief is a trivial factory for default constructible types.
   * \since 0.1.1
   *
   * This class is used when registering default constructible types with a container.
   *
   * \tparam Type The registered type.
   * \tparam Policy The creation policy.
   */
  template <typename Type, creation_policy Policy>
  class default_factory;

  template <typename Type>
  class default_factory<Type, creation_policy::always_new>
      : public abstract_factory
  {
    static_assert(std::is_default_constructible<Type>::value, "Type must be default constructible!");

  public:
    virtual ~default_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * This method simply creates a new instance of the registered type everytime it is called.
     **/
    virtual std::shared_ptr<void> create() override
    {
      return std::make_shared<Type>();
    }
  };

  template <typename Type>
  class default_factory<Type, creation_policy::always_same>
      : public abstract_factory
  {
    static_assert(std::is_default_constructible<Type>::value, "Type must be default constructible!");

  public:
    virtual ~default_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * When first called, this method creates a new instance of the registered type.
     * Subsequent calls will just return the existing instance.
     **/
    virtual std::shared_ptr<void> create() override
    {
      // lazy creation
      if (!instance_)
        instance_ = std::make_shared<Type>();
      return instance_;
    }

  private:
    std::shared_ptr<Type> instance_; ///< The internally stored instance.
  };

  /**
   * \author Felix Bytow
   * \brief is a trivial factory for copyable types.
   * \since 0.1.1
   *
   * This class is used when registering a copyable prototype instance.
   *
   * \tparam Type The registered type.
   * \tparam Policy The creation policy.
   */
  template <typename Type, creation_policy Policy>
  class prototype_factory;

  template <typename Type>
  class prototype_factory<Type, creation_policy::always_new>
      : public abstract_factory
  {
    static_assert(std::is_copy_constructible<Type>::value, "Type must be copy constructible!");

  public:
    /**
     * \brief takes a prototype instance and stores it for later use.
     * \param[in] proto The prototype instance.
     */
    prototype_factory(Type const & proto)
        : proto_(proto)
    {
    }

    virtual ~prototype_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * This method simply creates a new instance of the registered type by copying the stored prototype.
     **/
    virtual std::shared_ptr<void> create() override
    {
      return std::make_shared<Type>(proto_);
    }

  private:
    Type proto_; ///< The internally stored prototype.
  };

  template <typename Type>
  class prototype_factory<Type, creation_policy::always_same>
      : public abstract_factory
  {
    static_assert(std::is_copy_constructible<Type>::value, "Type must be copy constructible!");

  public:
    /**
     * \brief takes a prototype instance and creates a copy for later use.
     * \param[in] proto The prototype instance.
     */
    prototype_factory(Type const & proto)
        : instance_(new Type(proto))
    {
    }

    virtual ~prototype_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * This method returns a pointer to a copied prototype instance.
     **/
    virtual std::shared_ptr<void> create() override
    {
      return instance_;
    }

  private:
    std::shared_ptr<Type> instance_; ///< Copy of the original prototype.
  };

  /**
   * \author Felix Bytow
   * \brief is a factory for types with more complex construction.
   * \since 0.1.1
   *
   * This class is used when registering types with more complex construction.
   * To accomplish this the construction is handled something callable (a functor, a function pointer, a lambda).
   * For dependency resolution the functor shall use the provided container object.
   *
   * \tparam Container The container type.
   * \tparam Type The registered type.
   * \tparam Policy The creation policy.
   */
  template <typename Container, typename Type, creation_policy Policy>
  class functor_factory;

  template <typename Container, typename Type>
  class functor_factory<Container, Type, creation_policy::always_new>
      : public abstract_factory
  {
  public:
    /**
     * \brief takes a function object that can create objects of the registered type.
     * \param[in] fn A function object able to create objects of the registered type.
     * \param[in] c A DI container the function object can use to resolve dependencies.
     */
    functor_factory(std::function<Type* (Container &)> fn, Container & c)
        : fn_(fn)
        , c_(c)
    {
    }

    virtual ~functor_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * This method calls the creator function object and returns the pointer to the newly created object.
     **/
    virtual std::shared_ptr<void> create() override
    {
      return std::shared_ptr<Type>(fn_(c_));
    }

  private:
    std::function<Type* (Container &)> fn_; ///< The creator function.
    Container & c_; ///< The DI container for dependency resolution.
  };

  template <typename Container, typename Type>
  class functor_factory<Container, Type, creation_policy::always_same>
      : public abstract_factory
  {
  public:
    /**
     * \brief takes a function object that can create objects of the registered type.
     * \param[in] fn A function object able to create objects of the registered type.
     * \param[in] c A DI container the function object can use to resolve dependencies.
     */
    functor_factory(std::function<Type* (Container &)> fn, Container & c)
        : fn_(fn)
        , c_(c)
    {
    }

    virtual ~functor_factory() = default; ///< Default virtual destructor.

    /**
     * \brief creates a new instance of a registered type.
     *
     * When first called, this method calls the creator function object to create a new object of the registered type.
     * Subsequent calls simply return a pointer to that object.
     **/
    virtual std::shared_ptr<void> create() override
    {
      if (!instance_)
        instance_.reset(fn_(c_));
      return instance_;
    }

  private:
    std::function<Type* (Container &)> fn_; ///< The creator function.
    Container & c_; ///< The DI container for dependency resolution.
    std::shared_ptr<Type> instance_; ///< The internally stored instance.
  };
}

#endif // DDIC_FACTORY_HXX
