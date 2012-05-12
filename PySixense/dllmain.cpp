#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
//#include <boost/python/class.hpp>
#include <sixense.h>

BOOST_PYTHON_MODULE(PySixense)
{
    using namespace boost::python;

	//class_<sixenseControllerData>("ControllerData", no_init) //no constructors please
	//	.def_readonly("joystick_x", &sixenseControllerData::joystick_x)
	//	;

	def("Init", &sixenseInit);
	def("Exit", &sixenseExit);
}

