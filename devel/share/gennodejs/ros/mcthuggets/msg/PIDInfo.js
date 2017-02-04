// Auto-generated. Do not edit!

// (in-package mcthuggets.msg)


"use strict";

let _serializer = require('../base_serialize.js');
let _deserializer = require('../base_deserialize.js');
let _finder = require('../find.js');
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class PIDInfo {
  constructor() {
    this.error = new geometry_msgs.msg.Pose2D();
    this.desired = new geometry_msgs.msg.Pose2D();
    this.actual = new geometry_msgs.msg.Pose2D();
  }

  static serialize(obj, bufferInfo) {
    // Serializes a message object of type PIDInfo
    // Serialize message field [error]
    bufferInfo = geometry_msgs.msg.Pose2D.serialize(obj.error, bufferInfo);
    // Serialize message field [desired]
    bufferInfo = geometry_msgs.msg.Pose2D.serialize(obj.desired, bufferInfo);
    // Serialize message field [actual]
    bufferInfo = geometry_msgs.msg.Pose2D.serialize(obj.actual, bufferInfo);
    return bufferInfo;
  }

  static deserialize(buffer) {
    //deserializes a message object of type PIDInfo
    let tmp;
    let len;
    let data = new PIDInfo();
    // Deserialize message field [error]
    tmp = geometry_msgs.msg.Pose2D.deserialize(buffer);
    data.error = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [desired]
    tmp = geometry_msgs.msg.Pose2D.deserialize(buffer);
    data.desired = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [actual]
    tmp = geometry_msgs.msg.Pose2D.deserialize(buffer);
    data.actual = tmp.data;
    buffer = tmp.buffer;
    return {
      data: data,
      buffer: buffer
    }
  }

  static datatype() {
    // Returns string type for a message object
    return 'mcthuggets/PIDInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '92edbf2a438afd03b29a449025eeb3aa';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    geometry_msgs/Pose2D error
    geometry_msgs/Pose2D desired
    geometry_msgs/Pose2D actual
    ================================================================================
    MSG: geometry_msgs/Pose2D
    # This expresses a position and orientation on a 2D manifold.
    
    float64 x
    float64 y
    float64 theta
    `;
  }

};

module.exports = PIDInfo;
