;; Auto-generated. Do not edit!


(when (boundp 'test::RobotState)
  (if (not (find-package "TEST"))
    (make-package "TEST"))
  (shadow 'RobotState (find-package "TEST")))
(unless (find-package "TEST::ROBOTSTATE")
  (make-package "TEST::ROBOTSTATE"))

(in-package "ROS")
;;//! \htmlinclude RobotState.msg.html


(defclass test::RobotState
  :super ros::object
  :slots (_vision_x _vision_y _vision_theta _xhat _yhat _thetahat _vx _vy _w _xhat_future _yhat_future _thetahat_future _predict_forward_seconds _correction ))

(defmethod test::RobotState
  (:init
   (&key
    ((:vision_x __vision_x) 0.0)
    ((:vision_y __vision_y) 0.0)
    ((:vision_theta __vision_theta) 0.0)
    ((:xhat __xhat) 0.0)
    ((:yhat __yhat) 0.0)
    ((:thetahat __thetahat) 0.0)
    ((:vx __vx) 0.0)
    ((:vy __vy) 0.0)
    ((:w __w) 0.0)
    ((:xhat_future __xhat_future) 0.0)
    ((:yhat_future __yhat_future) 0.0)
    ((:thetahat_future __thetahat_future) 0.0)
    ((:predict_forward_seconds __predict_forward_seconds) 0.0)
    ((:correction __correction) nil)
    )
   (send-super :init)
   (setq _vision_x (float __vision_x))
   (setq _vision_y (float __vision_y))
   (setq _vision_theta (float __vision_theta))
   (setq _xhat (float __xhat))
   (setq _yhat (float __yhat))
   (setq _thetahat (float __thetahat))
   (setq _vx (float __vx))
   (setq _vy (float __vy))
   (setq _w (float __w))
   (setq _xhat_future (float __xhat_future))
   (setq _yhat_future (float __yhat_future))
   (setq _thetahat_future (float __thetahat_future))
   (setq _predict_forward_seconds (float __predict_forward_seconds))
   (setq _correction __correction)
   self)
  (:vision_x
   (&optional __vision_x)
   (if __vision_x (setq _vision_x __vision_x)) _vision_x)
  (:vision_y
   (&optional __vision_y)
   (if __vision_y (setq _vision_y __vision_y)) _vision_y)
  (:vision_theta
   (&optional __vision_theta)
   (if __vision_theta (setq _vision_theta __vision_theta)) _vision_theta)
  (:xhat
   (&optional __xhat)
   (if __xhat (setq _xhat __xhat)) _xhat)
  (:yhat
   (&optional __yhat)
   (if __yhat (setq _yhat __yhat)) _yhat)
  (:thetahat
   (&optional __thetahat)
   (if __thetahat (setq _thetahat __thetahat)) _thetahat)
  (:vx
   (&optional __vx)
   (if __vx (setq _vx __vx)) _vx)
  (:vy
   (&optional __vy)
   (if __vy (setq _vy __vy)) _vy)
  (:w
   (&optional __w)
   (if __w (setq _w __w)) _w)
  (:xhat_future
   (&optional __xhat_future)
   (if __xhat_future (setq _xhat_future __xhat_future)) _xhat_future)
  (:yhat_future
   (&optional __yhat_future)
   (if __yhat_future (setq _yhat_future __yhat_future)) _yhat_future)
  (:thetahat_future
   (&optional __thetahat_future)
   (if __thetahat_future (setq _thetahat_future __thetahat_future)) _thetahat_future)
  (:predict_forward_seconds
   (&optional __predict_forward_seconds)
   (if __predict_forward_seconds (setq _predict_forward_seconds __predict_forward_seconds)) _predict_forward_seconds)
  (:correction
   (&optional __correction)
   (if __correction (setq _correction __correction)) _correction)
  (:serialization-length
   ()
   (+
    ;; float64 _vision_x
    8
    ;; float64 _vision_y
    8
    ;; float64 _vision_theta
    8
    ;; float64 _xhat
    8
    ;; float64 _yhat
    8
    ;; float64 _thetahat
    8
    ;; float64 _vx
    8
    ;; float64 _vy
    8
    ;; float64 _w
    8
    ;; float64 _xhat_future
    8
    ;; float64 _yhat_future
    8
    ;; float64 _thetahat_future
    8
    ;; float64 _predict_forward_seconds
    8
    ;; bool _correction
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _vision_x
       (sys::poke _vision_x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vision_y
       (sys::poke _vision_y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vision_theta
       (sys::poke _vision_theta (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _xhat
       (sys::poke _xhat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yhat
       (sys::poke _yhat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _thetahat
       (sys::poke _thetahat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vx
       (sys::poke _vx (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vy
       (sys::poke _vy (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _w
       (sys::poke _w (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _xhat_future
       (sys::poke _xhat_future (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yhat_future
       (sys::poke _yhat_future (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _thetahat_future
       (sys::poke _thetahat_future (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _predict_forward_seconds
       (sys::poke _predict_forward_seconds (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; bool _correction
       (if _correction (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _vision_x
     (setq _vision_x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vision_y
     (setq _vision_y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vision_theta
     (setq _vision_theta (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _xhat
     (setq _xhat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yhat
     (setq _yhat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _thetahat
     (setq _thetahat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vx
     (setq _vx (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vy
     (setq _vy (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _w
     (setq _w (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _xhat_future
     (setq _xhat_future (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yhat_future
     (setq _yhat_future (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _thetahat_future
     (setq _thetahat_future (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _predict_forward_seconds
     (setq _predict_forward_seconds (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; bool _correction
     (setq _correction (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get test::RobotState :md5sum-) "9ebf5c3bc78dc310a9067120721e6ec8")
(setf (get test::RobotState :datatype-) "test/RobotState")
(setf (get test::RobotState :definition-)
      "float64 vision_x
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

")



(provide :test/RobotState "9ebf5c3bc78dc310a9067120721e6ec8")


