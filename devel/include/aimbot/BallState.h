// Generated by gencpp from file aimbot/BallState.msg
// DO NOT EDIT!


#ifndef AIMBOT_MESSAGE_BALLSTATE_H
#define AIMBOT_MESSAGE_BALLSTATE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace aimbot
{
template <class ContainerAllocator>
struct BallState_
{
  typedef BallState_<ContainerAllocator> Type;

  BallState_()
    : vision_x(0.0)
    , vision_y(0.0)
    , xhat(0.0)
    , yhat(0.0)
    , vx(0.0)
    , vy(0.0)
    , xhat_future(0.0)
    , yhat_future(0.0)
    , predict_forward_seconds(0.0)
    , correction(false)  {
    }
  BallState_(const ContainerAllocator& _alloc)
    : vision_x(0.0)
    , vision_y(0.0)
    , xhat(0.0)
    , yhat(0.0)
    , vx(0.0)
    , vy(0.0)
    , xhat_future(0.0)
    , yhat_future(0.0)
    , predict_forward_seconds(0.0)
    , correction(false)  {
  (void)_alloc;
    }



   typedef double _vision_x_type;
  _vision_x_type vision_x;

   typedef double _vision_y_type;
  _vision_y_type vision_y;

   typedef double _xhat_type;
  _xhat_type xhat;

   typedef double _yhat_type;
  _yhat_type yhat;

   typedef double _vx_type;
  _vx_type vx;

   typedef double _vy_type;
  _vy_type vy;

   typedef double _xhat_future_type;
  _xhat_future_type xhat_future;

   typedef double _yhat_future_type;
  _yhat_future_type yhat_future;

   typedef double _predict_forward_seconds_type;
  _predict_forward_seconds_type predict_forward_seconds;

   typedef uint8_t _correction_type;
  _correction_type correction;




  typedef boost::shared_ptr< ::aimbot::BallState_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::aimbot::BallState_<ContainerAllocator> const> ConstPtr;

}; // struct BallState_

typedef ::aimbot::BallState_<std::allocator<void> > BallState;

typedef boost::shared_ptr< ::aimbot::BallState > BallStatePtr;
typedef boost::shared_ptr< ::aimbot::BallState const> BallStateConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::aimbot::BallState_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::aimbot::BallState_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace aimbot

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'aimbot': ['/home/robot/catkin_ws/src/aimbot/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::aimbot::BallState_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::aimbot::BallState_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::aimbot::BallState_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::aimbot::BallState_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::aimbot::BallState_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::aimbot::BallState_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::aimbot::BallState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "16625b961fe41517c3be13a764c00cdd";
  }

  static const char* value(const ::aimbot::BallState_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x16625b961fe41517ULL;
  static const uint64_t static_value2 = 0xc3be13a764c00cddULL;
};

template<class ContainerAllocator>
struct DataType< ::aimbot::BallState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "aimbot/BallState";
  }

  static const char* value(const ::aimbot::BallState_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::aimbot::BallState_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 vision_x\n\
float64 vision_y\n\
float64 xhat\n\
float64 yhat\n\
float64 vx\n\
float64 vy\n\
float64 xhat_future\n\
float64 yhat_future\n\
float64 predict_forward_seconds\n\
bool correction\n\
";
  }

  static const char* value(const ::aimbot::BallState_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::aimbot::BallState_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.vision_x);
      stream.next(m.vision_y);
      stream.next(m.xhat);
      stream.next(m.yhat);
      stream.next(m.vx);
      stream.next(m.vy);
      stream.next(m.xhat_future);
      stream.next(m.yhat_future);
      stream.next(m.predict_forward_seconds);
      stream.next(m.correction);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct BallState_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::aimbot::BallState_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::aimbot::BallState_<ContainerAllocator>& v)
  {
    s << indent << "vision_x: ";
    Printer<double>::stream(s, indent + "  ", v.vision_x);
    s << indent << "vision_y: ";
    Printer<double>::stream(s, indent + "  ", v.vision_y);
    s << indent << "xhat: ";
    Printer<double>::stream(s, indent + "  ", v.xhat);
    s << indent << "yhat: ";
    Printer<double>::stream(s, indent + "  ", v.yhat);
    s << indent << "vx: ";
    Printer<double>::stream(s, indent + "  ", v.vx);
    s << indent << "vy: ";
    Printer<double>::stream(s, indent + "  ", v.vy);
    s << indent << "xhat_future: ";
    Printer<double>::stream(s, indent + "  ", v.xhat_future);
    s << indent << "yhat_future: ";
    Printer<double>::stream(s, indent + "  ", v.yhat_future);
    s << indent << "predict_forward_seconds: ";
    Printer<double>::stream(s, indent + "  ", v.predict_forward_seconds);
    s << indent << "correction: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.correction);
  }
};

} // namespace message_operations
} // namespace ros

#endif // AIMBOT_MESSAGE_BALLSTATE_H
