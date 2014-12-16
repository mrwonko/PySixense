#include <Python.h>
#include <structmember.h>
#include <sixense.h>
#include <stdexcept>
#include <functional>

//    Helper Stuff

class Defer
{
public:
  Defer( std::function< void() > deferred )
    : m_deferred( deferred )
  {}
  ~Defer()
  {
    m_deferred();
  }
private:
  std::function< void() > m_deferred;
};

//    General Wrapper Templates

template< int( *func )() >
static PyObject* wrap( PyObject* self, PyObject* arguments )
{
  return PyLong_FromLong( func() );
}

template< int( *func )( int ) >
static PyObject* wrap( PyObject* self, PyObject* arguments )
{
  int argument;
  if( !PyArg_ParseTuple( arguments, "i", &argument ) )
  {
    return nullptr;
  }
  return PyLong_FromLong( func( argument ) );
}

template< int( *func )( ) >
static PyObject* wrapAsBool( PyObject* self, PyObject* arguments )
{
  return PyBool_FromLong( func() );
}

template< int( *func )( int ) >
static PyObject* wrapAsBool( PyObject* self, PyObject* arguments )
{
  int argument;
  if( !PyArg_ParseTuple( arguments, "i", &argument ) )
  {
    return nullptr;
  }
  return PyBool_FromLong( func( argument ) );
}

template< int( *func )( int ) >
static PyObject* wrapFromBool( PyObject* self, PyObject* arguments )
{
  int argument;
  if( !PyArg_ParseTuple( arguments, "p", &argument ) )
  {
    return nullptr;
  }
  return PyLong_FromLong( func( argument ) );
}

namespace wrapped
{
  //    Binding for Constants

  struct Constants
  {
    PyObject_HEAD
    int success;
    int failure;
    int button1;
    int button2;
    int button3;
    int button4;
    int buttonBumper;
    int buttonJoystick;
    int buttonStart;
  };

  static PyMemberDef s_constantsMembers[] = {
    { "Success",        T_INT, offsetof( Constants, success ),        0, "Success Return Value" },
    { "Failure",        T_INT, offsetof( Constants, failure ),        0, "Failure Return Value" },
    { "ButtonJoystick", T_INT, offsetof( Constants, buttonJoystick ), 0, "Bitmask for Joystick Button" },
    { "ButtonBumper",   T_INT, offsetof( Constants, buttonBumper ),   0, "Bitmask for Bumper Button" },
    { "ButtonStart",    T_INT, offsetof( Constants, buttonStart ),    0, "Bitmask for Start Button" },
    { "Button1",        T_INT, offsetof( Constants, button1 ),        0, "Bitmask for Button 1" },
    { "Button2",        T_INT, offsetof( Constants, button2 ),        0, "Bitmask for Button 2" },
    { "Button3",        T_INT, offsetof( Constants, button3 ),        0, "Bitmask for Button 3" },
    { "Button4",        T_INT, offsetof( Constants, button4 ),        0, "Bitmask for Button 4" },
    { nullptr }
  };

  static PyTypeObject s_constantsType = {
    PyVarObject_HEAD_INIT( NULL, 0 )
    "PySixense.Constants", // tp_name
    sizeof( Constants ),   // tp_basicsize
    0,                     // tp_itemsize
    nullptr,               // tp_dealloc
    nullptr,               // tp_print
    nullptr,               // tp_getattr
    nullptr,               // tp_setattr
    nullptr,               // tp_reserved
    nullptr,               // tp_repr
    nullptr,               // tp_as_number
    nullptr,               // tp_as_sequence
    nullptr,               // tp_as_mapping
    nullptr,               // tp_hash
    nullptr,               // tp_call
    nullptr,               // tp_str
    nullptr,               // tp_getattro
    nullptr,               // tp_setattro
    nullptr,               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,    // tp_flags
    "Constants",           // tp_doc
    nullptr,               // tp_traverse
    nullptr,               // tp_clear
    nullptr,               // tp_richcompare
    0,                     // tp_weaklistoffset
    nullptr,               // tp_iter
    nullptr,               // tp_iternext
    nullptr,               // tp_methods
    s_constantsMembers     // tp_members
  };

