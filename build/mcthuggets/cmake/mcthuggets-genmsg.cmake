# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "mcthuggets: 3 messages, 0 services")

set(MSG_I_FLAGS "-Imcthuggets:/home/robot/catkin_ws/src/mcthuggets/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(mcthuggets_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_custom_target(_mcthuggets_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mcthuggets" "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" ""
)

get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_custom_target(_mcthuggets_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mcthuggets" "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" ""
)

get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_custom_target(_mcthuggets_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mcthuggets" "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" "geometry_msgs/Pose2D"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets
)
_generate_msg_cpp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets
)
_generate_msg_cpp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets
)

### Generating Services

### Generating Module File
_generate_module_cpp(mcthuggets
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(mcthuggets_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(mcthuggets_generate_messages mcthuggets_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_cpp _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_cpp _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_cpp _mcthuggets_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mcthuggets_gencpp)
add_dependencies(mcthuggets_gencpp mcthuggets_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mcthuggets_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets
)
_generate_msg_eus(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets
)
_generate_msg_eus(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets
)

### Generating Services

### Generating Module File
_generate_module_eus(mcthuggets
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(mcthuggets_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(mcthuggets_generate_messages mcthuggets_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_eus _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_eus _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_eus _mcthuggets_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mcthuggets_geneus)
add_dependencies(mcthuggets_geneus mcthuggets_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mcthuggets_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets
)
_generate_msg_lisp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets
)
_generate_msg_lisp(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets
)

### Generating Services

### Generating Module File
_generate_module_lisp(mcthuggets
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(mcthuggets_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(mcthuggets_generate_messages mcthuggets_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_lisp _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_lisp _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_lisp _mcthuggets_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mcthuggets_genlisp)
add_dependencies(mcthuggets_genlisp mcthuggets_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mcthuggets_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets
)
_generate_msg_nodejs(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets
)
_generate_msg_nodejs(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets
)

### Generating Services

### Generating Module File
_generate_module_nodejs(mcthuggets
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(mcthuggets_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(mcthuggets_generate_messages mcthuggets_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_nodejs _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_nodejs _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_nodejs _mcthuggets_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mcthuggets_gennodejs)
add_dependencies(mcthuggets_gennodejs mcthuggets_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mcthuggets_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets
)
_generate_msg_py(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets
)
_generate_msg_py(mcthuggets
  "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose2D.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets
)

### Generating Services

### Generating Module File
_generate_module_py(mcthuggets
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(mcthuggets_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(mcthuggets_generate_messages mcthuggets_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/BallState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_py _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/RobotState.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_py _mcthuggets_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/robot/catkin_ws/src/mcthuggets/msg/PIDInfo.msg" NAME_WE)
add_dependencies(mcthuggets_generate_messages_py _mcthuggets_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mcthuggets_genpy)
add_dependencies(mcthuggets_genpy mcthuggets_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mcthuggets_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mcthuggets
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(mcthuggets_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(mcthuggets_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mcthuggets
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(mcthuggets_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(mcthuggets_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mcthuggets
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(mcthuggets_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(mcthuggets_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mcthuggets
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(mcthuggets_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(mcthuggets_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mcthuggets
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(mcthuggets_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(mcthuggets_generate_messages_py geometry_msgs_generate_messages_py)
endif()
