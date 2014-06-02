#ifndef OPENMESH_PYTHON_PROPERTYMANAGER_HH
#define OPENMESH_PYTHON_PROPERTYMANAGER_HH

#include "Python/Bindings.hh"
#include "OpenMesh/Core/Utils/PropertyManager.hh"

namespace OpenMesh {
namespace Python {

/**
 * Wrapper for property managers.
 *
 * This class template is used to wrap property managers for %Python. It
 * inherits from it's first template parameter (a property manager) and
 * implements functions that are required by %Python but not provided by
 * property manager types.
 *
 * @tparam Manager A property manager type.
 * @tparam IndexHandle The appropriate index handle type.
 */
template <class Manager, class IndexHandle>
class PropertyManagerWrapperT : public Manager {
	public:

		/**
		 * Constructor
		 */
		PropertyManagerWrapperT() : Manager() { }

		/**
		 * Constructor
		 */
		PropertyManagerWrapperT(PolyConnectivity &_mesh, const char *_propname, bool _existing = false) :
			Manager(_mesh, _propname, _existing) {
		}

		/**
		 * Thin wrapper for Manager::retain.
		 *
		 * This wrapper function is required because Manager::retain has default
		 * arguments and therefore cannot be exposed directly.
		 */
		void retain_void() {
			Manager::retain();
		}

		/**
		 * Thin wrapper for Manager::retain.
		 *
		 * This wrapper function is required because Manager::retain has default
		 * arguments and therefore cannot be exposed directly.
		 */
		void retain_bool(bool _retain) {
			Manager::retain(_retain);
		}

		/**
		 * Implementation of %Python's \_\_getitem\_\_ magic method.
		 *
		 * @param _handle A handle of the appropriate handle type.
		 * @return The requested property value.
		 */
		object getitem(IndexHandle _handle) const {
			return (*this)[_handle];
		}

		/**
		 * Implementation of %Python's \_\_setitem\_\_ magic method.
		 *
		 * @param _handle A handle of the appropriate handle type.
		 * @param _value The property value to be set.
		 */
		void setitem(IndexHandle _handle, object _value) {
			(*this)[_handle] = _value;
		}

		/**
		 * Conveniently set the property value for an entire range of mesh items
		 * using a %Python iterator.
		 *
		 * @tparam Iterator A %Python iterator.
		 *
		 * @param _it An iterator that iterates over the items in the range.
		 * @param _value The value the range will be set to.
		 */
		template <class Iterator>
		void set_range(Iterator _it, object _value) {
			try {
				while (true) {
					(*this)[_it.next()] = _value;
				}
			}
			catch (error_already_set exception) {
				// This is expected behavior
				PyErr_Clear();
			}
		}
};

/**
 * Expose a property manager type to %Python.
 *
 * This function template is used to expose property managers to %Python. The
 * template parameters are used to instantiate the appropriate property manager
 * type.
 *
 * @tparam PropHandle A property handle type (e.g. %VPropHandle\<object\>).
 * @tparam IndexHandle The appropriate index handle type (e.g. %VertexHandle for
 * %VPropHandle\<object\>).
 *
 * @param _name The name of the property manager type to be exposed.
 *
 * @note Property managers are wrapped by PropertyManagerWrapperT before they
 * are exposed to %Python, i.e. they are not exposed directly.
 */
template <class PropHandle, class IndexHandle, class Iterator>
void expose_property_manager(const char *_name) {
	// Convenience typedefs
	typedef PropertyManager<PropHandle, PolyConnectivity> PropertyManager;
	typedef PropertyManagerWrapperT<PropertyManager, IndexHandle> PropertyManagerWrapper;

	// Member function pointer
	void (PropertyManagerWrapper::*set_range)(Iterator, object) = &PropertyManagerWrapper::set_range;

	// Expose PropertyManager type
	class_<PropertyManagerWrapper, boost::noncopyable>(_name)
		.def(init<PolyConnectivity&, const char *, optional<bool> >())

		.def("swap", &PropertyManagerWrapper::swap)
		.def("is_valid", &PropertyManagerWrapper::isValid)

		.def("__bool__", &PropertyManagerWrapper::operator bool)
		.def("__nonzero__", &PropertyManagerWrapper::operator bool)

		.def("get_raw_property", &PropertyManagerWrapper::getRawProperty, return_value_policy<copy_const_reference>())
		.def("get_name", &PropertyManagerWrapper::getName, return_value_policy<copy_const_reference>())
		.def("get_mesh", &PropertyManagerWrapper::getMesh, return_value_policy<reference_existing_object>())

		.def("retain", &PropertyManagerWrapper::retain_void)
		.def("retain", &PropertyManagerWrapper::retain_bool)

		.def("__getitem__", &PropertyManagerWrapper::getitem)
		.def("__setitem__", &PropertyManagerWrapper::setitem)

		.def("set_range", set_range)

		.def("property_exists", &PropertyManagerWrapper::propertyExists)
		.staticmethod("property_exists")

		.def("create_if_not_exists", &PropertyManagerWrapper::createIfNotExists)
		.staticmethod("create_if_not_exists")
		;
}

} // namespace OpenMesh
} // namespace Python

#endif
