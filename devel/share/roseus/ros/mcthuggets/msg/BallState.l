;; Auto-generated. Do not edit!


(when (boundp 'mcthuggets::BallState)
  (if (not (find-package "MCTHUGGETS"))
    (make-package "MCTHUGGETS"))
  (shadow 'BallState (find-package "MCTHUGGETS")))
(unless (find-package "MCTHUGGETS::BALLSTATE")
  (make-package "MCTHUGGETS::BALLSTATE"))

(in-package "ROS")
;;//! \htmlinclude BallState.msg.html


(defclass mcthuggets::BallState
  :super ros::object
  :slots (_vision_x _vision_y _xhat _yhat _vx _vy _xhat_future _yhat_future _predict_forward_seconds _correction ))

(defmethod mcthuggets::BallState
  (:init
   (&key
    ((:vision_x __vision_x) 0.0)
    ((:vision_y __vision_y) 0.0)
    ((:xhat __xhat) 0.0)
    ((:yhat __yhat) 0.0)
    ((:vx __vx) 0.0)
    ((:vy __vy) 0.0)
    ((:xhat_future __xhat_future) 0.0)
    ((:yhat_future __yhat_future) 0.0)
    ((:predict_forward_seconds __predict_forward_seconds) 0.0)
    ((:correction __correction) nil)
    )
   (send-super :init)
   (setq _vision_x (float __vision_x))
   (setq _vision_y (float __vision_y))
   (setq _xhat (float __xhat))
   (setq _yhat (float __yhat))
   (setq _vx (float __vx))
   (setq _vy (float __vy))
   (setq _xhat_future (float __xhat_future))
   (setq _yhat_future (float __yhat_future))
   (setq _predict_forward_seconds (float __predict_forward_seconds))
   (setq _correction __correction)
   self)
  (:vision_x
   (&optional __vision_x)
   (if __vision_x (setq _vision_x __vision_x)) _vision_x)
  (:vision_y
   (&optional __vision_y)
   (if __vision_y (setq _vision_y __vision_y)) _vision_y)
  (:xhat
   (&optional __xhat)
   (if __xhat (setq _xhat __xhat)) _xhat)
  (:yhat
   (&optional __yhat)
   (if __yhat (setq _yhat __yhat)) _yhat)
  (:vx
   (&optional __vx)
   (if __vx (setq _vx __vx)) _vx)
  (:vy
   (&optional __vy)
   (if __vy (setq _vy __vy)) _vy)
  (:xhat_future
   (&optional __xhat_future)
   (if __xhat_future (setq _xhat_future __xhat_future)) _xhat_future)
  (:yhat_future
   (&optional __yhat_future)
   (if __yhat_future (setq _yhat_future __yhat_future)) _yhat_future)
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
    ;; float64 _xhat
    8
    ;; float64 _yhat
    8
    ;; float64 _vx
    8
    ;; float64 _vy
    8
    ;; float64 _xhat_future
    8
    ;; float64 _yhat_future
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
     ;; float64 _xhat
       (sys::poke _xhat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yhat
       (sys::poke _yhat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vx
       (sys::poke _vx (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _vy
       (sys::poke _vy (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _xhat_future
       (sys::poke _xhat_future (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _yhat_future
       (sys::poke _yhat_future (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
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
   ;; float64 _xhat
     (setq _xhat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yhat
     (setq _yhat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vx
     (setq _vx (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _vy
     (setq _vy (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _xhat_future
     (setq _xhat_future (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _yhat_future
     (setq _yhat_future (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _predict_forward_seconds
     (setq _predict_forward_seconds (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; bool _correction
     (setq _correction (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get mcthuggets::BallState :md5sum-) "16625b961fe41517c3be13a764c00cdd")
(setf (get mcthuggets::BallState :datatype-) "mcthuggets/BallState")
(setf (get mcthuggets::BallState :definition-)
      "float64 vision_x
float64 vision_y
float64 xhat
float64 yhat
float64 vx
float64 vy
float64 xhat_future
float64 yhat_future
float64 predict_forward_seconds
bool correction

")



(provide :mcthuggets/BallState "16625b961fe41517c3be13a764c00cdd")


