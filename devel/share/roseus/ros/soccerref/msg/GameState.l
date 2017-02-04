;; Auto-generated. Do not edit!


(when (boundp 'soccerref::GameState)
  (if (not (find-package "SOCCERREF"))
    (make-package "SOCCERREF"))
  (shadow 'GameState (find-package "SOCCERREF")))
(unless (find-package "SOCCERREF::GAMESTATE")
  (make-package "SOCCERREF::GAMESTATE"))

(in-package "ROS")
;;//! \htmlinclude GameState.msg.html


(defclass soccerref::GameState
  :super ros::object
  :slots (_home_score _away_score _home_bot_count _away_bot_count _remaining_seconds _play _reset_field _second_half ))

(defmethod soccerref::GameState
  (:init
   (&key
    ((:home_score __home_score) 0)
    ((:away_score __away_score) 0)
    ((:home_bot_count __home_bot_count) 0)
    ((:away_bot_count __away_bot_count) 0)
    ((:remaining_seconds __remaining_seconds) 0)
    ((:play __play) nil)
    ((:reset_field __reset_field) nil)
    ((:second_half __second_half) nil)
    )
   (send-super :init)
   (setq _home_score (round __home_score))
   (setq _away_score (round __away_score))
   (setq _home_bot_count (round __home_bot_count))
   (setq _away_bot_count (round __away_bot_count))
   (setq _remaining_seconds (round __remaining_seconds))
   (setq _play __play)
   (setq _reset_field __reset_field)
   (setq _second_half __second_half)
   self)
  (:home_score
   (&optional __home_score)
   (if __home_score (setq _home_score __home_score)) _home_score)
  (:away_score
   (&optional __away_score)
   (if __away_score (setq _away_score __away_score)) _away_score)
  (:home_bot_count
   (&optional __home_bot_count)
   (if __home_bot_count (setq _home_bot_count __home_bot_count)) _home_bot_count)
  (:away_bot_count
   (&optional __away_bot_count)
   (if __away_bot_count (setq _away_bot_count __away_bot_count)) _away_bot_count)
  (:remaining_seconds
   (&optional __remaining_seconds)
   (if __remaining_seconds (setq _remaining_seconds __remaining_seconds)) _remaining_seconds)
  (:play
   (&optional __play)
   (if __play (setq _play __play)) _play)
  (:reset_field
   (&optional __reset_field)
   (if __reset_field (setq _reset_field __reset_field)) _reset_field)
  (:second_half
   (&optional __second_half)
   (if __second_half (setq _second_half __second_half)) _second_half)
  (:serialization-length
   ()
   (+
    ;; int32 _home_score
    4
    ;; int32 _away_score
    4
    ;; int32 _home_bot_count
    4
    ;; int32 _away_bot_count
    4
    ;; int32 _remaining_seconds
    4
    ;; bool _play
    1
    ;; bool _reset_field
    1
    ;; bool _second_half
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _home_score
       (write-long _home_score s)
     ;; int32 _away_score
       (write-long _away_score s)
     ;; int32 _home_bot_count
       (write-long _home_bot_count s)
     ;; int32 _away_bot_count
       (write-long _away_bot_count s)
     ;; int32 _remaining_seconds
       (write-long _remaining_seconds s)
     ;; bool _play
       (if _play (write-byte -1 s) (write-byte 0 s))
     ;; bool _reset_field
       (if _reset_field (write-byte -1 s) (write-byte 0 s))
     ;; bool _second_half
       (if _second_half (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _home_score
     (setq _home_score (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _away_score
     (setq _away_score (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _home_bot_count
     (setq _home_bot_count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _away_bot_count
     (setq _away_bot_count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _remaining_seconds
     (setq _remaining_seconds (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; bool _play
     (setq _play (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _reset_field
     (setq _reset_field (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _second_half
     (setq _second_half (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get soccerref::GameState :md5sum-) "04b9504566012ef17377e5d0ae0aba41")
(setf (get soccerref::GameState :datatype-) "soccerref/GameState")
(setf (get soccerref::GameState :definition-)
      "int32 home_score
int32 away_score
int32 home_bot_count
int32 away_bot_count
int32 remaining_seconds
bool play
bool reset_field
bool second_half
")



(provide :soccerref/GameState "04b9504566012ef17377e5d0ae0aba41")


