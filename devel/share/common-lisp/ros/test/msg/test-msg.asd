
(cl:in-package :asdf)

(defsystem "test-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "BallState" :depends-on ("_package_BallState"))
    (:file "_package_BallState" :depends-on ("_package"))
    (:file "PIDInfo" :depends-on ("_package_PIDInfo"))
    (:file "_package_PIDInfo" :depends-on ("_package"))
    (:file "RobotState" :depends-on ("_package_RobotState"))
    (:file "_package_RobotState" :depends-on ("_package"))
  ))