  //    Binding for sixenseControllerData

  struct ControllerData
  {
    PyObject_HEAD
    sixenseControllerData data;
    PyObject* pos;
    PyObject* rotMat;
    PyObject* rotQuat;
  };

  static PyMemberDef s_controllerDataMembers[] = {
    { "pos",                   T_OBJECT_EX, offsetof( ControllerData, pos ),                                                             0, "xyz Position as list" },
    { "rot_mat",               T_OBJECT_EX, offsetof( ControllerData, rotMat ),                                                          0, "3x3 Rotation Matrix as list of lists" },
    { "rot_quat",              T_OBJECT_EX, offsetof( ControllerData, rotQuat ),                                                         0, "Rotation Quaternion as list" },
    { "joystick_x",            T_FLOAT,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, joystick_x ),            0, "X position of analog stick" },
    { "joystick_y",            T_FLOAT,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, joystick_y ),            0, "Y position of analog stick" },
    { "trigger",               T_FLOAT,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, trigger ),               0, "Trigger location" },
    { "buttons",               T_UINT,      offsetof( ControllerData, data ) + offsetof( sixenseControllerData, buttons ),               0, "Bitfield of pressed buttons" },
    { "sequence_number",       T_UBYTE,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, sequence_number ),       0, "Sequence number" },
    { "firmware_revision",     T_USHORT,    offsetof( ControllerData, data ) + offsetof( sixenseControllerData, firmware_revision ),     0, "Firmware version" },
    { "hardware_revision",     T_USHORT,    offsetof( ControllerData, data ) + offsetof( sixenseControllerData, hardware_revision ),     0, "Hardware version" },
    { "packet_type",           T_USHORT,    offsetof( ControllerData, data ) + offsetof( sixenseControllerData, packet_type ),           0, "Type, currently always 1" },
    { "magnetic_frequency",    T_USHORT,    offsetof( ControllerData, data ) + offsetof( sixenseControllerData, magnetic_frequency ),    0, "Unused" },
    { "enabled",               T_INT,       offsetof( ControllerData, data ) + offsetof( sixenseControllerData, enabled ),               0, "If the controller is connected" },
    { "controller_index",      T_INT,       offsetof( ControllerData, data ) + offsetof( sixenseControllerData, controller_index ),      0, "Hardware index of controller" },
    { "is_docked",             T_UBYTE,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, is_docked ),             0, "Whether the controller is in the dock" },
    { "which_hand",            T_UBYTE,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, which_hand ),            0, "0: unknown, 1: left, 2: right" },
    { "hemi_tracking_enabled", T_UBYTE,     offsetof( ControllerData, data ) + offsetof( sixenseControllerData, hemi_tracking_enabled ), 0, "Whether the controller has been docked or hemisphere tracking has been enabled some other way" },
    { nullptr }
  };

  static void ControllerData_dealloc( PyObject* self )
  {
    ControllerData* data = reinterpret_cast< ControllerData* >( self );
    Py_XDECREF( data->pos );
    Py_XDECREF( data->rotMat );
    Py_XDECREF( data->rotQuat );
    Py_TYPE( self )->tp_free( self );
  }

  static PyTypeObject s_controllerDataType = {
    PyVarObject_HEAD_INIT( NULL, 0 )
    "PySixense.ControllerData", // tp_name
    sizeof( ControllerData ), // tp_basicsize
    0,                        // tp_itemsize
    ControllerData_dealloc,   // tp_dealloc
    nullptr,                  // tp_print
    nullptr,                  // tp_getattr
    nullptr,                  // tp_setattr
    nullptr,                  // tp_reserved
    nullptr,                  // tp_repr
    nullptr,                  // tp_as_number
    nullptr,                  // tp_as_sequence
    nullptr,                  // tp_as_mapping
    nullptr,                  // tp_hash
    nullptr,                  // tp_call
    nullptr,                  // tp_str
    nullptr,                  // tp_getattro
    nullptr,                  // tp_setattro
    nullptr,                  // tp_as_buffer
    Py_TPFLAGS_DEFAULT,       // tp_flags
    "Controller State",       // tp_doc
    nullptr,                  // tp_traverse
    nullptr,                  // tp_clear
    nullptr,                  // tp_richcompare
    0,                        // tp_weaklistoffset
    nullptr,                  // tp_iter
    nullptr,                  // tp_iternext
    nullptr,                  // tp_methods
    s_controllerDataMembers   // tp_members
  };

  struct PythonAllocError : std::runtime_error
  {
    using std::runtime_error::runtime_error;
  };

  class List
  {
  public:
    // throws bad_alloc on failure
    List()
      : m_obj(PyList_New( 0 ) )
    {
      if( !m_obj ) throw PythonAllocError( "Failed to create Python List" );
    }
    ~List()
    {
      if( m_obj ) Py_DECREF( m_obj );
    }
    List( const List& ) = delete;
    List& operator=( const List& ) = delete;
    List( List&& rhs )
      : m_obj( rhs.m_obj )
    {
      rhs.m_obj = nullptr;
    }
    List& operator=( List&& rhs )
    {
      if( m_obj ) Py_DECREF( m_obj );
      m_obj = rhs.m_obj;
      rhs.m_obj = nullptr;
    }
    void push_back( PyObject* obj )
    {
      if( PyList_Append( m_obj, obj ) != 0 )
      {
        throw PythonAllocError( "Failed to push back!" );
      }
    }
    void push_back( float f )
    {
      PyObject* obj = PyFloat_FromDouble( static_cast< double >( f ) );
      Defer d( [ obj ]() { Py_DECREF( obj ); } );
      push_back( obj );
    }
    void push_back( List& l )
    {
      push_back( l.m_obj );
    }
    operator PyObject*()
    {
      Py_INCREF( m_obj );
      return m_obj;
    }
  private:
    PyObject* m_obj;
  };

  static PyObject* mkControllerData( const sixenseControllerData& data )
  {
    try
    {
      List pos;
      for( int i = 0; i < 3; ++i )
      {
        pos.push_back( data.pos[ i ] );
      }
      List rotQuat;
      for( int i = 0; i < 4; ++i )
      {
        rotQuat.push_back( data.rot_quat[ i ] );
      }
      List rotMat;
      for( int x = 0; x < 3; ++x )
      {
        List vec;
        for( int y = 0; y < 3; ++y )
        {
          vec.push_back( data.rot_mat[ x ][ y ] );
        }
        rotMat.push_back( vec );
      }

      PyObject* obj = s_controllerDataType.tp_alloc( &s_controllerDataType, 0 );
      if( !obj )
      {
        return nullptr;
      }
      ControllerData* wrapped = reinterpret_cast< ControllerData* >( obj );
      wrapped->data = data;
      wrapped->pos = pos;
      wrapped->rotMat = rotMat;
      wrapped->rotQuat = rotQuat;
      return obj;
    }
    catch( PythonAllocError& )
    {
      return nullptr;
    }
  }

  static PyObject* mkAllControllerData( const sixenseAllControllerData& allData )
  {
    List list;
    for( int i = 0; i < 4; ++i )
    {
      PyObject* obj = mkControllerData( allData.controllers[ i ] );
      if( !obj )
      {
        return nullptr;
      }
      Defer d( [ obj ]() { Py_DECREF( obj ); } );
      // may throw PythonAllocError
      try
      {
        list.push_back( obj );
      }
      catch( PythonAllocError& )
      {
        return nullptr;
      }
    }
    return list;
  }

  static PyObject* GetData( PyObject* self, PyObject* arguments )
  {
    int which, index_back;
    if( !PyArg_ParseTuple( arguments, "ii", &which, &index_back ) )
    {
      return nullptr;
    }
    sixenseControllerData data{};
    int result = sixenseGetData( which, index_back, &data );
    PyObject* obj = ( result == SIXENSE_SUCCESS ) ? mkControllerData( data ) : ( Py_INCREF( Py_None ), Py_None );
    return Py_BuildValue( "(i,N)", result, obj );
  }

  static PyObject* GetAllData( PyObject* self, PyObject* arguments )
  {
    int index_back;
    if( !PyArg_ParseTuple( arguments, "i", &index_back ) )
    {
      return nullptr;
    }
    sixenseAllControllerData allData;
    int result = sixenseGetAllData( index_back, &allData );
    return Py_BuildValue( "(i,N)", result, result == SIXENSE_SUCCESS ? mkAllControllerData( allData ) : ( Py_INCREF( Py_None ), Py_None ) );
  }

  static PyObject* GetNewestData( PyObject* self, PyObject* arguments )
  {
    int which;
    if( !PyArg_ParseTuple( arguments, "i", &which ) )
    {
      return nullptr;
    }
    sixenseControllerData data{};
    int result = sixenseGetNewestData( which, &data );
    PyObject* obj = ( result == SIXENSE_SUCCESS ) ? mkControllerData( data ) : ( Py_INCREF( Py_None ), Py_None );
    return Py_BuildValue( "(i,N)", result, obj );
  }

  static PyObject* GetAllNewestData( PyObject* self, PyObject* arguments )
  {
    sixenseAllControllerData allData;
    int result = sixenseGetAllNewestData( &allData );
    return Py_BuildValue( "(i,N)", result, result == SIXENSE_SUCCESS ? mkAllControllerData( allData ) : ( Py_INCREF( Py_None ), Py_None ) );
  }

  //    Special Cases for some functions

  static PyObject* SetHemisphereTrackingMode( PyObject* self, PyObject* arguments )
  {
    int which_controller;
    int state;
    if( !PyArg_ParseTuple( arguments, "ii", &which_controller, &state ) )
    {
      return nullptr;
    }
    return PyLong_FromLong( sixenseSetHemisphereTrackingMode( which_controller, state ) );
  }

  static PyObject* GetHemisphereTrackingMode( PyObject* self, PyObject* arguments )
  {
    int which_controller;
    if( !PyArg_ParseTuple( arguments, "i", &which_controller ) )
    {
      return nullptr;
    }
    int state;
    int success = sixenseGetHemisphereTrackingMode( which_controller, &state );
    return Py_BuildValue( "(i,i)", success, state );
  }

  static PyObject* GetHighPriorityBindingEnabled( PyObject* self, PyObject* arguments )
  {
    int on_or_off;
    int success = sixenseGetHighPriorityBindingEnabled( &on_or_off );
    return Py_BuildValue( "(i,N)", success, PyBool_FromLong( on_or_off ) );
  }

  static PyObject* TriggerVibration( PyObject* self, PyObject* arguments )
  {
    int controller_id;
    int duration_100ms;
    int pattern_id;
    if( !PyArg_ParseTuple( arguments, "iii", &controller_id, &duration_100ms, &pattern_id ) )
    {
      return nullptr;
    }
    return PyLong_FromLong( sixenseTriggerVibration( controller_id, duration_100ms, pattern_id ) );
  }

  static PyObject* GetFilterEnabled( PyObject* self, PyObject* arguments )
  {
    int on_or_off = 0;
    int success = sixenseGetFilterEnabled( &on_or_off );
    return Py_BuildValue( "(i,N)", success, PyBool_FromLong( on_or_off ) );
  }

  static PyObject* SetFilterParams( PyObject* self, PyObject* arguments )
  {
    float near_range;
    float near_val;
    float far_range;
    float far_val;
    if( !PyArg_ParseTuple( arguments, "ffff", &near_range, &near_val, &far_range, &far_val ) )
    {
      return nullptr;
    }
    return PyLong_FromLong( sixenseSetFilterParams( near_range, near_val, far_range, far_val ) );
  }

  static PyObject* GetFilterParams( PyObject* self, PyObject* arguments )
  {
    float near_range, near_val, far_range, far_val;
    int success = sixenseGetFilterParams( &near_range, &near_val, &far_range, &far_val );
    return Py_BuildValue( "(i,f,f,f,f)", success, near_range, near_val, far_range, far_val );
  }

  static PyObject* SetBaseColor( PyObject* self, PyObject* arguments )
  {
    unsigned char red, green, blue;
    if( !PyArg_ParseTuple( arguments, "BBB", &red, &green, &blue ) )
    {
      return nullptr;
    }
    return PyLong_FromLong( sixenseSetBaseColor( red, green, blue ) );
  }

  static PyObject* GetBaseColor( PyObject* self, PyObject* arguments )
  {
    unsigned char red, green, blue;
    int success = sixenseGetBaseColor( &red, &green, &blue );
    return Py_BuildValue( "(i,B,B,B)", success, red, green, blue );
  }
}

