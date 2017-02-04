# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from test/RobotState.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class RobotState(genpy.Message):
  _md5sum = "9ebf5c3bc78dc310a9067120721e6ec8"
  _type = "test/RobotState"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float64 vision_x
float64 vision_y
float64 vision_theta
float64 xhat
float64 yhat
float64 thetahat
float64 vx
float64 vy
float64 w
float64 xhat_future
float64 yhat_future
float64 thetahat_future
float64 predict_forward_seconds
bool correction
"""
  __slots__ = ['vision_x','vision_y','vision_theta','xhat','yhat','thetahat','vx','vy','w','xhat_future','yhat_future','thetahat_future','predict_forward_seconds','correction']
  _slot_types = ['float64','float64','float64','float64','float64','float64','float64','float64','float64','float64','float64','float64','float64','bool']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       vision_x,vision_y,vision_theta,xhat,yhat,thetahat,vx,vy,w,xhat_future,yhat_future,thetahat_future,predict_forward_seconds,correction

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(RobotState, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.vision_x is None:
        self.vision_x = 0.
      if self.vision_y is None:
        self.vision_y = 0.
      if self.vision_theta is None:
        self.vision_theta = 0.
      if self.xhat is None:
        self.xhat = 0.
      if self.yhat is None:
        self.yhat = 0.
      if self.thetahat is None:
        self.thetahat = 0.
      if self.vx is None:
        self.vx = 0.
      if self.vy is None:
        self.vy = 0.
      if self.w is None:
        self.w = 0.
      if self.xhat_future is None:
        self.xhat_future = 0.
      if self.yhat_future is None:
        self.yhat_future = 0.
      if self.thetahat_future is None:
        self.thetahat_future = 0.
      if self.predict_forward_seconds is None:
        self.predict_forward_seconds = 0.
      if self.correction is None:
        self.correction = False
    else:
      self.vision_x = 0.
      self.vision_y = 0.
      self.vision_theta = 0.
      self.xhat = 0.
      self.yhat = 0.
      self.thetahat = 0.
      self.vx = 0.
      self.vy = 0.
      self.w = 0.
      self.xhat_future = 0.
      self.yhat_future = 0.
      self.thetahat_future = 0.
      self.predict_forward_seconds = 0.
      self.correction = False

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_13dB().pack(_x.vision_x, _x.vision_y, _x.vision_theta, _x.xhat, _x.yhat, _x.thetahat, _x.vx, _x.vy, _x.w, _x.xhat_future, _x.yhat_future, _x.thetahat_future, _x.predict_forward_seconds, _x.correction))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 105
      (_x.vision_x, _x.vision_y, _x.vision_theta, _x.xhat, _x.yhat, _x.thetahat, _x.vx, _x.vy, _x.w, _x.xhat_future, _x.yhat_future, _x.thetahat_future, _x.predict_forward_seconds, _x.correction,) = _get_struct_13dB().unpack(str[start:end])
      self.correction = bool(self.correction)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_13dB().pack(_x.vision_x, _x.vision_y, _x.vision_theta, _x.xhat, _x.yhat, _x.thetahat, _x.vx, _x.vy, _x.w, _x.xhat_future, _x.yhat_future, _x.thetahat_future, _x.predict_forward_seconds, _x.correction))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 105
      (_x.vision_x, _x.vision_y, _x.vision_theta, _x.xhat, _x.yhat, _x.thetahat, _x.vx, _x.vy, _x.w, _x.xhat_future, _x.yhat_future, _x.thetahat_future, _x.predict_forward_seconds, _x.correction,) = _get_struct_13dB().unpack(str[start:end])
      self.correction = bool(self.correction)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_13dB = None
def _get_struct_13dB():
    global _struct_13dB
    if _struct_13dB is None:
        _struct_13dB = struct.Struct("<13dB")
    return _struct_13dB
