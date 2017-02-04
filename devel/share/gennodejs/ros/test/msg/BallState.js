// Auto-generated. Do not edit!

// (in-package test.msg)


"use strict";

let _serializer = require('../base_serialize.js');
let _deserializer = require('../base_deserialize.js');
let _finder = require('../find.js');

//-----------------------------------------------------------

class BallState {
  constructor() {
    this.vision_x = 0.0;
    this.vision_y = 0.0;
    this.xhat = 0.0;
    this.yhat = 0.0;
    this.vx = 0.0;
    this.vy = 0.0;
    this.xhat_future = 0.0;
    this.yhat_future = 0.0;
    this.predict_forward_seconds = 0.0;
    this.correction = false;
  }

  static serialize(obj, bufferInfo) {
    // Serializes a message object of type BallState
    // Serialize message field [vision_x]
    bufferInfo = _serializer.float64(obj.vision_x, bufferInfo);
    // Serialize message field [vision_y]
    bufferInfo = _serializer.float64(obj.vision_y, bufferInfo);
    // Serialize message field [xhat]
    bufferInfo = _serializer.float64(obj.xhat, bufferInfo);
    // Serialize message field [yhat]
    bufferInfo = _serializer.float64(obj.yhat, bufferInfo);
    // Serialize message field [vx]
    bufferInfo = _serializer.float64(obj.vx, bufferInfo);
    // Serialize message field [vy]
    bufferInfo = _serializer.float64(obj.vy, bufferInfo);
    // Serialize message field [xhat_future]
    bufferInfo = _serializer.float64(obj.xhat_future, bufferInfo);
    // Serialize message field [yhat_future]
    bufferInfo = _serializer.float64(obj.yhat_future, bufferInfo);
    // Serialize message field [predict_forward_seconds]
    bufferInfo = _serializer.float64(obj.predict_forward_seconds, bufferInfo);
    // Serialize message field [correction]
    bufferInfo = _serializer.bool(obj.correction, bufferInfo);
    return bufferInfo;
  }

  static deserialize(buffer) {
    //deserializes a message object of type BallState
    let tmp;
    let len;
    let data = new BallState();
    // Deserialize message field [vision_x]
    tmp = _deserializer.float64(buffer);
    data.vision_x = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [vision_y]
    tmp = _deserializer.float64(buffer);
    data.vision_y = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [xhat]
    tmp = _deserializer.float64(buffer);
    data.xhat = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [yhat]
    tmp = _deserializer.float64(buffer);
    data.yhat = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [vx]
    tmp = _deserializer.float64(buffer);
    data.vx = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [vy]
    tmp = _deserializer.float64(buffer);
    data.vy = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [xhat_future]
    tmp = _deserializer.float64(buffer);
    data.xhat_future = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [yhat_future]
    tmp = _deserializer.float64(buffer);
    data.yhat_future = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [predict_forward_seconds]
    tmp = _deserializer.float64(buffer);
    data.predict_forward_seconds = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [correction]
    tmp = _deserializer.bool(buffer);
    data.correction = tmp.data;
    buffer = tmp.buffer;
    return {
      data: data,
      buffer: buffer
    }
  }

  static datatype() {
    // Returns string type for a message object
    return 'test/BallState';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '16625b961fe41517c3be13a764c00cdd';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 vision_x
    float64 vision_y
    float64 xhat
    float64 yhat
    float64 vx
    float64 vy
    float64 xhat_future
    float64 yhat_future
    float64 predict_forward_seconds
    bool correction
    
    `;
  }

};

module.exports = BallState;
