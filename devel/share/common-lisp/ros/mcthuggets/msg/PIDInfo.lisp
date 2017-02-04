; Auto-generated. Do not edit!


(cl:in-package mcthuggets-msg)


;//! \htmlinclude PIDInfo.msg.html

(cl:defclass <PIDInfo> (roslisp-msg-protocol:ros-message)
  ((error
    :reader error
    :initarg :error
    :type geometry_msgs-msg:Pose2D
    :initform (cl:make-instance 'geometry_msgs-msg:Pose2D))
   (desired
    :reader desired
    :initarg :desired
    :type geometry_msgs-msg:Pose2D
    :initform (cl:make-instance 'geometry_msgs-msg:Pose2D))
   (actual
    :reader actual
    :initarg :actual
    :type geometry_msgs-msg:Pose2D
    :initform (cl:make-instance 'geometry_msgs-msg:Pose2D)))
)

(cl:defclass PIDInfo (<PIDInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PIDInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PIDInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mcthuggets-msg:<PIDInfo> is deprecated: use mcthuggets-msg:PIDInfo instead.")))

(cl:ensure-generic-function 'error-val :lambda-list '(m))
(cl:defmethod error-val ((m <PIDInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mcthuggets-msg:error-val is deprecated.  Use mcthuggets-msg:error instead.")
  (error m))

(cl:ensure-generic-function 'desired-val :lambda-list '(m))
(cl:defmethod desired-val ((m <PIDInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mcthuggets-msg:desired-val is deprecated.  Use mcthuggets-msg:desired instead.")
  (desired m))

(cl:ensure-generic-function 'actual-val :lambda-list '(m))
(cl:defmethod actual-val ((m <PIDInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mcthuggets-msg:actual-val is deprecated.  Use mcthuggets-msg:actual instead.")
  (actual m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PIDInfo>) ostream)
  "Serializes a message object of type '<PIDInfo>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'error) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'desired) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'actual) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PIDInfo>) istream)
  "Deserializes a message object of type '<PIDInfo>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'error) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'desired) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'actual) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PIDInfo>)))
  "Returns string type for a message object of type '<PIDInfo>"
  "mcthuggets/PIDInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PIDInfo)))
  "Returns string type for a message object of type 'PIDInfo"
  "mcthuggets/PIDInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PIDInfo>)))
  "Returns md5sum for a message object of type '<PIDInfo>"
  "92edbf2a438afd03b29a449025eeb3aa")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PIDInfo)))
  "Returns md5sum for a message object of type 'PIDInfo"
  "92edbf2a438afd03b29a449025eeb3aa")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PIDInfo>)))
  "Returns full string definition for message of type '<PIDInfo>"
  (cl:format cl:nil "geometry_msgs/Pose2D error~%geometry_msgs/Pose2D desired~%geometry_msgs/Pose2D actual~%================================================================================~%MSG: geometry_msgs/Pose2D~%# This expresses a position and orientation on a 2D manifold.~%~%float64 x~%float64 y~%float64 theta~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PIDInfo)))
  "Returns full string definition for message of type 'PIDInfo"
  (cl:format cl:nil "geometry_msgs/Pose2D error~%geometry_msgs/Pose2D desired~%geometry_msgs/Pose2D actual~%================================================================================~%MSG: geometry_msgs/Pose2D~%# This expresses a position and orientation on a 2D manifold.~%~%float64 x~%float64 y~%float64 theta~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PIDInfo>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'error))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'desired))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'actual))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PIDInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'PIDInfo
    (cl:cons ':error (error msg))
    (cl:cons ':desired (desired msg))
    (cl:cons ':actual (actual msg))
))
