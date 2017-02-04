// Generated by gencpp from file mcthuggets/RobotState.msg
// DO NOT EDIT!


#ifndef MCTHUGGETS_MESSAGE_ROBOTSTATE_H
#define MCTHUGGETS_MESSAGE_ROBOTSTATE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace mcthuggets
{
template <class ContainerAllocator>
struct RobotState_
{
  typedef RobotState_<ContainerAllocator> Type;

  RobotState_()
    : vision_x(0.0)
    , vision_y(0.0)
    , vision_theta(0.0)
    , xhat(0.0)
    , yhat(0.0)
    , thetahat(0.0)
    , vx(0.0)
    , vy(0.0)
    , w(0.0)
    , xhat_future(0.0)
    , yhat_future(0.0)
    , thetahat_future(0.0)
    , predict_forward_seconds(0.0)
    , correction(false)  {
    }
  RobotState_(const ContainerAllocator& _alloc)
    : vision_x(0.0)
    , vision_y(0.0)
    , vision_theta(0.0)
    , xhat(0.0)
    , yhat(0.0)
    , thetahat(0.0)
    , vx(0.0)
    , vy(0.0)
    , w(0.0)
    , xhat_future(0.0)
    , yhat_future(0.0)
    , thetahat_future(0.0)
    , predict_forward_seconds(0.0)
    , correction(false)  {
  (void)_alloc;
    }



   typedef double _vision_x_type;
  _vision_x_type vision_x;

   typedef double _vision_y_type;
  _vision_y_type vision_y;

   typedef double _vision_theta_type;
  _vision_theta_type vision_theta;

   typedef double _xhat_type;
  _xhat_type xhat;

   typedef double _yhat_type;
  _yhat_type yhat;

   typedef double _thetahat_type;
  _thetahat_type thetahat;

   typedef double _vx_type;
  _vx_type vx;

   typedef double _vy_type;
  _vy_type vy;

   typedef double _w_type;
  _w_type w;

   typedef double _xhat_future_type;
  _xhat_future_type xhat_future;

   typedef double _yhat_future_type;
  _yhat_future_type yhat_future;

   typedef double _thetahat_future_type;
  _thetahat_future_type thetahat_future;

   typedef double _predict_forward_seconds_type;
  _predict_forward_seconds_type predict_forward_seconds;

   typedef uint8_t _correction_type;
  _correction_type correction;




  typedef boost::shared_ptr< ::mcthuggets::RobotState_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::mcthuggets::RobotState_<ContainerAllocator> const> ConstPtr;

}; // struct RobotState_

typedef ::mcthuggets::RobotState_<std::allocator<void> > RobotState;

typedef boost::shared_ptr< ::mcthuggets::RobotState > RobotStatePtr;
typedef boost::shared_ptr< ::mcthuggets::RobotState const> RobotStateConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::mcthuggets::RobotState_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::mcthuggets::RobotState_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace mcthuggets

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'mcthuggets': ['/home/robot/catkin_ws/src/mcthuggets/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::mcthuggets::RobotState_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::mcthuggets::RobotState_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mcthuggets::RobotState_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mcthuggets::RobotState_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mcthuggets::RobotState_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mcthuggets::RobotState_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::mcthuggets::RobotState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "9ebf5c3bc78dc310a9067120721e6ec8";
  }

  static const char* value(const ::mcthuggets::RobotState_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x9ebf5c3bc78dc310ULL;
  static const uint64_t static_value2 = 0xa9067120721e6ec8ULL;
};

template<class ContainerAllocator>
struct DataType< ::mcthuggets::RobotState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "mcthuggets/RobotState";
  }

  static const char* value(const ::mcthuggets::RobotState_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::mcthuggets::RobotState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 vision_x\n\
float64 vision_y\n\
float64 vision_theta\n\
float64 xhat\n\
float64 yhat\n\
float64 thetahat\n\
float64 vx\n\
float64 vy\n\
float64 w\n\
float64 xhat_future\n\
float64 yhat_future\n\
float64 thetahat_future\n\
float64 predict_forward_seconds\n\
bool correction\n\
";
  }

  static const char* value(const ::mcthuggets::RobotState_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::mcthuggets::RobotState_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.vision_x);
      stream.next(m.vision_y);
      stream.next(m.vision_theta);
      stream.next(m.xhat);
      stream.next(m.yhat);
      stream.next(m.thetahat);
      stream.next(m.vx);
      stream.next(m.vy);
      stream.next(m.w);
      stream.next(m.xhat_future);
      stream.next(m.yhat_future);
      stream.next(m.thetahat_future);
      stream.next(m.predict_forward_seconds);
      stream.next(m.correction);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct RobotState_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::mcthuggets::RobotState_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::mcthuggets::RobotState_<ContainerAllocator>& v)
  {
    s << indent << "vision_x: ";
    Printer<double>::stream(s, indent + "  ", v.vision_x);
    s << indent << "vision_y: ";
    Printer<double>::stream(s, indent + "  ", v.vision_y);
    s << indent << "vision_theta: ";
    Printer<double>::stream(s, indent + "  ", v.vision_theta);
    s << indent << "xhat: ";
    Printer<double>::stream(s, indent + "  ", v.xhat);
    s << indent << "yhat: ";
    Printer<double>::stream(s, indent + "  ", v.yhat);
    s << indent << "thetahat: ";
    Printer<double>::stream(s, indent + "  ", v.thetahat);
    s << indent << "vx: ";
    Printer<double>::stream(s, indent + "  ", v.vx);
    s << indent << "vy: ";
    Printer<double>::stream(s, indent + "  ", v.vy);
    s << indent << "w: ";
    Printer<double>::stream(s, indent + "  ", v.w);
    s << indent << "xhat_future: ";
    Printer<double>::stream(s, indent + "  ", v.xhat_future);
    s << indent << "yhat_future: ";
    Printer<double>::stream(s, indent + "  ", v.yhat_future);
    s << indent << "thetahat_future: ";
    Printer<double>::stream(s, indent + "  ", v.thetahat_future);
    s << indent << "predict_forward_seconds: ";
    Printer<double>::stream(s, indent + "  ", v.predict_forward_seconds);
    s << indent << "correction: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.correction);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MCTHUGGETS_MESSAGE_ROBOTSTATE_H