PyMODINIT_FUNC PyInit_PySixense()
{
  static PyMethodDef methods[] = {
      { "Init", wrap< sixenseInit >, METH_VARARGS, "PySixense.Init()\n\nInitializes the SDK. Must be successfully called before using any other function.\n\nReturns status." },
      { "Exit", wrap< sixenseExit >, METH_VARARGS, "PySixense.Exit()\n\nMust be called once you're done with the SDK if you called Init()\n\nReturns status." },
      { "GetMaxBases", wrap< sixenseGetMaxBases >, METH_VARARGS, "PySixense.GetMaxBases()\n\nReturns the maximum number of supported bases, currently 4." },
      { "SetActiveBase", wrap< sixenseSetActiveBase >, METH_VARARGS, "PySixense.SetActiveBase(base_num)\n\nSets the base of the given index active (i.e. all base-related functions calls will apply to this one)\n\nReturns status" },
      { "IsBaseConnected", wrapAsBool< sixenseIsBaseConnected >, METH_VARARGS, "PySixense.IsBaseConnected(base_num)\n\nReturns whether the base of the given index is currently connected to the PC. (False or True)" },
      { "GetMaxControllers", wrap< sixenseGetMaxControllers >, METH_VARARGS, "PySixense.GetMaxControllers()\n\nReturns the maximum number of controllers supported (per base?)" },
      { "IsControllerEnabled", wrapAsBool< sixenseIsControllerEnabled >, METH_VARARGS, "PySixense.IsControlledEnabled(controller_index)\n\nReturns whether the controller of the given index is enabled (False or True)" },
      { "GetNumActiveControllers", wrap< sixenseGetNumActiveControllers >, METH_VARARGS, "PySixense.GetNumActiveControllers()\n\nReturns the number of active controlles of the active base. Use IsControllerEnabled() to find out which ones are the active ones." },
      { "GetHistorySize", wrap< sixenseGetHistorySize >, METH_VARARGS, "PySixense.GetHistorySize()\n\nHow much previous ControllerData is available - usually 10." },
      { "GetData", wrapped::GetData, METH_VARARGS, "PySixense.GetData(controller_index, index_back)\n\nReturns status and a PySixense.ControllerData with the data from index_back polls ago (in range [0, PySixense.GetHistorySize() - 1]) for the given controller" },
      { "GetAllData", wrapped::GetAllData, METH_VARARGS, "PySixense.GetAllData(index_back)\n\nReturns status and a list containing 4 PySixense.ControllerData (one for each PySixense.GetMaxControllers()) with the data from index_back polls ago (in range [0, PySixense.GetHistorySize() - 1])" },
      { "GetNewestData", wrapped::GetNewestData, METH_VARARGS, "PySixense.GetNewestData(controller_index)\n\nReturns status and a PySixense.ControllerData with the newest data for the given controller" },
      { "GetAllNewestData", wrapped::GetAllNewestData, METH_VARARGS, "PySixense.GetAllNewestData()\n\nReturns status and a list containing 4 PySixense.ControllerData (one for each PySixense.GetMaxControllers()) with the newest data." },
      { "SetHemisphereTrackingMode", wrapped::SetHemisphereTrackingMode, METH_VARARGS, "sixenseSetHemisphereTrackingMode(which_controller, state)" },
      { "GetHemisphereTrackingMode", wrapped::GetHemisphereTrackingMode, METH_VARARGS, "sixenseGetHemisphereTrackingMode(int which_controller)" },
      { "AutoEnableHemisphereTracking", wrap< sixenseAutoEnableHemisphereTracking >, METH_VARARGS, "PySixense.AutoEnableHemisphereTracking(controller_id)\n\nDeprecated, refer to the Sixense SDK Reference if you really need it." },
      { "SetHighPriorityBindingEnabled", wrapFromBool< sixenseSetHighPriorityBindingEnabled >, METH_VARARGS, "PySixense.SetHighPriorityBindingEnabled(on_or_off)\n\nOnly for wireless devices. Refer to the Sixense SDK Reference if you need it." },
      { "GetHighPriorityBindingEnabled", wrapped::GetHighPriorityBindingEnabled, METH_VARARGS, "PySixense.GetHighPriorityBindingEnabled()\n\nOnly for wireless devices. Refer to the Sixense SDK Reference if you need it. Returns a 2-tuple." },
      { "TriggerVibration", wrapped::TriggerVibration, METH_VARARGS, "PySixense.TriggerVibration(controller_id, duration_100ms, pattern_id)\n\nOn controllers that support it (not the Razer Hydra), this triggers a vibration lasting duration_100ms deciseconds. pattern_id is currently ignored.\n\nReturns status." },
      { "SetFilterEnabled", wrapFromBool< sixenseSetFilterEnabled >, METH_VARARGS, "PySixense.SetFilterEnabled(on_or_off)\n\nEnables or disabled filtering. ( i.e.mapping movements to a certain range )\n\nReturns success." },
      { "GetFilterEnabled", wrapped::GetFilterEnabled, METH_VARARGS, "PySixense.GetFilterEnabled()\n\nReturns status and whether filtering is enabled (True or False)" },
      { "SetFilterParams", wrapped::SetFilterParams, METH_VARARGS, "PySixense.SetFilterParams(near_range, near_val, far_range, far_val)\n\nPlease refer to the Sixense SDK Reference" },
      { "GetFilterParams", wrapped::GetFilterParams, METH_VARARGS, "PySixense.GetFilterParams()\n\nPlease refer to the Sixense SDK Reference. Returns a 5-tuple." },
      { "SetBaseColor", wrapped::SetBaseColor, METH_VARARGS, "PySixense.SetBaseColor(red, green, blue)\n\nOnly available on the Wireless Devkits. Refer to the Sixense SDK Reference if you need it." },
      { "GetBaseColor", wrapped::GetBaseColor, METH_VARARGS, "PySixense.GetBaseColor()\n\nOnly available on the Wireless Devkits. Refer to the Sixense SDK Reference if you need it. Returns a 4-tuple" },
      { nullptr, nullptr, 0, nullptr } // end marker
  };

  static PyModuleDef moduleDef = {
    PyModuleDef_HEAD_INIT,
    "PySixense", // name
    nullptr, // documentation
    -1, // size of per-interpreter state; -1 since state kept in global variables
    methods // method table
  };

  if( PyType_Ready( &wrapped::s_controllerDataType ) < 0 )
  {
    return nullptr;
  }

  if( PyType_Ready( &wrapped::s_constantsType ) < 0 )
  {
    return NULL;
  }

  PyObject* module = PyModule_Create( &moduleDef );

  // Add Constants object
  {
    PyObject* obj = _PyObject_New( &wrapped::s_constantsType );
    wrapped::Constants& constants = *reinterpret_cast< wrapped::Constants* >( obj );
    constants.success = SIXENSE_SUCCESS;
    constants.failure = SIXENSE_FAILURE;
    constants.button1 = SIXENSE_BUTTON_1;
    constants.button2 = SIXENSE_BUTTON_2;
    constants.button3 = SIXENSE_BUTTON_3;
    constants.button4 = SIXENSE_BUTTON_4;
    constants.buttonBumper = SIXENSE_BUTTON_BUMPER;
    constants.buttonJoystick = SIXENSE_BUTTON_JOYSTICK;
    constants.buttonStart = SIXENSE_BUTTON_START;
    PyModule_AddObject( module, "Constants", obj );
  }

  return module;
}

