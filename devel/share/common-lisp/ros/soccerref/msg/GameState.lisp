; Auto-generated. Do not edit!


(cl:in-package soccerref-msg)


;//! \htmlinclude GameState.msg.html

(cl:defclass <GameState> (roslisp-msg-protocol:ros-message)
  ((home_score
    :reader home_score
    :initarg :home_score
    :type cl:integer
    :initform 0)
   (away_score
    :reader away_score
    :initarg :away_score
    :type cl:integer
    :initform 0)
   (home_bot_count
    :reader home_bot_count
    :initarg :home_bot_count
    :type cl:integer
    :initform 0)
   (away_bot_count
    :reader away_bot_count
    :initarg :away_bot_count
    :type cl:integer
    :initform 0)
   (remaining_seconds
    :reader remaining_seconds
    :initarg :remaining_seconds
    :type cl:integer
    :initform 0)
   (play
    :reader play
    :initarg :play
    :type cl:boolean
    :initform cl:nil)
   (reset_field
    :reader reset_field
    :initarg :reset_field
    :type cl:boolean
    :initform cl:nil)
   (second_half
    :reader second_half
    :initarg :second_half
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass GameState (<GameState>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GameState>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GameState)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name soccerref-msg:<GameState> is deprecated: use soccerref-msg:GameState instead.")))

(cl:ensure-generic-function 'home_score-val :lambda-list '(m))
(cl:defmethod home_score-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:home_score-val is deprecated.  Use soccerref-msg:home_score instead.")
  (home_score m))

(cl:ensure-generic-function 'away_score-val :lambda-list '(m))
(cl:defmethod away_score-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:away_score-val is deprecated.  Use soccerref-msg:away_score instead.")
  (away_score m))

(cl:ensure-generic-function 'home_bot_count-val :lambda-list '(m))
(cl:defmethod home_bot_count-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:home_bot_count-val is deprecated.  Use soccerref-msg:home_bot_count instead.")
  (home_bot_count m))

(cl:ensure-generic-function 'away_bot_count-val :lambda-list '(m))
(cl:defmethod away_bot_count-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:away_bot_count-val is deprecated.  Use soccerref-msg:away_bot_count instead.")
  (away_bot_count m))

(cl:ensure-generic-function 'remaining_seconds-val :lambda-list '(m))
(cl:defmethod remaining_seconds-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:remaining_seconds-val is deprecated.  Use soccerref-msg:remaining_seconds instead.")
  (remaining_seconds m))

(cl:ensure-generic-function 'play-val :lambda-list '(m))
(cl:defmethod play-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:play-val is deprecated.  Use soccerref-msg:play instead.")
  (play m))

(cl:ensure-generic-function 'reset_field-val :lambda-list '(m))
(cl:defmethod reset_field-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:reset_field-val is deprecated.  Use soccerref-msg:reset_field instead.")
  (reset_field m))

(cl:ensure-generic-function 'second_half-val :lambda-list '(m))
(cl:defmethod second_half-val ((m <GameState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader soccerref-msg:second_half-val is deprecated.  Use soccerref-msg:second_half instead.")
  (second_half m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GameState>) ostream)
  "Serializes a message object of type '<GameState>"
  (cl:let* ((signed (cl:slot-value msg 'home_score)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'away_score)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'home_bot_count)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'away_bot_count)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'remaining_seconds)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'play) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'reset_field) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'second_half) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GameState>) istream)
  "Deserializes a message object of type '<GameState>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'home_score) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'away_score) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'home_bot_count) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'away_bot_count) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'remaining_seconds) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:slot-value msg 'play) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'reset_field) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'second_half) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GameState>)))
  "Returns string type for a message object of type '<GameState>"
  "soccerref/GameState")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GameState)))
  "Returns string type for a message object of type 'GameState"
  "soccerref/GameState")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GameState>)))
  "Returns md5sum for a message object of type '<GameState>"
  "04b9504566012ef17377e5d0ae0aba41")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GameState)))
  "Returns md5sum for a message object of type 'GameState"
  "04b9504566012ef17377e5d0ae0aba41")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GameState>)))
  "Returns full string definition for message of type '<GameState>"
  (cl:format cl:nil "int32 home_score~%int32 away_score~%int32 home_bot_count~%int32 away_bot_count~%int32 remaining_seconds~%bool play~%bool reset_field~%bool second_half~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GameState)))
  "Returns full string definition for message of type 'GameState"
  (cl:format cl:nil "int32 home_score~%int32 away_score~%int32 home_bot_count~%int32 away_bot_count~%int32 remaining_seconds~%bool play~%bool reset_field~%bool second_half~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GameState>))
  (cl:+ 0
     4
     4
     4
     4
     4
     1
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GameState>))
  "Converts a ROS message object to a list"
  (cl:list 'GameState
    (cl:cons ':home_score (home_score msg))
    (cl:cons ':away_score (away_score msg))
    (cl:cons ':home_bot_count (home_bot_count msg))
    (cl:cons ':away_bot_count (away_bot_count msg))
    (cl:cons ':remaining_seconds (remaining_seconds msg))
    (cl:cons ':play (play msg))
    (cl:cons ':reset_field (reset_field msg))
    (cl:cons ':second_half (second_half msg))
))
