;; Auto-generated. Do not edit!


(when (boundp 'test::PIDInfo)
  (if (not (find-package "TEST"))
    (make-package "TEST"))
  (shadow 'PIDInfo (find-package "TEST")))
(unless (find-package "TEST::PIDINFO")
  (make-package "TEST::PIDINFO"))

(in-package "ROS")
;;//! \htmlinclude PIDInfo.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass test::PIDInfo
  :super ros::object
  :slots (_error _desired _actual ))

(defmethod test::PIDInfo
  (:init
   (&key
    ((:error __error) (instance geometry_msgs::Pose2D :init))
    ((:desired __desired) (instance geometry_msgs::Pose2D :init))
    ((:actual __actual) (instance geometry_msgs::Pose2D :init))
    )
   (send-super :init)
   (setq _error __error)
   (setq _desired __desired)
   (setq _actual __actual)
   self)
  (:error
   (&rest __error)
   (if (keywordp (car __error))
       (send* _error __error)
     (progn
       (if __error (setq _error (car __error)))
       _error)))
  (:desired
   (&rest __desired)
   (if (keywordp (car __desired))
       (send* _desired __desired)
     (progn
       (if __desired (setq _desired (car __desired)))
       _desired)))
  (:actual
   (&rest __actual)
   (if (keywordp (car __actual))
       (send* _actual __actual)
     (progn
       (if __actual (setq _actual (car __actual)))
       _actual)))
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/Pose2D _error
    (send _error :serialization-length)
    ;; geometry_msgs/Pose2D _desired
    (send _desired :serialization-length)
    ;; geometry_msgs/Pose2D _actual
    (send _actual :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/Pose2D _error
       (send _error :serialize s)
     ;; geometry_msgs/Pose2D _desired
       (send _desired :serialize s)
     ;; geometry_msgs/Pose2D _actual
       (send _actual :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/Pose2D _error
     (send _error :deserialize buf ptr-) (incf ptr- (send _error :serialization-length))
   ;; geometry_msgs/Pose2D _desired
     (send _desired :deserialize buf ptr-) (incf ptr- (send _desired :serialization-length))
   ;; geometry_msgs/Pose2D _actual
     (send _actual :deserialize buf ptr-) (incf ptr- (send _actual :serialization-length))
   ;;
   self)
  )

(setf (get test::PIDInfo :md5sum-) "92edbf2a438afd03b29a449025eeb3aa")
(setf (get test::PIDInfo :datatype-) "test/PIDInfo")
(setf (get test::PIDInfo :definition-)
      "geometry_msgs/Pose2D error
geometry_msgs/Pose2D desired
geometry_msgs/Pose2D actual
================================================================================
MSG: geometry_msgs/Pose2D
# This expresses a position and orientation on a 2D manifold.

float64 x
float64 y
float64 theta
")



(provide :test/PIDInfo "92edbf2a438afd03b29a449025eeb3aa")


